#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// librarie necasara pentru afisarea la LCD 
#include <DS3232RTC.h>
// librarie necesara pentru functionarea modulului RTC
      
DS3232RTC myRTC;
LiquidCrystal_I2C lcd(0x27, 16, 2);
// 2 linii si 16 caractere pe fiecare linie 

#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_FS4 370
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_E5  659

int D1 = 6;
int D2 = 9;
int D3 = 10;
int D4 = 11; 
int buzzer = A1;
int pinA = 2; 
int pinB = 3; 
int pinC = 4; 
int pinD = 5; 
int pinE = A0;
int pinF = 7; 
int pinG = 8; 
int button1=13; // Start numaratoare inversa
int button2=12; // Reset 
int button3=A3; // Minus
int button4=A2; // Plus
int initial_time = 60;
int tempo = 165;
byte char00[] = {B00001, B00011, B00111, B01111, B11111, B11111, B11111, B11111};
byte char01[] = {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B10101};
byte char02[] = {B10000, B11000, B11100, B11110, B11111, B11111, B11111, B11111};
byte char10[] = {B11111, B11111, B11111, B11111, B01111, B00111, B00011, B00001};
byte char11[] = {B11111, B10101, B11011, B11111, B11111, B11111, B11111, B11111};
byte char12[] = {B11111, B11111, B11111, B11111, B11110, B11100, B11000, B10000};
byte animation[] = {B11111, B10001, B10001, B10001, B10001, B11111, B11111, B11111};
// nota si durata ei- patrimi, optimi sau doimi
int melody[] = {NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, NOTE_CS5, 8, NOTE_B4, 8,
  NOTE_D4, 4, NOTE_E4, 4, NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4, NOTE_A4, 2};
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
// numarul total de note muzicale 
int wholenote = (60000 * 4) / tempo;
// durata unei note muzicale
int divider = 0, noteDuration = 0;

struct digits {
    int digit[4];
};

void setup() {
  Serial.begin(9600);              
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  pinMode(button4,INPUT_PULLUP);
  lcd.begin();
  lcd.backlight();
  lcd.home();
  myRTC.begin();
}

void lightNumber(int numberToDisplay) {
// functie de afisare a unei cifre la 7-segment display
  switch (numberToDisplay){
  case 0:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, LOW);
    break;

  case 1:
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
    break;

  case 2:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
    break;

  case 3:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, HIGH);
    break;

  case 4:
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    break;

  case 5:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    break;

  case 6:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    break;

  case 7:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
    break;

  case 8:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, HIGH);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    break;

  case 9:
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, HIGH);
    digitalWrite(pinD, HIGH);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, HIGH);
    digitalWrite(pinG, HIGH);
    break;

  case 10:
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
    digitalWrite(pinD, LOW);
    digitalWrite(pinE, LOW);
    digitalWrite(pinF, LOW);
    digitalWrite(pinG, LOW);
    break;  
  }
 
}

void SwitchDigit(int digit) {
// functie ce alege care dintre cele 4 cifre ale 7-segment display-ului va fi afisata 
  for (int i = 0; i < 4; i++) {
    if (i == digit) {
      if (i == 0) {
        digitalWrite(D1, LOW);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, HIGH);
        digitalWrite(D4, HIGH);
      }
      else if (i == 1) {
        digitalWrite(D1, HIGH);
        digitalWrite(D2, LOW);
        digitalWrite(D3, HIGH);
        digitalWrite(D4, HIGH);
      }
      else if (i == 2) {
        digitalWrite(D1, HIGH);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, LOW);
        digitalWrite(D4, HIGH);
      }
      else if (i == 3) {
        digitalWrite(D1, HIGH);
        digitalWrite(D2, HIGH);
        digitalWrite(D3, HIGH);
        digitalWrite(D4, LOW);
      }
    }
  }
}

struct digits todigits(int n){
// functie care primeste un numar de maxim 4 cifre si va returna
// intr-o structura toate cifrele care trebuie afisate pe 7-segment display
  struct digits mystruct;
  int zeros = 0;
  int d;
  for (int i = 0; i < 4; i++) {
    d = n / pow(10, 3 - i);
    zeros += d;
    n = n - d * pow(10, 3 - i);
    if ((zeros != 0) || (i==3)) {
      mystruct.digit[i] = d;
      // se va afisa cifra "d" pe pozitia "i"
    } else {
      mystruct.digit[i] = 10;
      // nu se va afisa nicio cifra pe pozitia "i"
    }
  }
  
return mystruct;
}

void PrintNumber(int n, int time) {
  struct digits mystruct;
  mystruct = todigits(n);
  
  for (int i = 0; i <= time / 20; i++) {
    if (digitalRead(button2) == LOW) {
    // daca este apasat butonul de reset in timpul numararii 
      return;
    }
    for (int j = 0; j < 4; j++) {
      // afiseaza pe rand fiecare cifra cu delay atat de mic
      // incat pare ca sunt afisate simultan
      SwitchDigit(j);
      lightNumber(mystruct.digit[j]);
      delay(5);
    }
  }
}

void displayTime() {
// functie care afiseaza ora, ziua, anul in timp real pe LCD 
  char buf[40];
  char buf1[40];
  time_t t = myRTC.get();
  sprintf(buf, "%.2d:%.2d:%.2d",hour(t), minute(t), second(t));
  sprintf(buf1, "%.2d %s %d", day(t),  monthShortStr(month(t)), year(t));
  lcd.setCursor(4, 0);
  lcd.print(buf);
  lcd.setCursor(3, 1);
  lcd.print(buf1);
}

