//Rhys Grover
//Grade 10 Electronics Culminating
//Supersonic Alarm System + Extras
//Format your sd card to Fat32
//Search up The mins for adafruit sd card reader for arduino mega and use them the others will not work
//Speaker requieres a amp
#include "SD.h"
#define SD_ChipSelectPin 53
#include "TMRpcm.h"
#include "SPI.h"
#define echoPin 31
#define trigPin 33
#define Speaker 46
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define SD_ChipSelectPin 53
#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 2

CRGB leds[NUM_LEDS];

TMRpcm audio;

LiquidCrystal_I2C lcd(0x27, 20, 4);
char keypressed;
const byte ROWS = 4;
const byte COLS = 4;
String password = "1234";
String tempPassword;
String tempPassword2;
String tempMusiccode;
boolean alarmon = false;
boolean alarmprimed = false;
boolean menumenu = true;
boolean passChangeMode = false;
boolean passChanged = false;
boolean MusiccodeB = false;
boolean MusiccodeA = false;
int k = 8;
int dis = 0;
int firstfewC = 22;

char keyMap[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {40, 42, 44, 48};
byte colPins[COLS] = {43, 41, 45, 47};

Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);

TMRpcm tmrpcm; // create an object for use in this sketch


void setup() {

  Serial.begin(9600);
  tmrpcm.speakerPin = 46;

  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(20);

  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("SD fail");
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 255, 255);
    FastLED.show();
  }


  pinMode(10, OUTPUT);
  pinMode(Speaker, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  tmrpcm.setVolume(6);
}


void loop() {

  int distance = msToCm( getDistance() );
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println("-----");
  Serial.print(dis);
  Serial.println(" ...cm");
  Serial.println("-----");

  if (alarmprimed == true)
  {
    int firstfew = 1000;
    if(firstfewC == 1){
      firstfew = 0;
    }else{
      firstfewC -= 1;
    }
    if ((distance + firstfew) < dis)
    {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 255, 0);
        FastLED.show();
        delay(2);
      }
      alarmon = true;
      tone(Speaker, 1000);
      enterPassword();
      alarmprimed = false;
    }
  }


  if (menumenu == true) {
    lcd.setCursor(0, 0);
    lcd.print("A - To Prime Alarm");
    lcd.setCursor(0, 1);
    lcd.print("B - To Set Code");
    lcd.setCursor(0, 2);
    lcd.print("C - To Play Music");
    lcd.setCursor(0,3);
    lcd.print("---By Rhys Grover---");
    keypressed = myKeypad.getKey();

    if (keypressed == 'A') {
      int i = 9;
      int hhh = 2000;
      audio.stopPlayback();
      menumenu = false;
      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("!Priming!");
      lcd.setCursor(9, 2);
      lcd.print("10");
      tone(Speaker, hhh, 100);
      hhh -= 100;
      delay(1000);
      lcd.clear();
      lcd.setCursor(5, 1);
      lcd.print("!Priming!");
      while (i == 2 || i == 3 || i == 4|| i == 5 || i == 6 || i == 7 || i == 8 || i == 9){
        int distance = msToCm( getDistance() );
        Serial.print(distance);
        Serial.println(" cm");
        Serial.println("-----");
        lcd.setCursor(9, 2);
        lcd.print(i);
        tone(Speaker, hhh, 100);
        delay(1000);
        dis = dis + distance;
        i--;
        hhh -= 100;
        if (i == 1) {
          alarmprimed = true;
          dis = dis / 8;
          dis -= 10;
          tone(Speaker, hhh, 500);
          lcd.clear();
          lcd.setCursor(6, 1);
          lcd.print("!PRIMED!");
          lcd.setCursor(6, 2);
          lcd.print("!PRIMED!");
          lcd.setCursor(0, 3);
          lcd.print("Distance At Cm> ");
          lcd.setCursor(16, 3);
          lcd.print(dis);
          delay(5000);
        }
      }
    }
    if (keypressed == 'B') {
      Changecode();
    }

    if(keypressed == 'C'){
      MusiccodeB = true;
      Musiccode();
    }
  }
}

long getDistance() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration;
}


long msToCm(long microseconds) {
  return microseconds / 29 / 2;
}

