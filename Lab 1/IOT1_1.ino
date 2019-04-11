// A0 pin to which Photo Resistor is connected
int lightPin = 0;

// Variable to store the Photo Resistor reading
int lightValue = 0;

void setup()
{
// Setup communications with the serial monitor
Serial.begin(9600);
}

void loop()
{
// Read the light-level value from A0 pin
lightValue = analogRead(lightPin);
// Write the value to the serial monitor
Serial.println(lightValue);
// Wait 1 second before next reading
delay(1000);
}
