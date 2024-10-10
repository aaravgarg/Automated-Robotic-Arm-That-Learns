   ////////////////////////////////////////////////////////
  //                Arduino Robotic Arm                 //             
 //              By Aarav Garg - Tech Nuttiez          //
////////////////////////////////////////////////////////

#include <Servo.h> 

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

//pressure for tft touch display
#define MINPRESSURE 200
#define MAXPRESSURE 1000

//page variable
String page;

//clicked state
boolean cs1 = false; 
boolean cs2 = false; 
boolean cs3 = false; 
boolean cs4 = false; 
boolean cs5 = false; 

//saved sequence number
int ssn = 0;

//init tft object
MCUFRIEND_kbv tft;

//define the servos
Servo s1;
Servo s2;
Servo s3;
Servo s4;

//joy pins
#define j1x A8
#define j1y A9
#define j2x A10
#define j2y A11

//joy values
int x_out = 0;
int y_out = 0;
int x_out2 = 0;
int y_out2 = 0;

//define variable for values of the button
int button1Pressed = 0;
boolean button2Pressed = false;
boolean mode = false;

//define int variable array for saved position of the servos
int servo1PosSave[]={90,90,90,90,90};
int servo2PosSave[]={90,90,90,90,90};
int servo3PosSave[]={90,90,90,90,90};
int servo4PosSave[]={90,90,90,90,90};

//tft callibration data
const int XP = 6, XM = A2, YP = A1, YM = 7; //240x320 ID = 0x9325
const int TS_LEFT = 142, TS_RT = 892, TS_TOP = 937, TS_BOT = 187;

//init touchscreen object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//init tft display gui buttons
Adafruit_GFX_Button manual_btn, auto_btn, backmanual_btn, backauto_btn, backrecord_btn, backplay_btn, record_btn, play_btn, save_btn;

int pixel_x, pixel_y;     //Touch_getXY() updates global vars
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.y, TS_TOP, TS_BOT, 0, 320); 
        pixel_y = map(p.x, TS_RT, TS_LEFT, 0, 240);
    }
    return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup() {
  //define the servo pins
  s1.attach(22);
  s2.attach(24);
  s3.attach(26);
  s4.attach(28);

  s1.write(90);
  s2.write(90);
  s3.write(90);
  s4.write(90);

  //define joy pins as input units
  pinMode(j1x, INPUT);
  pinMode(j1y, INPUT);
  pinMode(j2x, INPUT);
  pinMode(j2y, INPUT);

  //initialize serial communication
  Serial.begin(9600);

  uint16_t ID = tft.readID();
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);
  Serial.println("Calibrate for your Touch Panel");
  
  tft.begin(ID);
  tft.setRotation(1);            //LANDSCAPE - 1; PORTRAIT - 0
  tft.fillScreen(BLACK); 

  page = "home";

}

void loop(){

  if (page == "home"){
    chooseModePage();
  } else if (page == "manual"){
    manualModePage();
  } else if (page == "automated"){
    automatedModePage(); 
  } else if (page == "record"){
    recordPage();
  } else if (page == "play"){
    playPage();
  }
    
}

void chooseModePage(){

  cs2 = false;
  cs3 = false;
  cs4 = false;
  cs5 = false;
  ssn = 0;

  if(cs1 == false){
    tft.fillScreen(BLACK);
    manual_btn.initButton(&tft,  160, 120, 250, 55, WHITE, CYAN, BLACK, "MANUAL", 3);
    auto_btn.initButton(&tft, 160, 190, 250, 55, WHITE, CYAN, BLACK, "AUTOMATED", 3);  
    manual_btn.drawButton(false);
    auto_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(43, 55);                    //x,y
    tft.setTextColor(WHITE);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Choose Mode");                 //Your Text
    cs1 = true;
  }

  bool down = Touch_getXY();
    manual_btn.press(down && manual_btn.contains(pixel_x, pixel_y));
    auto_btn.press(down && auto_btn.contains(pixel_x, pixel_y));
    if (manual_btn.justReleased()){
        tft.setFont();
        manual_btn.drawButton(false);
        Serial.println("a");
        page = "manual";
    }        
    if (auto_btn.justReleased()){
        tft.setFont();
        auto_btn.drawButton(false);
        Serial.println("b");
        page = "automated";
    }
    if (manual_btn.justPressed()) {
        tft.setFont();
        manual_btn.drawButton(true);
        Serial.println("c");
    }
    if (auto_btn.justPressed()) {
        tft.setFont();
        auto_btn.drawButton(true);
        Serial.println("d");
    }  

}
  