bool countdown(int n, int del){
  for (int q = n; q > 0; q--){
    PrintNumber(q, del);
    if (digitalRead(button2) == LOW) {
      // daca este apasat butonul de reset in timpul numararii 
      return false;
    }
  }
  PrintNumber(0, 0);

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // durata fiecarei note 
    divider = melody[thisNote + 1];
    noteDuration = (wholenote) / divider;
    // buzzerul canta doar 90% din durata notei, restul fiind un delay
    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    // opreste buzzerul inainte de urmatoarea nota 
    noTone(buzzer);
  }

  // dupa ce buzzerul a terminat de cantat, LCD-ul afiseaza animatiile
  // menite sa anunte scurgerea timpului 
  lcd.clear();
  for (int i = 0; i < 3; i++) {
    lcd.createChar(0, char00);
    lcd.setCursor(0, 0);
    lcd.write(0);
    lcd.createChar(1, char01);
    lcd.setCursor(1, 0);
    lcd.write(1);
    lcd.createChar(2, char02);
    lcd.setCursor(2, 0);
    lcd.write(2);
    lcd.createChar(3, char10);
    lcd.setCursor(0, 1);
    lcd.write(3);
    lcd.createChar(4, char11);
    lcd.setCursor(1, 1);
    lcd.write(4);
    lcd.createChar(5, char12);
    lcd.setCursor(2, 1);
    lcd.write(5);
    lcd.createChar(0, char00);
    lcd.setCursor(13, 0);
    lcd.write(0);
    lcd.createChar(1, char01);
    lcd.setCursor(14, 0);
    lcd.write(1);
    lcd.createChar(2, char02);
    lcd.setCursor(15, 0);
    lcd.write(2);
    lcd.createChar(3, char10);
    lcd.setCursor(13, 1);
    lcd.write(3);
    lcd.createChar(4, char11);
    lcd.setCursor(14, 1);
    lcd.write(4);
    lcd.createChar(5, char12);
    lcd.setCursor(15, 1);
    lcd.write(5);
    delay(1000);
    lcd.clear();
    lcd.createChar(0, char00);
    lcd.setCursor(0, 0);
    lcd.write(0);
    lcd.createChar(1, char01);
    lcd.setCursor(1, 0);
    lcd.write(1);
    lcd.createChar(2, char02);
    lcd.setCursor(2, 0);
    lcd.write(2);
    lcd.createChar(3, char10);
    lcd.setCursor(0, 1);
    lcd.write(3);
    lcd.createChar(4, animation);
    lcd.setCursor(1, 1);
    lcd.write(4);
    lcd.createChar(5, char12);
    lcd.setCursor(2, 1);
    lcd.write(5);
    lcd.setCursor(5, 0);
    lcd.print("Timpul");
    lcd.setCursor(3, 1);
    lcd.print("s-a scurs!");
    lcd.createChar(0, char00);
    lcd.setCursor(13, 0);
    lcd.write(0);
    lcd.createChar(1, char01);
    lcd.setCursor(14, 0);
    lcd.write(1);
    lcd.createChar(2, char02);
    lcd.setCursor(15, 0);
    lcd.write(2);
    lcd.createChar(3, char10);
    lcd.setCursor(13, 1);
    lcd.write(3);
    lcd.createChar(4, animation);
    lcd.setCursor(14, 1);
    lcd.write(4);
    lcd.createChar(5, char12);
    lcd.setCursor(15, 1);
    lcd.write(5);
    delay(1000);
    if (i != 2)
      lcd.clear();
  }
  
  delay(500);
  lcd.clear();
  displayTime();
  delay(2000);
  lcd.clear();
  
return true;
}

void reset() {
  int m, zeros, d, pressed3 = 0, pressed4 = 0;
  m = initial_time;
  struct digits mystruct;
  mystruct = todigits(initial_time);
  displayTime();
  delay(3000); 
  lcd.clear(); 
  
  while (digitalRead(button1) == HIGH) {
    // daca nu este apasat butonul de start
    for (int j = 0; j < 4; j++) {
      SwitchDigit(j);
      lightNumber(mystruct.digit[j]);
      delay(2);
    }
    if (digitalRead(button3) == LOW) {
      // daca am apasat butonul 3
      if (pressed3 == 0 || pressed3 > 30) {
        if (initial_time > 0) {
          initial_time -= 1 ;
        }
        mystruct = todigits(initial_time);
      } 
      pressed3 += 1;
    }
    else if (digitalRead(button4) == LOW) {
      // daca am apasat butonul 4
      if (pressed4 == 0 || pressed4 > 30) {
        if (initial_time < 9999) {
          initial_time += 1 ;
        }
        mystruct = todigits(initial_time);
      } 
      pressed4 += 1;
    }
    if (digitalRead(button3) == HIGH) {
      // daca butonul 3 nu este apasat 
      pressed3 = 0;
    }
    if (digitalRead(button4) == HIGH) {
      // daca butonul 4 nu este apasat 
      pressed4 = 0;
    }
  }
}

void loop(){
  while (digitalRead(button2) == 1){
    displayTime();
    delay(200); 
    lcd.clear();
  };

  reset();
  while (countdown(initial_time, 962) == false) {
    reset();
  }
  
  while (digitalRead(button2) == 1){
    displayTime();
    delay(200); 
    lcd.clear();
  };
}
