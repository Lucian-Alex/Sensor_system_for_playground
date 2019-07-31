  //Include Libraries for SPI bus communication and nRF24L01 radio transceivers
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

  //create an RF24 object
RF24 radio(9, 10);  // CE, CSN

  //addresses of the transmitters
const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};

  //radio message length (how many characters) - NO LESS than specified for the transmitters communicating with this receiver
#define MSG_Length 32

  //Excel row number for each sensor column - will be incremented w/ each input  
int poleCounter = 2;
int stairsCounter = 2;
int tunnelCounter = 2;
int clockCounter = 2;
int slideCounter = 2;
int weatherCounter = 2;

void setup() {

  Serial.begin(9600);
  
  //Serial.println("CLEARDATA"); // clears sheet starting at row 2
  Serial.println("CLEARSHEET"); // clears sheet starting at row 1
      
    // define 5 columns in Excel
  Serial.println("LABEL, Pole Time, Pole Data, Stairs Time, Stairs Data, Tunnel Time, Tunnel Data, Clock Time, Clock Data, Slide Time, Slide Data");
  
  radio.begin();    // radio initialization
  
    // open reading pipes and assign them hex addresses
  for(byte i = 0; i < 6; i++)
    {
      radio.openReadingPipe(i, addressList[i]);  
    }
  
  radio.startListening();   // set device as receiver - ALWAYS after opening reading pipes

}

  
void loop() 
{    
//Reading an incoming message and storing it in Excel

  uint8_t pipeNumber;    
  if( radio.available(&pipeNumber) ) 
    {  
      char message[MSG_Length] = {0};
      radio.read(&message, sizeof(message));
      // Serial.println(message);   //debugging
  
      String time = (String)millis(); 
  
        //goes through each pipe number to identify the transmitter and it prints the message and timestamp in the suitable Excel cells  
      switch(pipeNumber)
        {
          case 0:
            {
              Serial.println("CELL,SET,A" + (String)poleCounter + "," + time); 
              Serial.println("CELL,SET,B" + (String)poleCounter + "," + (String)message);
              poleCounter++;
              break;
            }
          
          case 1:
            {
              Serial.println("CELL,SET,C" + (String)stairsCounter + "," + time); 
              Serial.println("CELL,SET,D" + (String)stairsCounter + "," + (String)message);
              stairsCounter++;
              break;
            }
          
          case 2:
            {
              Serial.println("CELL,SET,E" + (String)tunnelCounter + "," + time); 
              Serial.println("CELL,SET,F" + (String)tunnelCounter + "," + (String)message);
              tunnelCounter++;
              break;  
            }
      
          case 3:
            {
              Serial.println("CELL,SET,G" + (String)clockCounter + "," + time); 
              Serial.println("CELL,SET,H" + (String)clockCounter + "," + (String)message);
              clockCounter++;
              break;
            }
      
          case 4:
            {
              Serial.println("CELL,SET,I" + (String)slideCounter + "," + time); 
              Serial.println("CELL,SET,J" + (String)slideCounter + "," + (String)message);
              slideCounter++;
              break;
            }
          
          case 5:
            {
              Serial.println("CELL,SET,K" + (String)weatherCounter + "," + time); 
              Serial.println("CELL,SET,L" + (String)weatherCounter + "," + (String)message);
              slideCounter++;
              break;
            }
        }    
     }                       
}    
