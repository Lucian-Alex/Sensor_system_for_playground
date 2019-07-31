  // **** Potentiometer NO-360 SECTION ****

int in_pin = A0;  // potentiometer pin
int previous_value = 0;
int current_value;
int value_change;  // change in recorded value between current record and the previous

  // **** NRF Section ****

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

  //create an RF24 object
RF24 radio(9, 10);  // CE, CSN

  //address through which two radio transceiver modules communicate
const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
  
#define MSG_Length 32   //radio message length - 32 recommended
#define DELAY_TX 1000   //delay time


void setup() {

    // **** only nRF SECTION  ****

  Serial.begin(9600);
  
  radio.begin();
  radio.stopListening();  //Set module as transmitter   
  radio.openWritingPipe(addressList[3]);  //Set the address

}

void loop() {

  current_value = analogRead(in_pin);  
  value_change = abs(current_value - previous_value);
    
  if(value_change >= 10)
     {
        Serial.println("Clock Active");
        //Serial.println(current_value);
        
          //Send message to receiver
        const char message[MSG_Length] = "Clock Active";
        radio.write(&message, sizeof(message));
     }
  
  /*    else
          {
            Serial.println("Clock Idle");
  
            //Send message to receiver
            const char message[MSG_Length] = "Clock Idle";
            radio.write(&message, sizeof(message));
            
          }
  */

  previous_value = current_value;
  delay(DELAY_TX);
}
