//programm for Arduino MEGA 2560 for measuring of indoor and outdoor climate
//project: Measuring Apartment 214 in H12 El Gouna, part of research work at Campus El Gouna
//Christoph T. I. Banhardt
//last modified: 14 March 2015

//sources: 
//Temperature and Humidity measurnment with DHT22 by Rob Tillaart, v 0.1.03
//Temperature reading of sensors LM35DZ by Tapan Mojidra, project on instructables.com
//Pressure Readings with BMP180 by Mike Grusin, v 10
//Clock reading (and writing) of RTC DS3234 by Henning Karlsen, (C) 2014

#include <dht.h>                                 //library for digital temperature and humidity sensor RHT03 (type DH22)
#include <SFE_BMP180.h>                          //library for pressure sensor
#include <Wire.h>
#include <DS3234.h>                              //library for Real Time Clock DS3234

dht DHT1_0;
dht DHT1_1;                                        //initializing all DHT22 temperature and humidity sensors
dht DHT1_2;
dht DHT1_3;
dht DHT1_4;
dht DHT1_5;
dht DHT1_6;
dht DHT2_0;
dht DHT2_1;
dht DHT2_2;
dht DHT2_3;
dht DHT2_4;
dht DHT2_5;
dht DHT2_6;
dht DHT99;

DS3234 rtc(8);                                    //initialize clock on pin 8

#define DHT22_PIN10 30
#define DHT22_PIN11 31                             //digital pin connections for DHT22 sensors
#define DHT22_PIN12 32
#define DHT22_PIN13 33
#define DHT22_PIN14 34
#define DHT22_PIN15 35
#define DHT22_PIN16 36
#define DHT22_PIN20 40
#define DHT22_PIN21 41
#define DHT22_PIN22 42
#define DHT22_PIN23 43
#define DHT22_PIN24 44
#define DHT22_PIN25 45
#define DHT22_PIN26 46
#define DHT22_PIN99 2

struct                                           //nessecary structure for temp sensors DHT22 being used
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};
                                  
int time = 0;                                  //variables for timing
int count = 0;
Time t1;
Time t2;

float t_10 = 0;
float t_11 = 0;                                // measured values - temperatures
float t_12 = 0;
float t_13 = 0;
float t_14 = 0;
float t_15 = 0;
float t_16 = 0;
float t_20 = 0;
float t_21 = 0;
float t_22 = 0;
float t_23 = 0;
float t_24 = 0;
float t_25 = 0;
float t_26 = 0;
float t_out = 0;

float rh_13 = 0;                              // measured values - relative humidity
float rh_23 = 0;
float rh_out = 0;

SFE_BMP180 pressure;
double p_in;                                  //measured pressure

void setup(){
  
  pressure.begin();
  rtc.begin();
  
  Serial.begin(4800);
  
//  pinMode(sens_t11,INPUT);
//  pinMode(sens_t12,INPUT);
//  pinMode(sens_t13,INPUT);
//  pinMode(sens_t14,INPUT);
//  pinMode(sens_t15,INPUT);
//  pinMode(sens_t21,INPUT);
//  pinMode(sens_t22,INPUT);
//  pinMode(sens_t23,INPUT);
//  pinMode(sens_t24,INPUT);
//  pinMode(sens_t25,INPUT);
//  pinMode(sens_tout,INPUT);
  
  delay(1500);
  
  Serial.print("#; ");
  Serial.print("date; ");
  Serial.print("time; ");
  Serial.print("t_10; ");
  Serial.print("t_11; ");
  Serial.print("t_12; ");
  Serial.print("t_13; ");
  Serial.print("t_14; ");
  Serial.print("t_15; ");
  Serial.print("t_16; ");
  Serial.print("t_20; ");
  Serial.print("t_21; ");
  Serial.print("t_22; ");
  Serial.print("t_23; ");
  Serial.print("t_24; ");
  Serial.print("t_25; ");
  Serial.print("t_26; ");
  Serial.print("t_out; ");
  Serial.print("rh_13; ");
  Serial.print("rh_23; ");
  Serial.print("rh_out; ");
  Serial.println("p_in; ");
}

