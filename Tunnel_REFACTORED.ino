//****   Proximity Sensor HC-SR04 SECTION   ****

#define TUNNEL_DIAMETER 65

  // pin numbers for all 3 proximity sensors
const uint8_t trigPin_1 = 2;
const uint8_t echoPin_1 = 3;
const uint8_t trigPin_2 = 4;
const uint8_t echoPin_2 = 5;
const uint8_t trigPin_3 = 6;
const uint8_t echoPin_3 = 7;


// ****  nRF SECTION  ****

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

  //create an RF24 object
RF24 radio(9, 10);  // pins: CE, CSN

  //addresses through which two radio transceiver modules communicate
const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
    
#define MSG_Length 32   //radio message length - 32 recommended
#define DELAY_TX 1000   //delay time


void setup() {
                      
//****  Prox sensor SECTION  ****
      
      //sets pins as outputs/inputs
    pinMode(trigPin_1, OUTPUT);
    pinMode(echoPin_1, INPUT);
    pinMode(trigPin_2, OUTPUT);
    pinMode(echoPin_2, INPUT);
    pinMode(trigPin_3, OUTPUT);
    pinMode(echoPin_3, INPUT);
    
    
//****  nRF SECTION  ****
    
    Serial.begin(9600);
    radio.begin();
    radio.stopListening();  //Set module as transmitter  
    radio.openWritingPipe(addressList[2]);  //Set the transmitter's pipe address
  
}           

void loop() {

  float child_height;
  
    //get the distances detected by all 3 sensors
  float distance_1 = detect_Distance(trigPin_1, echoPin_1);
  float distance_2 = detect_Distance(trigPin_2, echoPin_2);
  float distance_3 = detect_Distance(trigPin_3, echoPin_3);
  float minimum_distance;
  
  if(distance_1 < TUNNEL_DIAMETER && distance_2 < TUNNEL_DIAMETER && distance_3 < TUNNEL_DIAMETER)
    {
        //calculate the minimum detected distance
      minimum_distance = find_minimum(distance_1, distance_2, distance_3);

        //calculate the height of a child crawling through the tunnel by doubling the distance from wrist to neck
      child_height = (TUNNEL_DIAMETER - minimum_distance) * 2;
      
         //Send the height as message to receiver      
      String text = "";
      text.concat(child_height);
    
      char message[MSG_Length] = {0};
      text.toCharArray(message, MSG_Length);
      
      radio.write(&message, sizeof(message));
      
      Serial.print("Message is: ");
      Serial.println(message);
    }

  delay(DELAY_TX);

}

float detect_Distance(byte triggerPin, byte echoPin)
  {
    float duration, distance;
      
      // Clears the trigPin
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    
      // Sets the trigPin on HIGH state for 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    
      // Reads the echoPin, returns the sound wave travel time in microseconds, calculates the distance
    duration = pulseIn(echoPin, HIGH); 
    distance = duration * 0.034/2;

    return distance;
  }

float find_minimum(float x, float y, float z)
  {
    if (x <= y && x <= z) 
                return x;
      else if (y <= x && y <= z) 
                return y;
      else if (z <= x && z <= y)
                return z;
  }
