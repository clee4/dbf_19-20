#include "HX711.h"
#include <SPI.h>
#include <SD.h>


File myFile;

// change this to match your SD shield or module;
const int chipSelect = 10;
String file_name = "DATALOG.txt";

// change this to match your SD shield or module;
HX711 scale;

//HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A2;
const int LOADCELL_SCK_PIN = A3;

//Pitot Tube Output
const int PITOT = A1;

const int NUM = -9999999;

unsigned long time; 

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  sd_card("---NEW DATA STARTS NOW---");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(1000);

}

void loop()
{
  time = micros()/100000;
  sd_card(String(get_weight())+ "," + String(read_pitot()) + "," + String(time));
  Serial.println(String(get_weight())+ "," + String(read_pitot()) + "," + String(time)); 
  delay(100);
}


int read_pitot()
{
  int pitot_val = analogRead(PITOT);
  Serial.print("Pitot Tube Reading: ");
  Serial.println(pitot_val);
  return (pitot_val);
}

long get_weight()
{
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
    return (reading);
  }
  Serial.println("HX711 not found.");
  return (NUM);
}

void sd_card(String data)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(file_name, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(data);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + file_name);
  }

}