void enterPassword() {
  k = 8;
  tempPassword2 = "";
  alarmon = true;
  tone(Speaker, 1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("**ALARM** **ALARM**");
  lcd.setCursor(3, 1);
  lcd.print("ENTER THE CODE");
  lcd.setCursor(0, 3);
  lcd.print("**ALARM** **ALARM**");
  lcd.setCursor(3, 2);
  lcd.print("Pass>");
  while (alarmprimed) {
   // int distance = msToCm( getDistance() );
   // Serial.print(distance);
   // Serial.println(" cm");
   // Serial.println("-----");
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY) {
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9' ) {
        tempPassword2 += keypressed;
        lcd.setCursor(k, 2);
        lcd.print("*");
        k++;
      }
    }
    if (k > 12 || keypressed == '#') {
      tempPassword2 = "";
      k = 8;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("**ALARM** **ALARM**");
      lcd.setCursor(3, 1);
      lcd.print("ENTER THE CODE");
      lcd.setCursor(0, 3);
      lcd.print("**ALARM** **ALARM**");
      lcd.setCursor(3, 2);
      lcd.print("Pass>");
    }
    if ( keypressed == '*') {
      if ( tempPassword2 == password ) {
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(" !CORRECT!!CORRECT!");
        lcd.setCursor(2, 2);
        lcd.print("SYSTEMS DISABLED");
        alarmon = false;
        alarmprimed = false;
        firstfewC = 22;
        int dis = 0;
        noTone(Speaker);
        for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(255, 255, 255);
        FastLED.show();
        delay(34);
      }
        menumenu = true;
        lcd.clear();
      }
      else if (tempPassword2 != password) {
        lcd.setCursor(3, 2);
        lcd.print("Wrong! Try Again");
        delay(1000);
        tempPassword2 = "";
        k = 8;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("**ALARM** **ALARM**");
        lcd.setCursor(3, 1);
        lcd.print("ENTER THE CODE");
        lcd.setCursor(0, 3);
        lcd.print("**ALARM** **ALARM**");
        lcd.setCursor(3, 2);
        lcd.print("Pass>");
        lcd.setCursor(8, 2);
      }
    }
  }
}

void Changecode() {
  lcd.clear();
  int h = 1;
  tone(Speaker, 2000, 100);
  tempPassword = "";
  lcd.setCursor(0, 0);
  lcd.print("Current Password");
  lcd.setCursor(0, 1);
  lcd.print(">");
  passChangeMode = true;
  passChanged = true;
  while (passChanged == true) {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY) {
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9' ) {
        tempPassword += keypressed;
        lcd.setCursor(h, 1);
        lcd.print("*");
        h++;
        tone(Speaker, 2000, 100);
      }
    }
    if (h > 5 || keypressed == '#') {
      tempPassword = "";
      h = 1;
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("Current Password");
      lcd.setCursor(7, 2);
      lcd.print(">");
      lcd.setCursor(13, 2);
      lcd.print("<");
    }
    if ( keypressed == '*') {
      h = 1;
      tone(Speaker, 2000, 100);
      if (password == tempPassword) {
        tempPassword = "";
        lcd.clear();
        lcd.clear();
        lcd.setCursor(3, 1);
        lcd.print("Set New Password");
        lcd.setCursor(7, 2);
        lcd.print(">");
        lcd.setCursor(13, 2);
        lcd.print("<");
        while (passChangeMode == true) {
          keypressed = myKeypad.getKey();
          if (keypressed != NO_KEY) {
            if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' || keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9' ) {
              tempPassword += keypressed;
              lcd.setCursor(h, 1);
              lcd.print("*");
              h++;
              tone(Speaker, 2000, 100);
            }
          }
          if (h > 5 || keypressed == '#') {
            tempPassword = "";
            h = 1;
            tone(Speaker, 2000, 100);
            lcd.clear();
            lcd.clear();
            lcd.setCursor(3, 1);
            lcd.print("Set New Password");
            lcd.setCursor(7, 2);
            lcd.print(">");
            lcd.setCursor(13, 2);
            lcd.print("<");
          }
          if ( keypressed == '*') {
            passChangeMode = false;
            passChanged = false;
            h = 1;
            tone(Speaker, 2000, 100);
            password = tempPassword;
            lcd.clear();
          }
        }
      }
    }
  }
}

