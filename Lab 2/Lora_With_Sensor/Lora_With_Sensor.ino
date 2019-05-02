#include <TinyLoRa.h>
#include <SPI.h>


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 3 

#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
uint8_t NwkSkey[16] = { 0x63, 0x05, 0x89, 0x03, 0x08, 0xEA, 0x03, 0x60, 0x72, 0x01, 0x41, 0x5B, 0xB0, 0x50, 0x05, 0xA0 };

uint8_t AppSkey[16] = { 0x86, 0xE9, 0xE8, 0xB0, 0x41, 0x18, 0x26, 0x0C, 0xA4, 0xAB, 0xED, 0x44, 0xDF, 0xDE, 0x47, 0x9D };

uint8_t DevAddr[4] = { 0x26, 0x01, 0x16, 0x6A };

// Network Session Key (MSB)
//uint8_t NwkSkey[16] = { 0x63, 0x05, 0x89, 0x03, 0x08, 0xEA, 0x03, 0x60, 0x72, 0x01, 0x41, 0x5B, 0xB0, 0x50, 0x05, 0xA0 };

// Application Session Key (MSB)
//uint8_t AppSkey[16] = { 0x86, 0xE9, 0xE8, 0xB0, 0x41, 0x18, 0x26, 0x0C, 0xA4, 0xAB, 0xED, 0x44, 0xDF, 0xDE, 0x47, 0x9D };

// Device Address (MSB)
//uint8_t DevAddr[4] = { 0x26, 0x06, 0x16, 0x6A };

/************************** Example Begins Here ***********************************/
// Data Packet to Send to TTN
unsigned char loraData[11] = {"hello LoRa"};

// How many times data transfer should occur, in seconds
const unsigned int sendInterval = 2;

// Pinout for Adafruit Feather 32u4 LoRa
TinyLoRa lora = TinyLoRa(2, 10);

// Pinout for Adafruit Feather M0 LoRa
//TinyLoRa lora = TinyLoRa(3, 8);


void setup()
{
  delay(2000);
  Serial.begin(9600);
  while (! Serial);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  // Initialize LoRa
  Serial.print("Starting LoRa...");
  // define multi-channel sending
  lora.setChannel(MULTI);
  // set datarate
  lora.setDatarate(SF7BW125);
  if(!lora.begin())
  {
    Serial.println("Failed");
    Serial.println("Check your radio");
    while(true);
  }
  Serial.println("Setup Complete");
}

void loop()
{
  char payload[30];
  char temperature[10];
  char hum[10];
  int msgLen = 0;
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    dtostrf(event.temperature, 3 , 2, temperature);
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    //payload = event.relative_humidity;
    dtostrf(event.relative_humidity, 3 , 2, hum);
  }
  Serial.println("Sending LoRa Data...");

  sprintf (payload, "Temperature: %s, Humidity: %s", temperature, hum);//, event.relative_humidity);
  msgLen = strlen(payload);

    
  Serial.println(payload);
  lora.sendData(payload, msgLen, lora.frameCounter);
  Serial.print("Frame Counter: ");Serial.println(lora.frameCounter);
  lora.frameCounter++;

  Serial.println("delaying...");
  delay(sendInterval * 1000);
}
