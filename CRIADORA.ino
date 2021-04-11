/*
 * CRIADORA
 */
#include <Time.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE    DHT22
#define DHTPIN 2 
#define calor 3
#define luz 4
#define buzzer 5
#define pulsador 6


unsigned long tiempo;
int intervalo = 1000;
int estadoPulsador;

float temperatura;
float humedad;

time_t dias;

DHT_Unified dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  rtc.begin();

  
  pinMode(calor, OUTPUT);
  pinMode(luz, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pulsador, INPUT);

  lcd.init();                
  lcd.backlight();
  
  lcd.setCursor(4,0);
  lcd.print("Jorge Roman");
  lcd.setCursor(1,1);
  lcd.print("Criadora Automatica");
  lcd.setCursor(4,2);
  lcd.print("Version 1.0");

   if (!rtc.begin()) 
   {
      Serial.println(F("Error de conexion RTC"));
      while (1);
   }
   if (rtc.lostPower()) 
   {
      //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      //rtc.adjust(DateTime(2021, 4, 11, 9, 20, 0));
   }
   
  delay(2000);
  lcd.clear();
}

void loop() 
{
  DateTime now = rtc.now();

  estadoPulsador = digitalRead(pulsador);

  lcd.setCursor(0,0);
  lcd.print("Dia: ");

  lcd.setCursor(0,1);
  lcd.print("Temperatura: ");
  lcd.setCursor(13,1);
  lcd.print(temperatura);
  lcd.setCursor(18,1);
  lcd.print((char)223);

  lcd.setCursor(0,2);
  lcd.print("Humedad: ");
  lcd.setCursor(9,2);
  lcd.print(humedad);
  lcd.setCursor(14,2);
  lcd.print("%");

  lcd.setCursor(0,3);
  lcd.print("Calor: ");

  lcd.setCursor(12,3);
  lcd.print("Luz: ");

///////////////////////////////////////////////////////////////////PULSADOR DE REINICIO DIAS///////////////////////////////////////////
  
  if(estadoPulsador == HIGH)
  {
    now.day = dias;
  }

  if()
  {
    
  }

///////////////////////////////////////////////////////////////////CALOR ACTIVADO POR TEMPERATURA//////////////////////////////////////
  
  if(temperatura <= 26)
  {
    digitalWrite(calor, HIGH);
    lcd.setCursor(7,3);
    lcd.print("ON ");
  }
  else
  {
    digitalWrite(calor, LOW);
    lcd.setCursor(7,3);
    lcd.print("OFF");
  }

///////////////////////////////////////////////////////////////////BUZZER ACTIVADO POR TEMPERATURA/////////////////////////////////////
  
  if((temperatura > 38) || (temperatura < 15))
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }

///////////////////////////////////////////////////////////////////LUZ ACTIVADA PRIMER TRAMO NOCHE///////////////////////////////////////

  if((now.hour()>=20) && (now.minute()>=00) && (now.hour()<=23) && (now.minute()<00))
  {
    digitalWrite(luz, HIGH);
    lcd.setCursor(17,3);
    lcd.print("ON ");
  }
  else
  {
    digitalWrite(luz, LOW);
    lcd.setCursor(17,3);
    lcd.print("OFF");
  }

///////////////////////////////////////////////////////////////////LUZ ACTIVADA SEGUNDO TRAMO NOCHE//////////////////////////////////////

    if((now.hour()>=03) && (now.minute()>=00) && (now.hour()<=06) && (now.minute()<00))
  {
    digitalWrite(luz, HIGH);
    lcd.setCursor(17,3);
    lcd.print("ON ");
  }
  else
  {
    digitalWrite(luz, LOW);
    lcd.setCursor(17,3);
    lcd.print("OFF");
  }

///////////////////////////////////////////////////////////////////LECTURA DE DHT CADA 1 SEGUNDO///////////////////////////////////////

  if(millis() - tiempo >= intervalo)
  {
    tiempo = millis();
  
    sensors_event_t event;
    
    dht.temperature().getEvent(&event);
      temperatura = event.temperature;
   
    dht.humidity().getEvent(&event);
      humedad = event.relative_humidity;
      
  } 
}
