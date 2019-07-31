//****  PRESSURE(scale) sensor SECTION  ****  
#include "HX711.h"

#define calibration_factor 1100000 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  3
#define CLK  2

HX711 scale; // create scale sensor object
float current_weight;
float previous_weight = 0;
float change;


//****   nRF SECTION  ****

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

  //create an RF24 object
RF24 radio(9, 10);  // CE, CSN

  //address through which two modules communicate.
const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};

#define MSG_Length 32   //radio message length- how many characters
#define DELAY_TX 2100   //delay time- NO LESS than 2000 or the pressure sensor won't work properly !!!!

  
void setup()
{   
//****  PRESSURE sensor SECTION  ****
  Serial.begin(9600);

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0


//****  nRF SECTION  ****
    
  radio.begin();
  radio.stopListening();    //Set transceiver as transmitter   
  radio.openWritingPipe(addressList[1]);    //set the pipe address for this transceiver
 
}

void loop()
{   

  current_weight = scale.get_units();  //scale.get_units() returns a float
  change = abs(current_weight - previous_weight);

    //if significant weight, store it in a String, convert it to char array and transmit it
  if(change > 0.05 && current_weight > 0.05)
      {
        
          String weight = "";
          weight.concat(abs(current_weight));

          char message[MSG_Length] = {0};
          weight.toCharArray(message, MSG_Length);

            //Serial.println(message);
          radio.write(&message, sizeof(message));

          Serial.println((String) "Reading: " + abs(current_weight) + " kg");
      
      } 

/* else
      {
        char message[MSG_Length] = "0";
        radio.write(&message, sizeof(message));
      }
*/                
//Serial.println((String) "Reading: " + abs(current_weight) + " kg");

  previous_weight = current_weight;
  delay(DELAY_TX);   
}
