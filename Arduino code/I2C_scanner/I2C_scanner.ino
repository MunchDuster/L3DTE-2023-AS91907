//I2C device scanner
#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}
void loop()
{
  Serial.println("Scanning...");
  
  bool deviceFound = false;
  for (byte address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission(); // Returns 0 if a device did acknowledge to the address.
    
    if (error == 0)
    {
      Serial.print("I2C device found at ");
      LogAddress(address);
      deviceFound = true;
    }
  }
  if (deviceFound)
    Serial.println("Done.");
  else
    Serial.println("No I2C devices found."); 
  delay(5000); // wait 5 seconds for next scan
}
void LogAddress(byte address) {
  Serial.print("address 0x");
  if (address < 16) 
    Serial.print("0");
  Serial.println(address, HEX);
}
