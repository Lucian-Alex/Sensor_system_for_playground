//**** Weather station section - made up of humidity & temperature sensor, rain sensor and light sensor ****

  //**** DHT Humidity & temperature sensor resources ****
    #include "dht.h"
    #define dht_apin A0 // Analog Pin sensor is connected to
     
    dht DHT;  //create object for the humidity & temperature sensor
    const int sensorMin = 0;     // sensor minimum
    const int sensorMax = 1024;  // sensor maximum


  //**** TSL2561 Light sensor resources ****
    #include <Wire.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_TSL2561_U.h>
    
    Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

  
  //**** Rain sensor resources ****
    void displaySensorDetails(void)
      {
        sensor_t sensor;
        tsl.getSensor(&sensor);
        delay(500);
      }
    
    void configureSensor(void) 
      {
        /* You can also manually set the gain or enable auto-gain support */
        // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
        // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
        tsl.enableAutorain_range(true);            /* Auto-gain ... switches automatically between 1x and 16x */
        
        /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
        tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
        // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
        // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
      }


// **** nRF SECTION ****

  #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>
  
    //create an RF24 object
  RF24 radio(9, 10);  // pins: CE, CSN
  
    //addresses through which two radio transceiver modules communicate
  const uint64_t addressList[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};
     
  #define MSG_Length 32 //radio message length - 32 recommended
  
    //Wait before accessing sensor again - in this case: NO LESS THAN 2 SECONDS !!! 
  #define DELAY_TX 7000 

void setup(){

//****  Weather Station Section  ****

  Serial.begin(9600);
  
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  Serial.println("Light Sensor Test");
  Serial.println("");
  
  delay(1500);   //Delay to let sensors start
  
    // Initialise the light sensor
  while ( !tsl.begin() )
    {
      /* Problem detecting the TSL2561 */
      Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR !");
    }
         
  displaySensorDetails(); // Display some basic information on this sensor
  configureSensor();  // Setup the sensor gain and integration time
  Serial.println("Sensors ready !"); 


//****  nRF SECTION  ****
  
  radio.begin();
  radio.stopListening();  //Set module as transmitter  
  radio.openWritingPipe(addressList[5]);  //Set the transmitter's pipe address
}

void loop(){
    
//**** DHT sensor readings **** 
  
  DHT.read11(dht_apin); 
  
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

    
//**** Rains sensor readings ****
  
  int rainSensorReading = analogRead(A1);
  
  // map rain sensor readings (0 to 1024) to 4 cases
  int rain_range = map(rainSensorReading, sensorMin, sensorMax, 0, 3);
  
  // rain_range value:
  switch (rain_range) {
     case 0:      // Sensor getting wet
        Serial.println("Flood");
        break;
     case 1:      // Sensor getting wet
        Serial.println("Rain Warning");
        break;
     case 2:      // Sensor dry
        Serial.println("Not Raining");
        break;
  }
  delay(2);  // delay between reads
  
    
//**** TSL event creation and sensor readings ****
  
  sensors_event_t event;
  tsl.getEvent(&event);
 
    //Display the results (light is measured in lux)
  if (event.light)
      {
        Serial.print(event.light); 
        Serial.println(" lux");
      }
  else
      {
        /* If event.light = 0 lux the sensor is probably saturated
           and no reliable data could be generated! */
        Serial.println("Sensor overload");
      }
    
  delay(DELAY_TX);  //Wait before accessing sensor again - NOT less than 2 seconds !!!
 
}

//Send message to receiver      
      String text = "";
      text.concat(<the value>);
    
      char message[MSG_Length] = {0};
      text.toCharArray(message, MSG_Length);
      
      radio.write(&message, sizeof(message));
      
      Serial.print("Message is: ");
      Serial.println(message);
