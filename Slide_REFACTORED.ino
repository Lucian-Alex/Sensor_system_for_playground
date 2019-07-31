  //**** Proximity Sensor HC-SR04 Section ****

const int triggerPin = 2; // white wire
const int echoPin = 3; // blue wire

  //**** nRF Section ******

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

  //create an RF24 object
RF24 radio(9, 10);  // CE, CSN

  //address through which two modules communicate.
const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
  
#define MSG_Length 32   //radio message length- 32 recommended
#define DELAY_TX 100    //delay time

void setup() {
//**** Proximity SECTION ****

  pinMode(triggerPin, OUTPUT);    // Sets the triggerPin as Output
  pinMode(echoPin, INPUT);      // Sets the echoPin as Input
 
//**** nRF SECTION ****
  
  Serial.begin(9600); 
  radio.begin();
  radio.stopListening();  //Set module as transmitter  
  radio.openWritingPipe(addressList[4]);  //set the address
}


void loop() {
  int distance = detect_Distance(triggerPin, echoPin);

  if(distance < 30) 
        {
          Serial.println("Slide Active");
          Serial.println(distance);
          
          const char message[MSG_Length] = "Slide Active";
          radio.write(&message, sizeof(message));
        }
        
/*      else
            {
              Serial.println("Slide Idle");
          
              const char message[MSG_Length] = "Slide Idle";
              radio.write(&message, sizeof(message));
            }
*/            
  delay(DELAY_TX);
}

int detect_Distance(byte trigPin, byte echoPin)
  {
    int duration, distance;
      
      // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    
      // Sets the trigPin on HIGH state for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
      // Reads the echoPin, returns the sound wave travel time in microseconds, calculates the distance
    duration = pulseIn(echoPin, HIGH); 
    distance = duration * 0.034/2;

    return distance;
  }
