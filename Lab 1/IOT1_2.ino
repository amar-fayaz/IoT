#include <SoftwareSerial.h>
SoftwareSerial esp8266(11, 12); // RX, TX

int lightPin = 0;
int lightValue = 0;
int count = 0;
char msgBuffer[64];
int msgLen = 0;
char commandStr[64];
void setup() {
// init serial communications
Serial.begin(9600);
esp8266.begin(19200);
esp8266.setTimeout(10000);
// reset WLAN
esp8266.println("AT+RST");
if (!esp8266.findUntil("ready", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+RST␣failed");
}
// config WLAN
esp8266.println("AT+CWMODE=1");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CWMODE=1␣failed");
}
esp8266.println("AT+CWJAP=\"CN-Students\",\"CnStudentsWiFiAccess123\"");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CWJAP=\"Nomad_EXT\"...␣failed");
}
// get IP Address
esp8266.println("AT+CIFSR");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CIFSR");
}
// setup UDP connection
esp8266.println("AT+CIPSTART=\"UDP\",\"192.168.1.126\",9999,8888,0");
if (!esp8266.findUntil("OK", "ERROR")) {
Serial.println("ESP␣ERROR:␣AT+CIPSTART␣failed");
}
}
void loop() {
// build message payload
lightValue = analogRead(lightPin);
sprintf(msgBuffer, "3156393 \; %d \; %d", ++count, lightValue);
msgLen = strlen(msgBuffer);
// send message
sprintf(commandStr, "AT+CIPSEND=%d", msgLen);
esp8266.println(commandStr);
delay(1000);
esp8266.write(msgBuffer);
Serial.println("Sent: UDP");
Serial.println(msgBuffer);
delay(4000);
}
