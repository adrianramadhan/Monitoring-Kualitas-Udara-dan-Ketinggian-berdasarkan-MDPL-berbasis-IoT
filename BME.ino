#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLyjUFb1kc"
#define BLYNK_DEVICE_NAME "Projek SMANJI"

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme;
BlynkTimer timer;

char auth[] = "NZB4giCN_HA3wjEvRbnFaoUz5DHr94Qb";
char ssid[] = "WIFI-TE1";
char pass[] = "Wifipnc2020";

float temperature;
float pressure;
float altitude;
float humidity;
float gas;
double dewPoint;

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println("BME680 test");

  if (!bme.begin()){
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, send_sensor);
 bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);
}

void loop()
{
  Blynk.run();
  timer.run();
  send_sensor();
}

  double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}

 void send_sensor () {
  float temperature = bme.temperature;
  float pressure = bme.pressure / 100.0;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.humidity;
  float gas = bme.gas_resistance / 1000.0;
  double dewPoint = dewPointFast(temperature, humidity);
 
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, altitude);
  Blynk.virtualWrite(V4, gas);
  Blynk.virtualWrite(V5, dewPoint);
  Blynk.virtualWrite(V6, pressure);
  Blynk.virtualWrite(V3, humidity);
Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Dew Point = ");
  Serial.print(dewPoint);
  Serial.println(" *C");

  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");

  Serial.println();
  delay(2000);
 }