void loop(){
  
  t1 = rtc.getTime();
  t2 = rtc.getTime();
  
  while (t1.min == t2.min){
    uint32_t start = micros();
    int chk10 = DHT1_0.read22(DHT22_PIN10);
    int chk11 = DHT1_1.read22(DHT22_PIN11);
    int chk12 = DHT1_2.read22(DHT22_PIN12);
    int chk13 = DHT1_3.read22(DHT22_PIN13);
    int chk14 = DHT1_4.read22(DHT22_PIN14);
    int chk15 = DHT1_5.read22(DHT22_PIN15);
    int chk16 = DHT1_6.read22(DHT22_PIN16);
    int chk20 = DHT2_0.read22(DHT22_PIN20);    
    int chk21 = DHT2_1.read22(DHT22_PIN21);
    int chk22 = DHT2_2.read22(DHT22_PIN22);
    int chk23 = DHT2_3.read22(DHT22_PIN23);
    int chk24 = DHT2_4.read22(DHT22_PIN24);
    int chk25 = DHT2_5.read22(DHT22_PIN25);
    int chk26 = DHT2_6.read22(DHT22_PIN26);
    int chk99 = DHT99.read22(DHT22_PIN99);
    uint32_t stop = micros();    
    
    t_10 = t_10 + DHT1_0.temperature;
    t_11 = t_11 + DHT1_1.temperature;
    t_12 = t_12 + DHT1_2.temperature;
    t_13 = t_13 + DHT1_3.temperature;
    t_14 = t_14 + DHT1_4.temperature;
    t_15 = t_15 + DHT1_5.temperature;
    t_16 = t_16 + DHT1_6.temperature;
    t_20 = t_20 + DHT2_0.temperature;    
    t_21 = t_21 + DHT2_1.temperature;
    t_22 = t_22 + DHT2_2.temperature;
    t_23 = t_23 + DHT2_3.temperature;
    t_24 = t_24 + DHT2_4.temperature;
    t_25 = t_25 + DHT2_5.temperature;
    t_26 = t_26 + DHT2_6.temperature;
    
    t_out = t_out + DHT99.temperature;
        
    count = count + 1;
//    Serial.print(count);
    t2 = rtc.getTime();
    delay(950);
  }
  
  t_10 = t_10 / count; 
  t_11 = t_11 / count;
  t_12 = t_12 / count;
  t_13 = t_13 / count;
  t_14 = t_14 / count;
  t_15 = t_15 / count;
  t_16 = t_16 / count;
  t_20 = t_20 / count;
  t_21 = t_21 / count;
  t_22 = t_22 / count;
  t_23 = t_23 / count;
  t_24 = t_24 / count;
  t_25 = t_25 / count;
  t_26 = t_26 / count;
  t_out = t_out / count;
  time = time + 1;
  
  p_in = getPressure(); 
  
  Serial.print(time);
  Serial.print("; ");
  Serial.print(t1.year);
  Serial.print("/");
  Serial.print(t1.mon);
  Serial.print("/");
  Serial.print(t1.date);
  Serial.print("; ");
  Serial.print(t1.hour);
  Serial.print(":");
  Serial.print(t1.min);
  Serial.print("; ");
//  Serial.print(rtc.getTimeStr());
//  Serial.print("; ");
  Serial.print(t_10);
  Serial.print("; ");
  Serial.print(t_11);
  Serial.print("; ");
  Serial.print(t_12);
  Serial.print("; ");  
  Serial.print(t_13); 
  Serial.print("; ");
  Serial.print(t_14);
  Serial.print("; ");
  Serial.print(t_15);
  Serial.print("; ");
  Serial.print(t_16);
  Serial.print("; ");
  Serial.print(t_20);
  Serial.print("; ");
  Serial.print(t_21);
  Serial.print("; ");
  Serial.print(t_22);
  Serial.print("; ");  
  Serial.print(t_23); 
  Serial.print("; ");
  Serial.print(t_24);
  Serial.print("; ");
  Serial.print(t_25);
  Serial.print("; ");
  Serial.print(t_26);
  Serial.print("; ");
  Serial.print(t_out); 
  Serial.print("; ");
  Serial.print(DHT1_3.humidity, 1); 
  Serial.print("; ");
  Serial.print(DHT2_3.humidity, 1); 
  Serial.print("; ");
  Serial.print(DHT99.humidity, 1);
  Serial.print("; ");
  Serial.print(p_in);
  Serial.println("; ");
  
//  delay(834);
  count = 0;
  t_10 = 0;
  t_11 = 0;
  t_12 = 0;
  t_13 = 0;
  t_14 = 0;
  t_15 = 0;
  t_16 = 0;
  t_20 = 0;
  t_21 = 0;
  t_22 = 0;
  t_23 = 0;
  t_24 = 0;
  t_25 = 0;
  t_26 = 0;
  t_out = 0;
}


double getPressure()
{
  char status;
  double T,P,p0,a;
  status = pressure.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0)
    {
      status = pressure.startPressure(3);
      if (status != 0)
      {
        delay(status);
        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
  

