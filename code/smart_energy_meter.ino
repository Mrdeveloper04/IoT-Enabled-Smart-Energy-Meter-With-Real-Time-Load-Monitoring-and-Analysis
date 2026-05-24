#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "YOUR_BLYNK_AUTH_TOKEN";
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

PZEM004Tv30 pzem(D6, D7);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BUTTON_PIN D3

BlynkTimer timer;

float voltage, current, power, energy;

void sendData()
{
  voltage = pzem.voltage();
  current = pzem.current();
  power = pzem.power();
  energy = pzem.energy();

  if (isnan(voltage)) voltage = 0;
  if (isnan(current)) current = 0;
  if (isnan(power)) power = 0;
  if (isnan(energy)) energy = 0;

  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, power);
  Blynk.virtualWrite(V3, energy);

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("V:");
  lcd.print(voltage);

  lcd.print(" I:");
  lcd.print(current);

  lcd.setCursor(0,1);
  lcd.print("P:");
  lcd.print(power);

  lcd.print(" E:");
  lcd.print(energy);
}

void checkButton()
{
  if (digitalRead(BUTTON_PIN) == LOW)
  {
    pzem.resetEnergy();
    delay(500);
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(2000L, sendData);
}

void loop()
{
  Blynk.run();
  timer.run();
  checkButton();
}
