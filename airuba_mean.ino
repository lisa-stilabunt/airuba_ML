//Based on airuba_min as a basic sensor test
//with additional data handling
//screening out outlying values
//taking several measurements
//and sending the average

#include <SoftwareSerial.h>
#include <Adafruit_SHT31.h>

//averaging stuff
unsigned long previousMillis = 0;
const long interval = 120000; //time period over which readings are taken for averaging
const long frequency = 2000; //delay between readings

float pm1=0;
float pm25=0;
float pm10=0;
float temp=0;
float hum=0;
int p_count=0;
int ht_count=0;

//PMS5003 comms stuff
typedef enum {
    FIRSTBYTE,
    SECONDBYTE,
    READ,
    
}dataparcer;

int transmitPM01(char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[2]<<8) + thebuf[3]); //count PM1.0 value of the air detector module
  return PM01Val;
}
 
int transmitPM2_5(char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[4]<<8) + thebuf[5]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }
 
int transmitPM10(char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[6]<<8) + thebuf[7]); //count PM10 value of the air detector module  
  return PM10Val;
}

//instantiate temp humidity sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

//serial connection for pms5003 particulate sensor
SoftwareSerial swSer(14, 12);  //RX, TX --- GPIO pins for RX and TX, WeMos D1 Mini is D5, D6, NodeMCU is D5, D6

void setup() { 
  Serial.begin(9600);                                    

  swSer.begin(9600);
  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) { // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
  }
    
  delay(30000); //let the sensor stabilize on startup
} 


//more pms5003 data stuff
char a;
char databuffer[32];
uint8_t i_data=0;
char mystring[10];
bool dataready = false;
dataparcer datastate = FIRSTBYTE;
int PM01Value=0;          //define PM1.0 value of the air detector module
int PM2_5Value=0;         //define PM2.5 value of the air detector module
int PM10Value=0;         //define PM10 value of the air detector module


void loop() {  

  unsigned long currentMillis = millis();
  
    // get data from PMS5003 sensor
    while (swSer.available() > 0) {  //wait for data at software serial
      a = swSer.read();
  
      switch(datastate){
        case FIRSTBYTE:
          if (a == 0x42) {
            datastate = SECONDBYTE;     
          }
          break;
        case SECONDBYTE:
          if (a == 0x4d) {
            datastate = READ;          
          }
          break;
        case READ:
          databuffer[i_data] = a;
          i_data++;
          if(i_data>29){
            datastate = FIRSTBYTE;
            dataready = true;
            i_data = 0;
          }
          break;
        default:
          break;
      } 
    }

 
    if(dataready){

        PM01Value = transmitPM01(databuffer); //count PM1.0 value of the air detector module
        PM2_5Value = transmitPM2_5(databuffer); //count PM2.5 value of the air detector module
        PM10Value = transmitPM10(databuffer); //count PM10 value of the air detector module 
            
        dataready = false;
        
        pm1=pm1+PM01Value;
        pm25=pm25+PM2_5Value;
        pm10=pm10+PM10Value;
        p_count++;
    }

    // get data from SHT31 sensor
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    temp = temp+t;
    hum = hum+h;
    ht_count++;

    if (currentMillis - previousMillis >= interval) {
       previousMillis = currentMillis;
       PM01Value=pm1/p_count;
       PM2_5Value=pm25/p_count;
       PM10Value=pm10/p_count;
       h=hum/ht_count;
       t=temp/ht_count;

       p_count=0;
       ht_count=0;
       pm1=0;
       pm25=0;
       pm10=0;
       temp=0;
       hum=0;

       //send data to serial monitor
       Serial.print("pm1: ");
       Serial.println(PM01Value);
       Serial.print("pm2_5: ");
       Serial.println(PM2_5Value);
       Serial.print("pm10: ");
       Serial.println(PM10Value);
       Serial.print("h: ");
       Serial.println(h);
       Serial.print("t: ");
       Serial.println(t);
    }

    //wait 2 seconds
    delay(frequency);
}
