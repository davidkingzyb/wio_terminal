#include "Display.h"
#include "Util.h"
#include "Menu.h"
#include <Wire.h>

#define CARDKB_ADDR 0x5F


void setup() {
    Serial1.begin(115200);
    Serial.begin(115200);
    Wire.begin();

    pinMode(5, INPUT);//keyboard wire

    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);

    Display::init(); 
    Util::initSDCard();
}

String tty_string="";

void loop() {
    //rasberry pi serial1
    while(Serial1.available()){
        tty_string=tty_string+char(Serial1.read());
        //delay(1);
    }

    if(tty_string.length()>0){
        Serial.print(tty_string);
        Util::record(tty_string);
        tty_string="";
    }

    // keyboard wire
    Wire.requestFrom(CARDKB_ADDR, 1);
    while(Wire.available()){
        char c = Wire.read(); // receive a byte as characterif
        if (c != 0){
            if((int)c==0x80){//fn+esc ^C
                //Serial.print("^C");
                c=(char)0x03;
            }else if((int)c==0x8b){
                c=(char)0x04;
            }
            //Serial.print(c, DEC);
            //Serial.println(c);
            Serial1.write(c);
        }
    }

    if(digitalRead(WIO_KEY_A) == LOW) {
        Menu::menuKeyboard();
        delay(200);
    }else if (digitalRead(WIO_KEY_B) == LOW) {
        Menu::menuScripts();
        delay(200);
    }else if (digitalRead(WIO_KEY_C) == LOW) {
        Menu::menuMenus();
        delay(200);
    }else if (digitalRead(WIO_5S_UP) == LOW) {
        Menu::doUp();
        delay(100);
    }else if (digitalRead(WIO_5S_DOWN) == LOW) {
        Menu::doDown();
        delay(100);
    }else if (digitalRead(WIO_5S_LEFT) == LOW) {
        Menu::doLeft();
        delay(100);
    }else if (digitalRead(WIO_5S_RIGHT) == LOW) {
        Menu::doRight();
        delay(100);
    }else if (digitalRead(WIO_5S_PRESS) == LOW) {
        Menu::doConfirm();
        delay(200);
    }


    //delay(10);//for server loop    
}
