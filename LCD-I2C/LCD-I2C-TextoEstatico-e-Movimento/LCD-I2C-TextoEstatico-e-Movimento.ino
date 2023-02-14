/*********
LCD CRISTAL LIQUIDO - I2C
*********/

#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

String messageStatic0 = "ROBSON  BRASIL";
String messageStatic1 = "Manaus - AM - Brasil";
String messageStatic3 = "LOBO ALFA  AUTOMACAO";
String messageToScroll2 = "Estou testando o LCD de Cristal Liquido no formato I2C";

// Function to scroll text
// The function acepts the following arguments:
// row: row number where the text will be displayed
// message: message to scroll
// delayTime: delay between each character shifting
// lcdColumns: number of columns of your LCD
void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup(){
  // initialize LCD
  lcd.begin();
  // turn on LCD backlight                      
  lcd.backlight();
}

void loop(){
  // set cursor to first column, first row
  lcd.setCursor(3, 0);
  // print static message
  lcd.print(messageStatic0);
  // set cursor to second column, first row
  lcd.setCursor(0, 1);
  // print static message
  lcd.print(messageStatic1);
  // set cursor to second column, first row
  lcd.setCursor(0, 3);
  // print static message
  lcd.print(messageStatic3);    
  // print scrolling message
  scrollText(2, messageToScroll2, 250, lcdColumns);
}