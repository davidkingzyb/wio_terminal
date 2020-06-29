#include "TFT_eSPI.h"
#include "Display.h"

/*
320 x 240
        50      100      150              300   
 ----------------------------------------------
| showC | showB | showA | showStatus       | * |                       
 -----------------------------------------------15
                | 1  2  3  4  5  6  7  8  9  0 |   
                |------------------------------|35      
                | q  w  e  r  t  y  u  i  o  p |   
                |------------------------------|55   
                | a  s  d  f  g  h  j  k  l CR |   
                |------------------------------|75   
                | z  x  c  v  b  n  m    tab BS|  
                |------------------------------|95
 
*/

TFT_eSPI tft;
#define LCD_BACKLIGHT (72Ul)

bool isLight=true;

void Display::init(){
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Wio Terminal by CheatPlayer",80,115);
    tft.fillRect(0, 0, 320, 15, TFT_CYAN);
}

void Display::print(String s,int x,int y){
    tft.drawString(s,x,y);
}

void Display::turnOn(){
    digitalWrite(LCD_BACKLIGHT, HIGH);
    isLight=true;
}

void Display::turnOff(){
    digitalWrite(LCD_BACKLIGHT, LOW);
    isLight=false;
}

void Display::showC(String s){
    tft.fillRect(0, 0, 60, 15, TFT_CYAN);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
    tft.drawString(s,2,4);
}

void Display::showB(String s){
    tft.fillRect(60, 0, 60, 15, TFT_CYAN);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
    tft.drawString(s,62,4);
}

void Display::showA(String s){
    tft.fillRect(120, 0, 60, 15, TFT_CYAN);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
    tft.drawString(s,122,4);
}

void Display::showCmd(String s){
    tft.fillRect(120, 0, 190, 15, TFT_CYAN);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
    tft.drawString(s,122,4);
}

void Display::showStatus(int x,int y,int16_t color){
    tft.fillRect(310, 0, 10, 15, TFT_CYAN);
    tft.fillCircle(x, y, 2, color);
}

void Display::clear(){
    tft.fillRect(0,15,320,225,TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

void Display::clearKeyboard(){
    tft.fillRect(100,15,200,80,TFT_BLACK);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
}

void Display::showKey(int x,int y,String s,int16_t color){
    tft.fillRect(x,y,19,19,color);
    tft.setTextColor(TFT_BLACK, color);
    tft.drawString(s,x+5,y+5);
}   

void Display::clearMenus(int len){
    tft.fillRect(0,15,70,len*20,TFT_BLACK);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
}

void Display::showMenu(int index,String name,int16_t color){
    tft.fillRect(0,15+index*20,70,19,color);
    tft.setTextColor(TFT_BLACK, color);
    tft.drawString(name,5,20+index*20);
}

void Display::clearScripts(int len){
    tft.fillRect(50,15,50,len*20,TFT_BLACK);
    tft.setTextColor(TFT_BLACK, TFT_CYAN);
}

void Display::showScripts(int index,String name,int16_t color){
    tft.fillRect(50,15+index*20,50,19,color);
    tft.setTextColor(TFT_BLACK, color);
    tft.drawString(name,55,20+index*20);
}