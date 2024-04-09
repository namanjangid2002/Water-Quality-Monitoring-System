#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ONE_WIRE_BUS 4 // GPIO pin for DS18B20 sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define TDS_PIN 32

const int potPin = A0;
float ph;
float pHValue = 0;
float TDSValue = 0;
float t = 0;

void setup(void)
{

  Serial.begin(9600);
  sensors.begin();
  pinMode(potPin, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("TDS, pH, Temp");
  display.display();
  delay(2000);
  Serial.println("");
}

void loop(void)
{
  readTemperature();
  readTDS();
  readPh();
  delay(2); // allow the cpu to switch to other tasks
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("TDS: ");
  display.println(TDSValue);
  display.print("pH: ");
  display.println(1.7 * pHValue * 3.3 * 3.3 / 4095.0);
  display.print("Temp: ");
  display.print(t);
  display.println(" C");
  display.display();
}

float readTemperature()
{
  sensors.requestTemperatures();
  t = sensors.getTempCByIndex(0);
  if (isnan(t))
  {
    // Serial.println("Failed to read from sensor!");
    return -1;
  }
  else
  {
    Serial.print(t);
    Serial.print(",");
    return t;
  }
}

float readPh()
{
  pHValue = analogRead(potPin);

  float voltage = 1.7 * pHValue * (3.3 / 4095.0);
  ph = (3.3 * voltage);
  Serial.println(ph);
  return ph;
  delay(500);
}
float readTDS()
{
  TDSValue = analogRead(TDS_PIN);
  Serial.print(TDSValue);
  Serial.print(",");
  return TDSValue;
  delay(500);
}
