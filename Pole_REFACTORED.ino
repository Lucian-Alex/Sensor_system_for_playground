//****  Gyro SECTION - Measuring Angle with a MPU-6050(GY-521)  ****

#include<Wire.h>

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
boolean A = false;
int minVal = 265;
int maxVal = 402;

double actual_x;
double actual_y;
double actual_z;

double initial_x = 0;
double initial_y = 0;
double initial_z = 0;

double change_x;
double change_y;
double change_z;


//****  nRF SECTION  ****

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

  //create an RF24 object
RF24 radio(9, 10);  // CE, CSN

  //address through which two radio transceiver modules communicate
const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
  
#define MSG_Length 32   //define radio message length- 32 recommended  
#define DELAY_TX 300   //define delay in transmission


void setup()
{
//****  Gyro SECTION  ****

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);   


//****  nRF SECTION  ****
  radio.begin();
  radio.stopListening();    //Set module as transmitter
  radio.openWritingPipe(addressList[0]);    //set the address for this transceiver
 
}
void loop()
{
//****  Gyro section  ****
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  
  AcX = Wire.read()<<8|Wire.read();
  AcY = Wire.read()<<8|Wire.read();
  AcZ = Wire.read()<<8|Wire.read();
      
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);

  actual_x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  actual_y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  actual_z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  change_x = abs(actual_x - initial_x);
  change_y = abs(actual_y - initial_y);
  change_z = abs(actual_z - initial_z);
  

//****  Send message to receiver  ****

  if(change_x >= 10 || change_z >= 10 || change_y >= 10)
    {
     A = true;
     Serial.println("Pole Active");
     
     char message[MSG_Length] = "Pole Active";
     radio.write(&message, sizeof(message));
    }

/*    else
        {
          A = false;
          Serial.println("Pole Idle");
          
          char message[MSG_Length] = "Pole Idle";
          radio.write(&message, sizeof(message));
        }   
*/
  initial_x = actual_x;
  initial_y = actual_y;
  initial_z = actual_z;

delay(DELAY_TX);

}