void Musiccode() {
        int yy = 1;
        tempMusiccode = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Song Code");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.setCursor(0,2);
        lcd.print("Code's Can Be Found>");
        lcd.setCursor(0,3);
        lcd.print("Rhys-Grover's Github");
        while (MusiccodeB == true) {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY) {
      if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' || keypressed == '8' || keypressed == '9' ) {
        tempMusiccode += keypressed;
        lcd.setCursor(yy, 1);
        lcd.print(keypressed);
        yy++;
      }
    }
     if ( keypressed == '*') {       //Runaway
       if(tempMusiccode == "4215"){
         lcd.clear();
         lcd.setCursor(4, 0);
         lcd.print("Now Playing!");
         lcd.setCursor(6, 1);
         lcd.print("Runaway");
         lcd.setCursor(5,2);
         lcd.print("Kanye West");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Runaway.wav");
         MusiccodeA = true;
         MusiccodeB = false;
    }
        // Dark Fantasy
       if(tempMusiccode == "3752"){
         lcd.clear();
         lcd.setCursor(4, 0);
         lcd.print("Now Playing!");
         lcd.setCursor(4, 1);
         lcd.print("Dark Fantasy");
         lcd.setCursor(5,2);
         lcd.print("Kanye West");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("DF.wav");
         MusiccodeA = true;
         MusiccodeB = false;
     }
      //on sight
       if(tempMusiccode == "1547"){
         lcd.clear();
         lcd.setCursor(4, 0);
         lcd.print("Now Playing!");
         lcd.setCursor(7, 1);
         lcd.print("Power");
         lcd.setCursor(5,2);
         lcd.print("Kanye West");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Power.wav");
         MusiccodeA = true;
         MusiccodeB = false;
     }
        // On Sight
       if(tempMusiccode == "9754"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(5, 1);
        lcd.print("On Sight");
         lcd.setCursor(5,2);
         lcd.print("Kanye West");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Onsight.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }

       // Off the Grid
       if(tempMusiccode == "5414"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(4, 1);
        lcd.print("Off The Grid");
         lcd.setCursor(5,2);
         lcd.print("Kanye West");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Offthegrid.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }

       // Waves
       if(tempMusiccode == "9881"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(7, 1);
        lcd.print("Waves");
         lcd.setCursor(5,2);
         lcd.print("Kanye West");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Waves.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }

     // Jigsaw Falling Into Place
       if(tempMusiccode == "1411"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(0, 1);
        lcd.print("Jigsaw Falling>Place");
         lcd.setCursor(5,2);
         lcd.print("Radiohead");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Jigsawfallin.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }

    // Judebox Joints
       if(tempMusiccode == "1566"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(3, 1);
        lcd.print("Judebox Joints");
         lcd.setCursor(5,2);
         lcd.print("Asap Rocky");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Judeboxjoints.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }

    // Sickomode
       if(tempMusiccode == "3733"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(5, 1);
        lcd.print("Sickomode");
         lcd.setCursor(4,2);
         lcd.print("Travis Scott");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Sickomode.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }

    // Radioactive
       if(tempMusiccode == "1122"){
        lcd.clear();
         lcd.setCursor(4, 0);
        lcd.print("Now Playing!");
         lcd.setCursor(4, 1);
        lcd.print("Radioactive");
         lcd.setCursor(3,2);
         lcd.print("Imagine Dragons");
          lcd.setCursor(1,3);
          lcd.print("Return to Menu - #");
         tmrpcm.setVolume(6);
         tmrpcm.play("Radioactive.wav");
         MusiccodeA = true;
        MusiccodeB = false;
    }
   
     }
   
    if (yy > 5 || keypressed == '#') {
      tempMusiccode = "";
      yy = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Song Code");
        lcd.setCursor(0, 1);
        lcd.print(">");
        lcd.setCursor(0,2);
        lcd.print("Code's Can Be Found>");
        lcd.setCursor(0,3);
        lcd.print("Rhys-Grover's Github");
        lcd.setCursor(1, 1);
    }
     }
     while(MusiccodeA == true){
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY) {
      if (keypressed == '#') {
         audio.stopPlayback();
         MusiccodeA = false;
         MusiccodeB = false;
         menumenu = true;
         lcd.clear();
      }
     }
}
}
