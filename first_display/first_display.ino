#include <LiquidCrystal.h>
#include <IRremote.h>
#define V0 9
#define RECV 10

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  //(RS, E, D4, D5, D6, D7)

IRrecv myIRrecv(RECV);
decode_results results; 

void setup() {
  
  pinMode(V0, OUTPUT);
  analogWrite(V0, 90); //setam intensitatea
  
  lcd.begin(16, 2); //mentionam (cate coloane, cate linii) vrem sa folosim
  lcd.clear();
  lcd.setCursor(0, 0);
  
  myIRrecv.enableIRIn();
  myIRrecv.blink13(true);

  Serial.begin(9600);
}

void loop() {

   
   //lcd.setCursor(0, 0);
   if (myIRrecv.decode(&results)){
    lcd.clear();
   delay(100);
       switch(results.value){
          case 0xFF30CF:
          {lcd.print("1");
          break ;}
          case 0xFF18E7:{
          lcd.print("2");
          break ;}
          case 0xFF7A85:{
          lcd.print("3");
          break ;}
          case 0xFF10EF:{
          lcd.print("4");
          break ;}
          case 0xFF38C7:{
          lcd.print("5");
          break ;}
          case 0xFF5AA5:{
          lcd.print("6");
          break ;}
          case 0xFF42BD:{
          lcd.print("7");
          break ;}
          case 0xFF4AB5:{
          lcd.print("8");
          break ;}
          case 0xFF52AD:{
          lcd.print("9");
          break ;}
      Serial.println(results.value, HEX);
      
   }
        myIRrecv.resume();
   }
}
