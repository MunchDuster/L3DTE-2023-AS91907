//QAPASS LCD display with I2C attachment

//Wiring:
//I2C - Arduino Nano
//GND - GND
//VCC - 5V
//SDA - A4
//SCL - A5

#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){
  // initialize the lcd 
  lcd.init();                      
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("Woop woop!");
}

void loop(){
  
}
