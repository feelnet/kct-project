/*
   KRI - KRAI 2019
   -----------------------------------
   Type         : Manual Robot
   Author       : Arson Marianus
   Last Update  : Apr/02/2019, 20:42
   -----------------------------------
*/

#include <PS2X_lib.h>
#include <Servo.h>

#define DATA         A0
#define CMD          A2
#define SEL          A1
#define CLK          A3

#define PANJANG_ON   51
#define PANJANG_NO   53

#define BKIR_PWM     3
#define DKAN_PWM     5
#define BKAN_PWM     2
#define DKIR_PWM     4
#define PANJANG_PWM  6

#define NORM_SPEED  110
#define MIN_SPEED   0
#define TURB_SPEED  255

byte  DKAN_B = 0,
      DKAN_F = 0,
      DKIR_B = 0,
      DKIR_F = 0,
      BKIR_B = 0,
      BKIR_F = 0,
      BKAN_B = 0,
      BKAN_F = 0;

unsigned int time_step = 0;
byte ser = 0, count = 0, flag_mode  = 0;
bool flag_p = 0;
PS2X psx;
Servo swipe;
Servo grab;

void setup() {
  Serial.begin(57600);
  delay(300);

  for (byte i = 35; i < 54; i += 2) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (byte i = 2; i < 7; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(11, INPUT_PULLUP);


  while (true) {
    int error = psx.config_gamepad(CLK, CMD, SEL, DATA, false, false);
    if (error == 0) {
      Serial.println("System Is Ready...");
      flag_mode = 0;
      mode();
      delay(300);
      break;
    }
    delay(300);
  }
  swipe.attach(13);
  grab.attach(12);

  swipe.write(90);
  grab.write(180);
}
void loop() {
  psx.read_gamepad(false, false);
  if (flag_p == 1 && digitalRead(11) == 0) {
    digitalWrite(PANJANG_ON, HIGH);
    digitalWrite(PANJANG_NO, HIGH);
    analogWrite(PANJANG_PWM, TURB_SPEED);
  }
    if (flag_p == 1 && digitalRead(11) == 1) {
    digitalWrite(PANJANG_ON, HIGH);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, TURB_SPEED);
  }
  if (psx.Button(PSB_L1) && psx.Button(PSB_L2) && psx.ButtonPressed(PSB_R1)) {
    count++;
    flag_mode = (count % 2 == 0) ? 0 : 1;
    mode();
  }

  if (!psx.Button(PSB_L1)  && !psx.Button(PSB_L2) && psx.Button(PSB_CIRCLE))          belokKanan();
  else if (!psx.Button(PSB_L1)  && !psx.Button(PSB_L2) && psx.Button(PSB_SQUARE))     belokKiri();

  else if (psx.Button(PSB_L1)  && psx.Button(PSB_L2) && psx.Button(PSB_CIRCLE))     turboBelokKanan();
  else if (psx.Button(PSB_L1)  && psx.Button(PSB_L2) && psx.Button(PSB_SQUARE))     turboBelokKiri();

  else if (psx.Button(PSB_L1)   && !psx.Button(PSB_L2) && psx.Button(PSB_CIRCLE)) {
    if (flag_mode == 0) {
      putarKanan();
    }
    else if (flag_mode == 1) {
      putarKiri();
    }
  }

  else if (psx.Button(PSB_L1)   && !psx.Button(PSB_L2) && psx.Button(PSB_SQUARE)) {
    if (flag_mode == 0) {
      putarKiri();
    }
    else if (flag_mode == 1) {
      putarKanan();
    }
  }

  else if (psx.Button(PSB_L2)   && !psx.Button(PSB_L1) && psx.Button(PSB_CIRCLE)) {
    if (flag_mode == 0) {
      balikKanan();
    }
    else if (flag_mode == 1) {
      balikKiri();
    }
  }

  else if (psx.Button(PSB_L2)   && !psx.Button(PSB_L1) && psx.Button(PSB_SQUARE)) {
    if (flag_mode == 0) {
      balikKiri();
    }
    else if (flag_mode == 1) {
      balikKanan();
    }
  }

  else if (psx.Button(PSB_L2)   && !psx.Button(PSB_L1) && psx.Button(PSB_TRIANGLE))   serongKiri();
  else if (psx.Button(PSB_L2)   && !psx.Button(PSB_L1) && psx.Button(PSB_CROSS))      serongKanan();

  else if (!psx.Button(PSB_L2)   && !psx.Button(PSB_L1)  && psx.Button(PSB_TRIANGLE)) maju();
  else if (!psx.Button(PSB_L2)   && psx.Button(PSB_L1)   && psx.Button(PSB_TRIANGLE)) turboMaju();
  else if (psx.Button(PSB_L2)   && !psx.Button(PSB_L1)  && psx.Button(PSB_TRIANGLE)) serongKiri();


  else if (!psx.Button(PSB_L2)   && !psx.Button(PSB_L1)  && psx.Button(PSB_CROSS))    mundur();
  else if (!psx.Button(PSB_L2)   && psx.Button(PSB_L1)   && psx.Button(PSB_CROSS))    turboMundur();
  else if (psx.Button(PSB_L2)   && !psx.Button(PSB_L1)  && psx.Button(PSB_CROSS)) serongKanan();

  else diam();

  if (psx.ButtonPressed(PSB_PAD_UP))      panjangOn();
  if (psx.Button(PSB_PAD_DOWN))    panjangNo();

  if (psx.Button(PSB_PAD_LEFT))    angkat();
  if (psx.Button(PSB_PAD_RIGHT))   turun();

  if (!psx.Button(PSB_L1)  && !psx.Button(PSB_L2) && psx.ButtonPressed(PSB_R1))  dorong();
  if (!psx.Button(PSB_L1)  && !psx.Button(PSB_L2) && psx.ButtonReleased(PSB_R1)) tarik();

  if (psx.ButtonPressed(PSB_R2)) penjepit();

  if ((!psx.Button(PSB_L2)          &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_TRIANGLE))   ||
      (!psx.Button(PSB_L2)          &&  psx.Button(PSB_L1)              && psx.ButtonReleased(PSB_TRIANGLE))   ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.Button(PSB_TRIANGLE))           ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.ButtonReleased(PSB_TRIANGLE))   ||
      (!psx.Button(PSB_L2)          &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_CROSS))      ||
      (!psx.Button(PSB_L2)          &&  psx.Button(PSB_L1)              && psx.ButtonReleased(PSB_CROSS))      ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.Button(PSB_CROSS))              ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.ButtonReleased(PSB_CROSS))      ||
      (!psx.Button(PSB_L2)          &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_CIRCLE))     ||
      (!psx.Button(PSB_L2)          &&  psx.Button(PSB_L1)              && psx.ButtonReleased(PSB_CIRCLE))     ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.Button(PSB_CIRCLE))             ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.ButtonReleased(PSB_CIRCLE))     ||
      (psx.Button(PSB_L2)           &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_CIRCLE))     ||
      (psx.Button(PSB_L2)           &&  psx.Button(PSB_L1)              && psx.ButtonReleased(PSB_CIRCLE))     ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.Button(PSB_CIRCLE))             ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_CIRCLE))     ||
      (!psx.Button(PSB_L2)          &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_SQUARE))     ||
      (!psx.Button(PSB_L2)          &&  psx.Button(PSB_L1)              && psx.ButtonReleased(PSB_SQUARE))     ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.Button(PSB_SQUARE))             ||
      (!psx.Button(PSB_L2)          &&  psx.ButtonReleased(PSB_L1)      && psx.ButtonReleased(PSB_SQUARE))     ||
      (psx.Button(PSB_L2)           &&  psx.Button(PSB_L1)              && psx.ButtonReleased(PSB_SQUARE))     ||
      (psx.Button(PSB_L2)           &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_SQUARE))     ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.Button(PSB_SQUARE))             ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_SQUARE))     ||
      (psx.Button(PSB_L2)           &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_TRIANGLE))   ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.Button(PSB_TRIANGLE))           ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_TRIANGLE))   ||
      (psx.Button(PSB_L2)           &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_CROSS))      ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.Button(PSB_CROSS))              ||
      (psx.ButtonReleased(PSB_L2)   &&  !psx.Button(PSB_L1)             && psx.ButtonReleased(PSB_CROSS))) {
    time_step = 0;
  }
}
void maju() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);
  
  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void turboMaju() {
  time_step += 8;
  if (time_step >= TURB_SPEED) {
    time_step = TURB_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }

  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void mundur() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, HIGH);
  digitalWrite(BKIR_B, HIGH);
  digitalWrite(DKIR_B, HIGH);
  digitalWrite(BKAN_B, HIGH);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }

  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void turboMundur() {
  time_step += 8;
  if (time_step >= TURB_SPEED) {
    time_step = TURB_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, HIGH);
  digitalWrite(BKIR_B, HIGH);
  digitalWrite(DKIR_B, HIGH);
  digitalWrite(BKAN_B, HIGH);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void belokKanan() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, HIGH);
  digitalWrite(BKIR_B, HIGH);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}

