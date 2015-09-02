//programm for Arduino MEGA 2560 for measuring of indoor and outdoor climate
//project: Measuring Apartment 214 in H12 El Gouna, part of research work at Campus El Gouna
//Christoph T. I. Banhardt
//last modified: 01 September 2015

//sources: 
//Temperature and Humidity measurnment with DHT22 by Rob Tillaart, v 0.1.03
//Temperature reading of sensors LM35DZ by Tapan Mojidra, project on instructables.com
//Pressure Readings with BMP180 by Mike Grusin, v 10
//Clock reading (and writing) of RTC DS3234 by Henning Karlsen, (C) 2014

#include <dht.h>                                 //library for digital temperature and humidity sensor RHT03 (type DH22)
#include <SFE_BMP180.h>                          //library for pressure sensor
#include <Wire.h>
#include <DS3234.h>                              //library for Real Time Clock DS3234

dht DHT1;                                        //initializing all 3 sensors
dht DHT2;
dht DHT3;

DS3234 rtc(8);                                  //initialize clock on pin 8

#define DHT22_PIN1 2                             //digital pin connections for sensors
#define DHT22_PIN2 3
#define DHT22_PIN3 4

struct                                           //nessecary structure for sensors being used
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
Time t;
Time d;

int sens_t11 = A0;                              //sensors connectors t[xy]
int sens_t12 = A1;                              //x:  Position 1 = inside at window wall        y:  height 1 = near floor
int sens_t13 = 2;                               //    Position 2 = inside at opposite wall          height 2 = 1m
int sens_t14 = A2;                              //    out = outside measurnment                     height 3 = 1.8m
int sens_t15 = A3;                              //                                                  height 4 = 3m
int sens_t21 = A4;                              //                                                  height 5 = near ceiling
int sens_t22 = A5;
int sens_t23 = 3;
int sens_t24 = A6;
int sens_t25 = A7;
int sens_tout = 4;

float t_11 = 0;                                 // measured values - temperatures
float t_12 = 0;
float t_13 = 0;
float t_14 = 0;
float t_15 = 0;
float t_21 = 0;
float t_22 = 0;
float t_23 = 0;
float t_24 = 0;
float t_25 = 0;
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
  
  pinMode(sens_t11,INPUT);
  pinMode(sens_t12,INPUT);
  pinMode(sens_t13,INPUT);
  pinMode(sens_t14,INPUT);
  pinMode(sens_t15,INPUT);
  pinMode(sens_t21,INPUT);
  pinMode(sens_t22,INPUT);
  pinMode(sens_t23,INPUT);
  pinMode(sens_t24,INPUT);
  pinMode(sens_t25,INPUT);
  pinMode(sens_tout,INPUT);
  
  Serial.print("#; ");
  Serial.print("date; ");
  Serial.print("time; ");
  Serial.print("t_11; ");
  Serial.print("t_12; ");
  Serial.print("t_13; ");
  Serial.print("t_14; ");
  Serial.print("t_15; ");
  Serial.print("t_21; ");
  Serial.print("t_22; ");
  Serial.print("t_23; ");
  Serial.print("t_24; ");
  Serial.print("t_25; ");
  Serial.print("t_out; ");
  Serial.print("rh_13; ");
  Serial.print("rh_23; ");
  Serial.print("rh_out; ");
  Serial.println("p_in; ");
}

void loop(){
  
  t = rtc.getTime();
  
  while (t.sec < 59){
    t_11 = t_11 + 500 * analogRead(sens_t11) / 1024;
    t_12 = t_12 + 500 * analogRead(sens_t12) / 1024;
    t_14 = t_14 + 500 * analogRead(sens_t14) / 1024;
    t_15 = t_15 + 500 * analogRead(sens_t15) / 1024;
    t_21 = t_21 + 500 * analogRead(sens_t21) / 1024;
    t_22 = t_22 + 500 * analogRead(sens_t22) / 1024;
    t_24 = t_24 + 500 * analogRead(sens_t24) / 1024;
    t_25 = t_25 + 500 * analogRead(sens_t25) / 1024;
    count = count + 1;
    t = rtc.getTime();
    delay(980);
  }
   
  t_11 = t_11 / count + 0.29;
  t_12 = t_12 / count + 0.11;
  t_14 = t_14 / count - 0.07;
  t_15 = t_15 / count + 0.9;
  t_21 = t_21 / count + 0.93;
  t_22 = t_22 / count + 0.94;
  t_24 = t_24 / count + 0.93;
  t_25 = t_25 / count + 0.9;
  time = time + 1;
  
  uint32_t start = micros();
  int chk1 = DHT1.read22(DHT22_PIN1);
  int chk2 = DHT2.read22(DHT22_PIN2);
  int chk3 = DHT3.read22(DHT22_PIN3);
  uint32_t stop = micros();
  
  p_in = getPressure(); 
  
  Serial.print(time);
  Serial.print("; ");
  Serial.print(t.year);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.date);
  Serial.print("; ");
  Serial.print(rtc.getTimeStr());
  Serial.print("; ");
  Serial.print(t_11);
  Serial.print("; ");
  Serial.print(t_12);
  Serial.print("; ");  
  Serial.print(DHT1.temperature - 1.59, 1); 
  Serial.print("; ");
  Serial.print(t_14);
  Serial.print("; ");
  Serial.print(t_15);
  Serial.print("; ");
  Serial.print(t_21);
  Serial.print("; ");
  Serial.print(t_22);
  Serial.print("; ");  
  Serial.print(DHT2.temperature - 1.35, 1); 
  Serial.print("; ");
  Serial.print(t_24);
  Serial.print("; ");
  Serial.print(t_25);
  Serial.print("; ");
  Serial.print(DHT3.temperature - 2.03, 1); 
  Serial.print("; ");
  Serial.print(DHT1.humidity + 1.25, 1); 
  Serial.print("; ");
  Serial.print(DHT2.humidity - 0.51, 1); 
  Serial.print("; ");
  Serial.print(DHT3.humidity - 1.32, 1);
  Serial.print("; ");
  Serial.print(p_in);
  Serial.println("; ");
  
  delay(834);
  count = 0;
  t_11 = 0;
  t_12 = 0;
  t_14 = 0;
  t_15 = 0;
  t_21 = 0;
  t_22 = 0;
  t_24 = 0;
  t_25 = 0;
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
  
