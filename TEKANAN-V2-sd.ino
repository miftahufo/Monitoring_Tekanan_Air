//#include <DS3231.h>
#include  <virtuabotixRTC.h>  //Library used
#include <SoftwareSerial.h>
SoftwareSerial ArduinoUno (4, 3);

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//sd card
#include <SPI.h>
#include <SD.h>


File myFile;
virtuabotixRTC myRTC(6, 7, 8); //If you change the wiring change the pins here also

int pinCS = 10;

void setup() {
    Serial.begin(9600);
    lcd.init();   
    lcd.backlight();
    pinMode(pinCS, OUTPUT);
    
    Serial.println("Datalogger Sensor");

  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
  myRTC.setDS1302Time(15, 22, 21, 7, 14, 1, 2018);

  ArduinoUno.begin(4800);
}
 
void loop(){
int sensorVal=analogRead(A0);
Serial.print("Sensor Value: ");
Serial.print(sensorVal);
  lcd.setCursor(0,0);
  lcd.print("Value:");
  lcd.print(sensorVal);

float voltage = (sensorVal*5.0)/1024.0;
    Serial.print("Volts: ");
    Serial.print(voltage);
      lcd.setCursor(10,0);
      lcd.print("V");
      lcd.print(voltage);
   
  float pressure_pascal = (3.0*((float)voltage-0.47))*1000000.0;
  float pressure_bar = pressure_pascal/10e5;
    Serial.print("Pressure = ");
    Serial.print(pressure_bar);
    Serial.println(" bars");
      lcd.setCursor(0,1);
      lcd.print("Pressure:");
      lcd.print(pressure_bar);

       //kirim data ke esp
    ArduinoUno.print(pressure_bar);
    ArduinoUno.println("\n");
    delay(1000);

    myFile = SD.open("dbsensor.txt", FILE_WRITE);
    if (myFile)
  {
    myFile.print(myRTC.dayofmonth);myFile.print("|"); 
    myFile.print(myRTC.month);myFile.print("|"); 
    myFile.print(myRTC.year);myFile.print("|");
    myFile.print(myRTC.hours);myFile.print("|"); 
    myFile.print(myRTC.minutes);myFile.print("|");
    myFile.print(myRTC.seconds);myFile.print("|");  
    myFile.print(sensorVal);myFile.print("|");
    myFile.print(voltage);myFile.print("V |");
    myFile.print(pressure_bar);myFile.print("bars");
    myFile.println("");
    myFile.close();
    
    Serial.println(" | Data Tersimpan");
  }
   // if the file didn't open, print an error:
  else {
    Serial.println("error opening");
  }
  delay(1000);
  
}
 