void belokKiri() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, HIGH);
  digitalWrite(BKAN_B, HIGH);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void turboBelokKanan() {
  time_step += 8;
  if (time_step >= TURB_SPEED) {
    time_step = TURB_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, HIGH);
  digitalWrite(BKIR_B, HIGH);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}

void turboBelokKiri() {
  time_step += 8;
  if (time_step >= TURB_SPEED) {
    time_step = TURB_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, HIGH);
  digitalWrite(BKAN_B, HIGH);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }

  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void putarKanan() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, HIGH);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, HIGH);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void putarKiri() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, HIGH);
  digitalWrite(DKIR_B, HIGH);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void balikKanan() {
  time_step += 8;
  if (time_step >= TURB_SPEED) {
    time_step = TURB_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, HIGH);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, HIGH);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void balikKiri() {
  time_step += 8;
  if (time_step >= TURB_SPEED) {
    time_step = TURB_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, HIGH);
  digitalWrite(DKIR_B, HIGH);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, time_step);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  delay(20);
}
void serongKiri() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, HIGH);
  digitalWrite(BKIR_F, HIGH);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }

  analogWrite(DKAN_PWM, time_step);
  analogWrite(BKIR_PWM, time_step);
  analogWrite(DKIR_PWM, MIN_SPEED);
  analogWrite(BKAN_PWM, MIN_SPEED);
  delay(20);
}
void serongKanan() {
  time_step += 8;
  if (time_step >= NORM_SPEED) {
    time_step = NORM_SPEED;
  }
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, HIGH);
  digitalWrite(BKAN_F, HIGH);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    analogWrite(PANJANG_PWM, LOW);
  }

  analogWrite(DKAN_PWM, MIN_SPEED);
  analogWrite(BKIR_PWM, MIN_SPEED);
  analogWrite(DKIR_PWM, time_step);
  analogWrite(BKAN_PWM, time_step);
  delay(20);
}
void diam() {
  digitalWrite(DKAN_F, LOW);
  digitalWrite(BKIR_F, LOW);
  digitalWrite(DKIR_F, LOW);
  digitalWrite(BKAN_F, LOW);
  digitalWrite(DKAN_B, LOW);
  digitalWrite(BKIR_B, LOW);
  digitalWrite(DKIR_B, LOW);
  digitalWrite(BKAN_B, LOW);

  if (flag_p != 1) {
    digitalWrite(PANJANG_ON, LOW);
    digitalWrite(PANJANG_NO, LOW);
    digitalWrite(PANJANG_PWM, LOW);
  }
  
  analogWrite(DKAN_PWM, MIN_SPEED);
  analogWrite(BKIR_PWM, MIN_SPEED);
  analogWrite(DKIR_PWM, MIN_SPEED);
  analogWrite(BKAN_PWM, MIN_SPEED);
  delay(20);
}
void panjangOn() {
  flag_p = 1;
  digitalWrite(PANJANG_ON, HIGH);
  digitalWrite(PANJANG_NO, LOW);
  analogWrite(PANJANG_PWM, 255);
  delay(20);
}
void panjangNo() {
  flag_p = 0;
  digitalWrite(PANJANG_ON, LOW);
  digitalWrite(PANJANG_NO, HIGH);
  analogWrite(PANJANG_PWM, TURB_SPEED);
  delay(20);
}
void angkat() {
  digitalWrite(9, LOW);
  delay(20);
}
void turun() {
  digitalWrite(9, HIGH);
  delay(20);
}
void dorong() {
  digitalWrite(8, LOW);
  delay(5);
}
void tarik() {
  digitalWrite(8, HIGH);
  delay(5);
}
void penjepit() {
  ser++;
  if (ser == 1)
  {
    swipe.write(0);
  }
  else if (ser == 2)
  {
    grab.write(0);
  }
  else if (ser == 3)
  {
    grab.write(180);
    delay(1000);
    swipe.write(90);
    ser = 0;
  }
}

void mode() {
  if (flag_mode == 0) {
    DKAN_B = 49,
    DKAN_F = 47,
    DKIR_B = 45,
    DKIR_F = 43,
    BKIR_B = 41,
    BKIR_F = 39,
    BKAN_B = 35,
    BKAN_F = 37;
  }
  else {
    DKAN_B = 47,
    DKAN_F = 49,
    DKIR_B = 43,
    DKIR_F = 45,
    BKIR_B = 39,
    BKIR_F = 41,
    BKAN_B = 37,
    BKAN_F = 35;
  }
}