void manualModePage(){

  Serial.print(analogRead(j1x));
  Serial.print("\t");
  Serial.print(analogRead(j2x));
  Serial.print("\t");
  Serial.print(analogRead(j1y));
  Serial.print("\t");
  Serial.println(analogRead(j2y));

  cs1 = false;
  cs3 = false;
  cs4 = false;
  cs5 = false;
  ssn = 0;

  if (cs2 == false){
    tft.fillScreen(BLACK);
    backmanual_btn.initButton(&tft,  160, 190, 250, 55, WHITE, CYAN, BLACK, "BACK", 3);  
    backmanual_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(50, 50);                    //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Manual Mode");                 //Your Text
    tft.setFont(&FreeSansBold12pt7b);
    tft.setCursor(20, 100);                    //x,y
    tft.setTextColor(WHITE);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Use the Joystick here to");                 //Your Text
    tft.setFont(&FreeSansBold12pt7b);
    tft.setCursor(20, 130);                    //x,y
    tft.setTextColor(WHITE);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("control the Robotic Arm!"); 
    cs2 = true;
  }

  bool down = Touch_getXY();
  backmanual_btn.press(down && backmanual_btn.contains(pixel_x, pixel_y));
  if (backmanual_btn.justReleased()){
      tft.setFont();
      backmanual_btn.drawButton(false);
      Serial.println("a");
      page = "home";
  }        
  if (backmanual_btn.justPressed()) {
      tft.setFont();
      backmanual_btn.drawButton(true);
      Serial.println("c");
  }

  //-------------------------------Manual Joystick Control-------------------------------------------------------------------------------------------------------------

  if(analogRead(j1x) >= 800){
     for (int i = s1.read(); i < 180; i++){
      if (analogRead(j1x) < 800){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }

  else if(analogRead(j1x) <= 300){
     for (int i = s1.read(); i > 0; i--){
      if (analogRead(j1x) > 300){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j1y) >= 800){
     for (int i = s2.read(); i < 140; i++){
      if (analogRead(j1y) < 800){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }

  else if(analogRead(j1y) <= 300){
     for (int i = s2.read(); i > 70; i--){
      if (analogRead(j1y) > 300){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j2x) >= 800){
     for (int i = s4.read(); i < 120; i++){
      if (analogRead(j2x) < 800){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }

  else if(analogRead(j2x) <= 300){
     for (int i = s4.read(); i > 50; i--){
      if (analogRead(j2x) > 300){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }
//-------------------------------------------
  if(analogRead(j2y) >= 800){
     for (int i = s3.read(); i < 120; i++){
      if (analogRead(j2y) < 800){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }

  else if(analogRead(j2y) <= 300){
     for (int i = s3.read(); i > 60; i--){
      if (analogRead(j2y) > 300){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }
  
}

void automatedModePage(){

  cs1 = false;
  cs2 = false;
  cs4 = false;
  cs5 = false;
  ssn = 0;

  if (cs3 == false){
    tft.fillScreen(BLACK);
    backauto_btn.initButton(&tft,  160, 180, 280, 55, WHITE, CYAN, BLACK, "BACK", 3);  
    backauto_btn.drawButton(false);
    record_btn.initButton(&tft,  103, 115, 162, 55, WHITE, CYAN, BLACK, "RECORD", 3);  
    record_btn.drawButton(false);
    play_btn.initButton(&tft,  245, 115, 110, 55, WHITE, CYAN, BLACK, "PLAY", 3);  
    play_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(70, 60);                    //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Auto Mode");                 //Your Text
    cs3 = true;
  }  

  bool down = Touch_getXY();
    record_btn.press(down && record_btn.contains(pixel_x, pixel_y));
    play_btn.press(down && play_btn.contains(pixel_x, pixel_y));
    backauto_btn.press(down && backauto_btn.contains(pixel_x, pixel_y));
    if (record_btn.justReleased()){
        tft.setFont();
        record_btn.drawButton(false);
        page = "record";
    }        
    if (play_btn.justReleased()){
        tft.setFont();
        play_btn.drawButton(false);
        page = "play";
    }
    if (backauto_btn.justReleased()){
        tft.setFont();
        backauto_btn.drawButton(false);
        page = "home";
    }
    if (record_btn.justPressed()) {
        tft.setFont();
        record_btn.drawButton(true);
    }
    if (play_btn.justPressed()) {
        tft.setFont();
        play_btn.drawButton(true);
    }  
    if (backauto_btn.justPressed()) {
        tft.setFont();
        backauto_btn.drawButton(true);
    }  
}

void recordPage(){

  cs1 = false;
  cs2 = false;
  cs3 = false;
  cs5 = false;

  if (cs4 == false){
    tft.fillScreen(BLACK);
    backrecord_btn.initButton(&tft,  160, 205, 260, 55, GREEN, CYAN, BLACK, "BACK", 3);  
    backrecord_btn.drawButton(false);
    save_btn.initButton(&tft,  160, 140, 260, 55, CYAN, GREEN, BLACK, "SAVE", 3);  
    save_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(67, 50);                    //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Recording..."); 
    tft.fillRect(35, 70, 250, 30, WHITE);
    tft.drawRect(35, 70, 50, 30, BLUE);  
    tft.drawRect(85, 70, 50, 30, BLUE); 
    tft.drawRect(135, 70, 50, 30, BLUE); 
    tft.drawRect(185, 70, 50, 30, BLUE); 
    tft.drawRect(235, 70, 50, 30, BLUE); 
    cs4 = true;
  }

  bool down = Touch_getXY();
    backrecord_btn.press(down && backrecord_btn.contains(pixel_x, pixel_y));
    save_btn.press(down && save_btn.contains(pixel_x, pixel_y));
    if (backrecord_btn.justReleased()){
        tft.setFont();
        backrecord_btn.drawButton(false);
        Serial.println("a");
        page = "automated";
    }        
    if (save_btn.justReleased()){
        tft.setFont();
        save_btn.drawButton(false);
        Serial.println("b");
        switch(ssn){
          case 0:
            servo1PosSave[0] = s1.read();
            servo2PosSave[0] = s2.read();
            servo3PosSave[0] = s3.read();
            servo4PosSave[0] = s4.read();
            tft.fillRect(35, 70, 50, 30, YELLOW);
            break;
          case 1:
            servo1PosSave[1] = s1.read();
            servo2PosSave[1] = s2.read();
            servo3PosSave[1] = s3.read();
            servo4PosSave[1] = s4.read();
            tft.fillRect(85, 70, 50, 30, YELLOW);
            break;
          case 2:
            servo1PosSave[2] = s1.read();
            servo2PosSave[2] = s2.read();
            servo3PosSave[2] = s3.read();
            servo4PosSave[2] = s4.read();
            tft.fillRect(135, 70, 50, 30, YELLOW);
           break;
          case 3:
            servo1PosSave[3] = s1.read();
            servo2PosSave[3] = s2.read();
            servo3PosSave[3] = s3.read();
            servo4PosSave[3] = s4.read();
            tft.fillRect(185, 70, 50, 30, YELLOW);
            break;
          case 4:
            servo1PosSave[4] = s1.read();
            servo2PosSave[4] = s2.read();
            servo3PosSave[4] = s3.read();
            servo4PosSave[4] = s4.read();
            tft.fillRect(235, 70, 50, 30, YELLOW);
            break;
        }
        ssn++;
    }
    if (backrecord_btn.justPressed()) {
        tft.setFont();
        backrecord_btn.drawButton(true);
        Serial.println("c");
    }
    if (save_btn.justPressed()) {
        tft.setFont();
        save_btn.drawButton(true);
        Serial.println("d");
    }

   //-------------------------------Manual Joystick Control-------------------------------------------------------------------------------------------------------------

  if(analogRead(j1x) >= 800){
     for (int i = s1.read(); i < 180; i++){
      if (analogRead(j1x) < 800){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }

  if(analogRead(j1x) <= 300){
     for (int i = s1.read(); i > 0; i--){
      if (analogRead(j1x) > 300){
        break;
      }
      s1.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j1y) >= 800){
     for (int i = s2.read(); i < 140; i++){
      if (analogRead(j1y) < 800){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }

  if(analogRead(j1y) <= 300){
     for (int i = s2.read(); i > 70; i--){
      if (analogRead(j1y) > 300){
        break;
      }
      s2.write(i);
      delay(20);
     }
  }
//-------------------------------------------
  if(analogRead(j2x) >= 800){
     for (int i = s4.read(); i < 120; i++){
      if (analogRead(j2x) < 800){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }

  if(analogRead(j2x) <= 300){
     for (int i = s4.read(); i > 50; i--){
      if (analogRead(j2x) > 300){
        break;
      }
      s4.write(i);
      delay(30);
     }
  }
//-------------------------------------------
  if(analogRead(j2y) >= 800){
     for (int i = s3.read(); i < 120; i++){
      if (analogRead(j2y) < 800){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }

  if(analogRead(j2y) <= 300){
     for (int i = s3.read(); i > 60; i--){
      if (analogRead(j2y) > 300){
        break;
      }
      s3.write(i);
      delay(20);
     }
  }
}

void playPage(){

  cs1 = false;
  cs2 = false;
  cs3 = false;
  cs4 = false;
  ssn = 0;

  if (cs5 == false){
    tft.fillScreen(BLACK);
    backplay_btn.initButton(&tft,  160, 190, 250, 55, WHITE, CYAN, BLACK, "BACK", 3);  
    backplay_btn.drawButton(false);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setCursor(85, 50);                    //x,y
    tft.setTextColor(CYAN);                  //color
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.print("Playing...");                 //Your Text
    tft.setFont(&FreeSansBold12pt7b);
    tft.setTextSize(1);                       //Eg., 1,2,3
    tft.setCursor(30, 100);
    tft.setTextColor(WHITE);
    tft.print("Total Positions: 5");
    cs5 = true;
  }

  bool down = Touch_getXY();
  backplay_btn.press(down && backplay_btn.contains(pixel_x, pixel_y));
  if (backplay_btn.justReleased()){
      tft.setFont();
      backplay_btn.drawButton(false);
      Serial.println("a");
      page = "automated";
  }        
  if (backplay_btn.justPressed()) {
      tft.setFont();
      backplay_btn.drawButton(true);
      Serial.println("c");
  }

  
  for (int i = 0; i < 5; i++){

    tft.fillRect(0, 110, 320, 40, BLACK);
    tft.setCursor(30, 130);                  
    tft.setTextSize(1);                     
    String a = "Current Position: ";
    int j = i + 1;
    String b = a + j;
    tft.print(b);  

    bool down = Touch_getXY();
    backplay_btn.press(down && backplay_btn.contains(pixel_x, pixel_y));
    if (backplay_btn.justReleased()){
        tft.setFont();
        backplay_btn.drawButton(false);
        Serial.println("a");
        page = "automated";
    }        
    
    if (backplay_btn.justPressed()) {
        tft.setFont();
        backplay_btn.drawButton(true);
        Serial.println("c");
    }
    
    if (servo1PosSave[i] < s1.read()){
      for (int p = s1.read(); p >= servo1PosSave[i]; p--){
        s1.write(p);
        delay(20); 
      }
    }

    if (servo1PosSave[i] > s1.read()){
      for (int p = s1.read(); p <= servo1PosSave[i]; p++){
        s1.write(p);
        delay(20); 
      }
    }

    if (servo2PosSave[i] < s2.read()){
      for (int p = s2.read(); p >= servo2PosSave[i]; p--){
        s2.write(p);
        delay(20); 
      }
    }

    if (servo2PosSave[i] > s2.read()){
      for (int p = s2.read(); p <= servo2PosSave[i]; p++){
        s2.write(p);
        delay(20); 
      }
    }
  
    if (servo3PosSave[i] < s3.read()){
      for (int p = s3.read(); p >= servo3PosSave[i]; p--){
        s3.write(p);
        delay(20); 
      }
    }

    if (servo3PosSave[i] > s3.read()){
      for (int p = s3.read(); p <= servo3PosSave[i]; p++){
        s3.write(p);
        delay(20); 
      }
    }

    if (servo4PosSave[i] < s4.read()){
      for (int p = s4.read(); p >= servo4PosSave[i]; p--){
        s4.write(p);
        delay(20); 
      }
    }

    if (servo4PosSave[i] > s4.read()){
      for (int p = s4.read(); p <= servo4PosSave[i]; p++){
        s4.write(p);
        delay(20); 
      }
    }
  }
}

/*void loop() {
  if(digitalRead(btnmode) == HIGH){
    switch(mode){
      case false:
       mode = true;
       break;

      case true: 
       mode = false;
       break;
    }
   }

   else{
    
   switch(mode){
    case false:
      lcd.clear();
      lcd.print("Manual Mode");
      
      //---JOY-1-X-AXIS-UP---
    if(analogRead(j1x) >= 800){
     for(int i = s2.read(); i < 120; i++){
      x_out = analogRead(j1x);
      if(x_out < 800){
        break;
      }
      s2.write(i);
      delay(20);
     }
    }
    //---JOY-1-X-AXIS-DOWN---
    else if(analogRead(j1x) <= 200){
     for(int i = s2.read(); i > 30; i--){
      x_out = analogRead(j1x);
      if(x_out > 200){
        break;
      }
      s2.write(i);
      delay(20);
     }
    }
    //---JOY-1-Y-AXIS-UP---
    if(analogRead(j1y) >= 800){
     for(int i = s1.read(); i < 150; i++){
      y_out = analogRead(j1y);
      if(y_out < 800){
        break;
      }
      s1.write(i);
      delay(20);
     }
    }
    //---JOY-1-Y-AXIS-DOWN---
    else if(analogRead(j1y) <= 200){
     for(int i = s1.read(); i > 30; i--){
      y_out = analogRead(j1y);
      if(y_out > 200){
        break;
      }
      s1.write(i);
      delay(20);
     }
    }
    //---JOY-2-X-AXIS-UP---
    if(analogRead(j2x) >= 800){
     for(int i = s3.read(); i < 150; i++){
      x_out2 = analogRead(j2x);
      if(x_out2 < 800){
        break;
      }
      s3.write(i);
      delay(20);
     }
    }
    //---JOY-2-X-AXIS-DOWN---
    else if(analogRead(j2x) <= 200){
     for(int i = s3.read(); i > 60; i--){
      x_out2 = analogRead(j2x);
      if(x_out2 > 200){
        break;
      }
      s3.write(i);
      delay(20);
     }
    }
    //---JOY-2-Y-AXIS-UP---
    if(analogRead(j2y) >= 800){
     for(int i = s4.read(); i < 180; i++){
      y_out2 = analogRead(j2y);
      if(y_out2 < 800){
        break;
      }
      s4.write(i);
      delay(20);
     }
    }
    //---JOY-2-Y-AXIS-DOWN---
    else if(analogRead(j2y) <= 200){
     for(int i = s4.read(); i > 75; i--){
      y_out2 = analogRead(j2y);
      if(y_out2 > 200){
        break;
      }
      s4.write(i);
      delay(20);
     }
    }
      break;
  
   case true:
      lcd.clear();
      lcd.print("Automated Mode");
    //if button2 is pressed, set the mode to playing
  if(digitalRead(button2) == HIGH){
    button2Pressed = true;
  }
  //if button3 is pressed, go back to recording
  if(digitalRead(button3) == HIGH){
    button1Pressed = 0;
    button2Pressed = false;
  }
  //--------------------------------------------------PLAYING MODE--------------------------------------------------
  if(button2Pressed){
    Serial.print("Playing...");
    lcd.clear();
    lcd.print("Playing...");
    lcd.setCursor(0,1);
    lcd.print("Automated Mode");
    for(int i = 0; i < 5; i++){
      if(servo3PosSave[i] < s3.read()){
        for(int a = s3.read(); a > servo3PosSave[i]; i--){
          s3.write(a);
        }
      }
      else if(servo3PosSave[i] > s3.read()){
        for(int a = s3.read(); a < servo3PosSave[i]; i++){
          s3.write(a);
        }
      }
      s2.write(servo2PosSave[i]);
      delay(500);      
      s1.write(servo1PosSave[i]);
      delay(500);      
      s4.write(servo4PosSave[i]);
      delay(2000);
    }
  }
  //--------------------------------------------------RECORDING MODE--------------------------------------------------
  else if(button2Pressed == false){
    Serial.println("Recording...");
    lcd.clear();
    lcd.print("Recording...");
    lcd.setCursor(0,1);
    lcd.print("Automated Mode");
    //---JOY-1-X-AXIS-UP---
    if(analogRead(j1x) >= 800){
     for(int i = s2.read(); i < 120; i++){
      x_out = analogRead(j1x);
      if(x_out < 800){
        break;
      }
      s2.write(i);
      delay(20);
     }
    }
    //---JOY-1-X-AXIS-DOWN---
    else if(analogRead(j1x) <= 200){
     for(int i = s2.read(); i > 30; i--){
      x_out = analogRead(j1x);
      if(x_out > 200){
        break;
      }
      s2.write(i);
      delay(20);
     }
    }
    //---JOY-1-Y-AXIS-UP---
    if(analogRead(j1y) >= 800){
     for(int i = s1.read(); i < 180; i++){
      y_out = analogRead(j1y);
      if(y_out < 800){
        break;
      }
      s1.write(i);
      delay(20);
     }
    }
    //---JOY-1-Y-AXIS-DOWN---
    else if(analogRead(j1y) <= 200){
     for(int i = s1.read(); i > 0; i--){
      y_out = analogRead(j1y);
      if(y_out > 200){
        break;
      }
      s1.write(i);
      delay(20);
     }
    }
    //---JOY-2-X-AXIS-UP---
    if(analogRead(j2x) >= 800){
     for(int i = s3.read(); i < 150; i++){
      x_out2 = analogRead(j2x);
      if(x_out2 < 800){
        break;
      }
      s3.write(i);
      delay(20);
     }
    }
    //---JOY-2-X-AXIS-DOWN---
    else if(analogRead(j2x) <= 200){
     for(int i = s3.read(); i > 60; i--){
      x_out2 = analogRead(j2x);
      if(x_out2 > 200){
        break;
      }
      s3.write(i);
      delay(20);
     }
    }
    //---JOY-2-Y-AXIS-UP---
    if(analogRead(j2y) >= 800){
     for(int i = s4.read(); i < 180; i++){
      y_out2 = analogRead(j2y);
      if(y_out2 < 800){
        break;
      }
      s4.write(i);
      delay(20);
     }
    }
    //---JOY-2-Y-AXIS-DOWN---
    else if(analogRead(j2y) <= 200){
     for(int i = s4.read(); i > 75; i--){
      y_out2 = analogRead(j2y);
      if(y_out2 > 200){
        break;
      }
      s4.write(i);
      delay(20);
     }
    }
  //--------SAVING THE SEQUENCE---------  
  if(digitalRead(button1) == HIGH){
    button1Pressed++;
    switch(button1Pressed){
      case 1:
        servo1PosSave[0] = s1.read();
        servo2PosSave[0] = s2.read();
        servo3PosSave[0] = s3.read();
        servo4PosSave[0] = s4.read();
        lcd.home();
        Serial.println("Position #1 Saved");
        lcd.print("Position 1 Saved");
       break;
       case 2:
        servo1PosSave[1] = s1.read();
        servo2PosSave[1] = s2.read();
        servo3PosSave[1] = s3.read();
        servo4PosSave[1] = s4.read();
        lcd.home();
        Serial.println("Position #2 Saved");
        lcd.print("Position 2 Saved");
       break;
       case 3:
        servo1PosSave[2] = s1.read();
        servo2PosSave[2] = s2.read();
        servo3PosSave[2] = s3.read();
        servo4PosSave[2] = s4.read();
        lcd.home();
        Serial.println("Position #3 Saved");
        lcd.print("Position 3 Saved");
       break;
       case 4:
        servo1PosSave[3] = s1.read();
        servo2PosSave[3] = s2.read();
        servo3PosSave[3] = s3.read();
        servo4PosSave[3] = s4.read();
        lcd.home();
        Serial.println("Position #4 Saved");
        lcd.print("Position 4 Saved");
       break;
       case 5:
        servo1PosSave[4] = s1.read();
        servo2PosSave[4] = s2.read();
        servo3PosSave[4] = s3.read();
        servo4PosSave[4] = s4.read();
        lcd.home();
        Serial.println("Position #5 Saved");
        lcd.print("Position 5 Saved");
       break;
    }
  }
  }
  delay(500);
  break;
  }
  
}
}*/
