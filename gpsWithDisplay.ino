  /*
  The Adafruit GFX Graphics core library is also required
  https://github.com/adafruit/Adafruit-GFX-Library
  Be sure to install it!
  With adafruit_SSD1351 library
  */
  
// You can use any (4 or) 5 pins 
#define sclk 2
#define mosi 3
#define dc   8
#define cs   10
#define rst  9

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF




//Make sure to install the adafruit GPS library from https://github.com/adafruit/Adafruit-GPS-Library
#include <Adafruit_GPS.h> //Load the GPS Library. Make sure you have installed the library form the adafruit site above
#include <SoftwareSerial.h> //Load the Software Serial Library. This library in effect gives the arduino additional serial ports

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>

#include <SPI.h> //Oled display uses the SPI


// Option 1: use any pins for OLED but a little slower
//Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_SSD1351 tft = Adafruit_SSD1351(cs, dc, rst);

SoftwareSerial btSerial(4,5); // Bluetooth Software Serial

SoftwareSerial mySerial(3, 2); //Initialize SoftwareSerial, and tell it you will be connecting through pins 2 and 3
Adafruit_GPS GPS(&mySerial); //Create GPS object

String NMEA1;  //We will use this variable to hold our first NMEA sentence
String NMEA2;  //We will use this variable to hold our second NMEA sentence
char c;       //Used to read the characters spewing from the GPS module



void setup()
{
  Serial.begin(9600);
  btSerial.begin(38400);  //Turn on the Serial Monitor
  
  GPS.begin(9600);       //Turn GPS on at baud rate of 9600
  GPS.sendCommand("$PGCMD,33,0*6D"); // Turn Off GPS Antenna Update
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Tell GPS we want only $GPRMC and $GPGGA NMEA sentences
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate


   tft.begin(); 
   tft.fillScreen(BLACK);
   tft.setFont(&FreeMonoBoldOblique12pt7b);
   testdrawtext("Welcome", WHITE,5,50);
   tft.setFont();
   testdrawtext("Kunchala Anil",BLUE,40,57);
   delay(1000);
   tft.fillScreen(BLACK);
   //tft.setFont(&FreeSerif9pt7b);
  }


void loop()                     // run over and over again
{
  readGPS();  //This is a function we define below which reads two NMEA sentences from GPS

}
void readGPS() { //This function will read and remember two NMEA sentences from GPS
  clearGPS();    //Serial port probably has old or corrupt data, so begin by clearing it all out
  while (!GPS.newNMEAreceived()) { //Keep reading characters in this loop until a good NMEA sentence is received
    c = GPS.read(); //read a character from the GPS
  }
  GPS.parse(GPS.lastNMEA());  //Once you get a good NMEA, parse it
  NMEA1 = GPS.lastNMEA();    //Once parsed, save NMEA sentence into NMEA1
  while (!GPS.newNMEAreceived()) { //Go out and get the second NMEA sentence, should be different type than the first one read above.
    c = GPS.read();
  }
     tft.fillScreen(BLACK);

  GPS.parse(GPS.lastNMEA());
  NMEA2 = GPS.lastNMEA();
  btSerial.println(NMEA1);
  btSerial.println(NMEA2);
  btSerial.println("");

  Serial.print("GPS FIX : ");
  Serial.println(GPS.fix);
  testdrawtext("Fix : ", WHITE,5,25);
  char bufFix[1];
  itoa(GPS.fix,bufFix,2);
   
  testdrawtext(bufFix,RED,40,25);

  Serial.print("Latitude : ");
  Serial.print(GPS.latitude); //Write measured latitude to file
  Serial.print(" ");
  Serial.println(GPS.lat); //Which hemisphere N or S

  char bufLat[8];
  dtostrf(GPS.latitude,2,2,bufLat);
  testdrawtext("Lat : ",WHITE,5,35);
 
  testdrawtext(bufLat,WHITE,40,35);

  testdrawtext("- ",WHITE,90,35);
  
  char latBuf[] = {GPS.lat,'\0'};
  Serial.print("latBuf -->");
  Serial.println(latBuf);
  
  testdrawtext(latBuf,WHITE,95,35);
  
  //sprintf(bufLat,"LAT : %l - %c",GPS.latitude,GPS.lat);
  //estdrawtext(bufLat,WHITE,5,35);
  
  Serial.print("Longitude : ");
  Serial.print(GPS.longitude); //Write measured longitude to file
  Serial.print(" ");
  Serial.println(GPS.lon); //Which Hemisphere E or W

  char bufLon[8];

  dtostrf(GPS.longitude,2,2,bufLat);
  testdrawtext("Lon : ",WHITE,5,45);
  testdrawtext(bufLat,WHITE,40,45);
  
  testdrawtext("- ",WHITE,90,45);
  
  char lonBuf[] = {GPS.lon,'\0'};
  Serial.print("lonBuf -->");
  Serial.println(lonBuf);
  
  testdrawtext(lonBuf,WHITE,95,45);
  
  //sprintf(bufLon,"LON : %l - %c",12.23,GPS.lon);
  //testdrawtext(bufLon,WHITE,5,45);

  Serial.print("Buffer... For Kongitude");
  Serial.println(bufLon);

    
  Serial.print("Altitude : ");
  Serial.println(GPS.altitude);
  char bufAlt[5];
  dtostrf(GPS.altitude,2,2,bufAlt);
  testdrawtext("Alti : ",WHITE,5,55);
  testdrawtext(bufAlt,WHITE,45,55);
   

  Serial.print("Speed : ");
  Serial.println(GPS.speed);
  Serial.println(" ");
  
  char bufSpeed[5];
  dtostrf(GPS.speed,2,2,bufSpeed);
  testdrawtext("speed : ",WHITE,5,65);
  testdrawtext(bufSpeed,WHITE,50,65);
 


  Serial.print(GPS.hour+5);Serial.print(":");//GMT adjustment +5:30
  Serial.print(GPS.minute+30);Serial.print(":");
  Serial.print(GPS.seconds);Serial.println("");

  Serial.print(GPS.year);Serial.print("/");
  Serial.print(GPS.month);Serial.print("/");
  Serial.print(GPS.day);Serial.println("");

  char bufDateTime[20];
  sprintf(bufDateTime,"UTC : %d:%d:%d %d/%d/%d ",GPS.hour,GPS.minute,GPS.seconds,GPS.year,GPS.month,GPS.day);
  //sprintf(bufDateTime,"%d:%d:%d",GPS.hour,GPS.minute,GPS.seconds);
  testdrawtext(bufDateTime,WHITE,5,15);
  
}
void clearGPS() {  //Since between GPS reads, we still have data streaming in, we need to clear the old data by reading a few sentences, and discarding these
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());

}


void testdrawtext(char *text, uint16_t color, uint8_t set_x,uint8_t set_y) {
  tft.setCursor(set_x,set_y);
  tft.setTextColor(color);
  tft.print(text);
  
}
