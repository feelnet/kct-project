/*
  KRI - KRAI 2019
  ------------------------------------
  Type          : Automatic Robot
  Author        : Arson Marianus
  Last  Update  : Apr/20/2019, 19:25
  ------------------------------------
*/

#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <EEPROM.h>

#define KIRLU1              51
#define KIRLU2              53
#define KIRLU6              6

#define KANLU1              49
#define KANLU2              47
#define KANLU6              7

#define KIRDAL1             2
#define KIRDAL2             3
#define KANDAL1             5
#define KANDAL2             8

#define PUTIH_SENS_KIRLU    22
#define PUTIH_SENS_KANLU    24
#define PUTIH_SENS_KANDAL   28
#define PUTIH_SENS_KIRDAL   26

#define HITAM_SENS_KIRLU    18
#define HITAM_SENS_KANLU    19
#define HITAM_SENS_KIRDAL   20
#define HITAM_SENS_KANDAL   21
byte delay_timing = 0;
byte flag_A = 0,
     flag_B = 0,
     flag_C = 0,
     flag_D = 0,
     flag_Robot = 0,
     flag_Break = 0,
     flag_gerege_comp = 0;

byte count7 = 0;
byte lalala = 0;
byte count11 = 0;
byte buttonPin = 14;
byte bacaPutihKIRLU  = 0,
     bacaPutihKANLU  = 0,
     bacaPutihKANDAL = 0,
     bacaPutihKIRDAL = 0,
     bacaHitamKIRLU  = 0,
     bacaHitamKANLU  = 0,
     bacaHitamKANDAL = 0,
     bacaHitamKIRDAL = 0;

byte geregeSensorPin = 10;
byte mountainSensorPin = 15;

unsigned long last_micros = 0;
unsigned long micros_A = 0;

byte KIRLU_SPEED = 210,
     KANLU_SPEED = 215,
     KIRDAL_SPEED = 220,
     KANDAL_SPEED = 240,
     stepCount = 0,
     stepStart = 25,
     stepTurn1 = 4,
     stepCP1pre = 6,
     stepCP1 = 12,
     stepTurn2 = 4,
     stepCP2 = 7,
     stepTurn3 = 7,
     stepCP3 = 19,
     stepTurn4 = 7,
     stepFinal = 30;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A7, A6, A5, A4};
byte colPins[COLS] = {A3, A2, A1, A0};
char data[3] = {NULL, NULL, NULL};
char rst = '0';
byte pos = 2, a = 0, mode = 0;
bool flag_s = 0, flag_m = 0, flag_k = 0, flag_t = 0;
bool flag_out = 0;
byte flag_jalan = 0;

const int rs = 43, en = 41, d4 = 39, d5 = 37, d6 = 35, d7 = 33;

int addr_mode = 0,
    ee_kirlu = 1,
    ee_kirdal = 2,
    ee_kandal = 3,
    ee_kanlu = 4,
    ee_stepTurn1 = 5,
    ee_stepCP1pre = 6,
    ee_stepCP1 = 7,
    ee_stepTurn2 = 8,
    ee_stepCP2 = 9,
    ee_stepTurn3 = 10,
    ee_stepCP3 = 11,
    ee_stepTurn4 = 12,
    ee_stepFinal = 13,
    ee_stepStart = 14,
    ee_rst = 15,
    ee_flag_robot = 16;


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo serv1;
Servo serv2;
Servo geregeServo;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {

  TCCR3B = TCCR3B & 0b11111000 | 0x05;
  TCCR4B = TCCR4B & 0b11111000 | 0x05;

  Serial.begin(57600);
  lcd.begin(16, 4);
  konfigurasiPin();

  mode = EEPROM.read(addr_mode);
  rst = EEPROM.read(ee_rst);

  KIRLU_SPEED = EEPROM.read(ee_kirlu);
  KIRDAL_SPEED = EEPROM.read(ee_kirdal);
  KANDAL_SPEED = EEPROM.read(ee_kandal);
  KANLU_SPEED = EEPROM.read(ee_kanlu);
  stepTurn1 = EEPROM.read(ee_stepTurn1);
  stepTurn2 = EEPROM.read(ee_stepTurn2);
  stepTurn3 = EEPROM.read(ee_stepTurn3);
  stepTurn4 = EEPROM.read(ee_stepTurn4);
  stepCP1pre = EEPROM.read(ee_stepCP1pre);
  stepCP2 = EEPROM.read(ee_stepCP2);
  stepFinal = EEPROM.read(ee_stepFinal);
  stepCP1 = EEPROM.read(ee_stepCP1);
  stepCP3 = EEPROM.read(ee_stepCP3);
  stepStart = EEPROM.read(ee_stepStart);
  flag_Robot = EEPROM.read(ee_flag_robot);
  serv1.attach(12);
  serv2.attach(11);
  geregeServo.attach(13);
  while (true) {
    lcd.setCursor(0, 0);
    lcd.print("  ANYYARANG TEAM");
    lcd.setCursor(0, 1);
    lcd.print("___KRI_KRAI_2019");
    lcd.setCursor(0, 2);
    lcd.print("#KCTMENANG");
    lcd.setCursor(0, 3);
    lcd.print("#KCTJUARA  #AMIN");
    serv1.write(80);
    serv2.write(100);
    geregeServo.write(180);
    char op = keypad.getKey();
    if (op != NULL) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Tunggu...");
      delay(500);
      lcd.setCursor(0, 1);
      lcd.print("Siap...");
      delay(500);
      break;
    }
  }
  micros_A = micros();
  last_micros = micros();
  lcd.clear();
}
void loop() {
  buttonCheck();
  setup_();
  if (flag_Robot == 0) {
    serv1.write(0);
    serv2.write(170);
    diam();
  }
  if (flag_Robot == 1) {
    if (mode == 1) {
      if (rst == '0') {
        geregeServo.write(170);
        delay(3000);
        jalanLurus();
      }
      else if (rst == 'A') {
        while (true) {
          last_micros = micros();
          while (true) {
            interPin();
            if (flag_Break == 1) {
              flag_Break = 0;
              flag_gerege_comp = 1;
              break;
            }
            byte x = digitalRead(geregeSensorPin);
            if (micros() - last_micros >= 500000) {
              last_micros = micros();
              if (x == 0) {
                geregeServo.write(180);
                flag_gerege_comp = 0;
                break;
              }
              else if (x == 1) {
                geregeServo.write(140);
              }
            }
          }
          if (flag_gerege_comp == 1) {
            flag_gerege_comp = 0;
            lcd.clear();
            break;
          }
          else if (flag_gerege_comp == 0) {
            delay(3000);
            stepCount = 1;
            jalanMode_A();
          }
        }
      }
      else if (rst == 'B') {
        stepCount = stepStart + stepTurn1 + 1;
        geregeServo.write(170);
        delay(3000);
        jalanMode_A_();
      }
      else if (rst == 'C') {
        stepCount = stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + 1;
        geregeServo.write(170);
        delay(3000);
        jalanMode_A_();
      }
      else if (rst == 'D') {
        while (true) {
          last_micros = micros();
          while (true) {
            interPin();
            if (flag_Break == 1) {
              flag_Break = 0;
              flag_gerege_comp = 1;
              break;
            }
            byte x = digitalRead(mountainSensorPin);
            if (micros() - last_micros >= 300000) {
              last_micros = micros();
              if (x == 0) {
                geregeServo.write(170);
                flag_gerege_comp = 0;
                break;
              }
              else if (x == 1) {
                geregeServo.write(170);
              }
            }
          }
          if (flag_gerege_comp == 1) {
            flag_gerege_comp = 0;
            lcd.clear();
            break;
          }
          else if (flag_gerege_comp == 0) {
            stepCount = stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + 1;
            geregeServo.write(170);
            delay(1000);
            jalanMode_A();
          }
        }
      }
    }
    else if (mode == 2) {
      if (rst == '0') {
        geregeServo.write(170);
        delay(3000);
        jalanLurus();
      }
      else if (rst == 'A') {
        while (true) {
          last_micros = micros();
          while (true) {
            interPin();
            if (flag_Break == 1) {
              flag_Break = 0;
              flag_gerege_comp = 1;
              break;
            }
            byte x = digitalRead(geregeSensorPin);
            if (micros() - last_micros >= 500000) {
              last_micros = micros();
              if (x == 0) {
                geregeServo.write(170);
                flag_gerege_comp = 0;
                break;
              }
              else if (x == 1) {
                geregeServo.write(140);
              }
            }
          }
          if (flag_gerege_comp == 1) {
            flag_gerege_comp = 0;
            lcd.clear();
            break;
          }
          else if (flag_gerege_comp == 0) {
            delay(3000);
            stepCount = 1;
            jalanMode_B();
          }
        }
      }
      else if (rst == 'B') {
        stepCount = stepStart + stepTurn1 + 1;
        geregeServo.write(170);
        delay(3000);
        jalanMode_B_();
      }
      else if (rst == 'C') {
        stepCount = stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + 1;
        geregeServo.write(170);
        delay(3000);
        jalanMode_B_();
      }
      else if (rst == 'D') {
        while (true) {
          last_micros = micros();
          while (true) {
            interPin();
            if (flag_Break == 1) {
              flag_Break = 0;
              flag_gerege_comp = 1;
              break;
            }
            byte x = digitalRead(mountainSensorPin);
            if (micros() - last_micros >= 30000) {
              last_micros = micros();
              if (x == 0) {
                geregeServo.write(170);
                flag_gerege_comp = 0;
                break;
              }
              else if (x == 1) {
                geregeServo.write(170);
              }
            }
          }
          if (flag_gerege_comp == 1) {
            flag_gerege_comp = 0;
            lcd.clear();
            break;
          }
          else if (flag_gerege_comp == 0) {
            stepCount = stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + 1;
            geregeServo.write(170);
            delay(1000);
            jalanMode_B();
          }
        }
      }
    }
    else if (mode == 0) {
      diam();
    }
  }
}
void setup_() {
  lcd.setCursor(0, 0);
  lcd.print(">> MENU");
  if (flag_Robot == 0) {
    lcd.setCursor(13, 0);
    lcd.print("OFF");
  }
  else if (flag_Robot == 1) {
    lcd.setCursor(13, 0);
    lcd.print("ON  ");
  }
  lcd.setCursor(0, 1);
  lcd.print("1. Pos : ");
  lcd.setCursor(9, 1);
  if (mode == 0) {
    lcd.print("-");
  }
  if (mode == 1) {
    lcd.print("Merah");
  }
  if (mode == 2) {
    lcd.print("Biru");
  }
  lcd.setCursor(0, 2);
  lcd.print("2. Kec : Cek/Set");
  lcd.setCursor(0, 3);
  lcd.print("3. CP  : -");
  lcd.setCursor(9, 3);
  if (rst == '0') {
    lcd.print("Lurus");
  }
  if (rst == 'A') {
    lcd.print("Start");
  }
  if (rst == 'B') {
    lcd.print("Sand Dune");
  }
  if (rst == 'C') {
    lcd.print("Tussok");
  }
  if (rst == 'D') {
    lcd.print("Mountain");
  }

  char key = keypad.getKey();
  switch (key) {
    case '1' :
      setPosisi();
      break;
    case '2' :
      setKecepatan();
      break;
    case '3' :
      break;
    case '7':
      count7++;
      if (count7 % 2 == 0) {
        count7 = 0;
        diam();
        break;
      }
      else if (count7 % 2 != 0) {
        while (true) {
          setup_();
          if (count7 % 2 == 0) {
            break;
          }
          aturPosisi();
        }
      }
      break;
    case '4':
      count7++;
      if (count7 % 2 == 0) {
        count7 = 0;
        diam();
        break;
      }
      else if (count7 % 2 != 0) {
        while (true) {
          setup_();
          if (count7 % 2 == 0) {
            break;
          }
          digitalWrite(KIRLU1, HIGH);
          digitalWrite(KIRLU2, HIGH);
          analogWrite(KIRLU6, KIRLU_SPEED);

          digitalWrite(KANLU1, HIGH);
          digitalWrite(KANLU2, LOW);
          analogWrite(KANLU6, (215 - 150));

          analogWrite(KIRDAL1, 0);
          analogWrite(KANDAL1, (240 - 60));
          analogWrite(KIRDAL2, (220 - 70));
          analogWrite(KANDAL2, 0);
        }
      }
      break;
    case '5':
      count7++;
      if (count7 % 2 == 0) {
        count7 = 0;
        diam();
        break;
      }
      else if (count7 % 2 != 0) {
        while (true) {
          setup_();
          if (count7 % 2 == 0) {
            break;
          }
          digitalWrite(KIRLU1, HIGH);
          digitalWrite(KIRLU2, LOW);
          analogWrite(KIRLU6, (210 - 160));

          digitalWrite(KANLU1, HIGH);
          digitalWrite(KANLU2, HIGH);
          analogWrite(KANLU6, KANLU_SPEED);

          analogWrite(KIRDAL1, (220 - 60));
          analogWrite(KANDAL1, 0);
          analogWrite(KIRDAL2, 0);
          analogWrite(KANDAL2, (240 - 70));
        }
      }
      break;
    case '6':
      speedAwal();
      while (true) {
        kalibrasiHitam();
        if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

          Serial.println("Kalibrasi Hitam OK!!");

          flag_A = 0;
          flag_B = 0;
          flag_C = 0;
          flag_D = 0;

          break;
        }
      }
      break;
    case '8':
      speedAwal();
      while (true) {
        kalibrasi();
        if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

          Serial.println("Kalibrasi OK!!");

          flag_A = 0;
          flag_B = 0;
          flag_C = 0;
          flag_D = 0;

          break;
        }
      }
      break;
    case '9' :
      mode = 0;
      rst = '0';
      KIRLU_SPEED = 210;
      KANLU_SPEED = 215;
      KIRDAL_SPEED = 220;
      KANDAL_SPEED = 240;
      stepStart = 25;
      stepTurn1 = 4;
      stepCP1pre = 6;
      stepCP1 = 12;
      stepTurn2 = 4;
      stepCP2 = 7;
      stepTurn3 = 7;
      stepCP3 = 19;
      stepTurn4 = 7;
      stepFinal = 30;

      lcd.clear();

      EEPROM.write(addr_mode, mode);
      EEPROM.write(ee_rst, rst);
      EEPROM.write(ee_kirlu, KIRLU_SPEED);
      EEPROM.write(ee_kirdal, KIRDAL_SPEED);
      EEPROM.write(ee_kandal, KANDAL_SPEED);
      EEPROM.write(ee_kanlu, KANLU_SPEED);
      EEPROM.write(ee_stepTurn1, stepTurn1);
      EEPROM.write(ee_stepTurn2, stepTurn2);
      EEPROM.write(ee_stepTurn3, stepTurn3);
      EEPROM.write(ee_stepTurn4, stepTurn4);
      EEPROM.write(ee_stepCP1pre, stepCP1pre);
      EEPROM.write(ee_stepCP2, stepCP2);
      EEPROM.write(ee_stepFinal, stepFinal);
      EEPROM.write(ee_stepCP1, stepCP1);
      EEPROM.write(ee_stepCP3, stepCP3);
      EEPROM.write(ee_stepStart, stepStart);
      EEPROM.write(ee_flag_robot, 0);

      break;
    case '0' :
      rst = '0';
      lcd.clear();
      EEPROM.write(ee_rst, rst);
      break;
    case 'A' :
      rst = 'A';
      lcd.clear();
      EEPROM.write(ee_rst, rst);
      break;
    case 'B' :
      rst = 'B';
      lcd.clear();
      EEPROM.write(ee_rst, rst);
      break;
    case 'C' :
      rst = 'C';
      lcd.clear();
      EEPROM.write(ee_rst, rst);
      break;
    case 'D' :
      rst = 'D';
      lcd.clear();
      EEPROM.write(ee_rst, rst);
      break;
  }
}
void aturPosisi() {

  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, LOW);
  analogWrite(KIRLU6, KIRLU_SPEED);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, LOW);
  analogWrite(KANLU6, KANLU_SPEED);

  analogWrite(KIRDAL1, KIRDAL_SPEED);
  analogWrite(KANDAL1, KANDAL_SPEED);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, 0);
}
void setPosisi() {
  lcd.clear();
  while (true) {
    if (flag_k == 1) {
      flag_k = 0;
      lcd.clear();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print(">> SET POSISI");
    lcd.setCursor(0, 1);
    lcd.print("1. Merah");
    lcd.setCursor(0, 2);
    lcd.print("2. Biru");
    lcd.setCursor(0, 3);
    lcd.print("C. Kembali");

    char op = keypad.getKey();
    switch (op) {
      case 'C' :
        flag_k = 1;
        lcd.clear();
        break;
      case '1' :
        mode = 1;
        EEPROM.write(addr_mode, mode);
        flag_k = 1;
        lcd.clear();
        break;
      case '2' :
        mode = 2;
        EEPROM.write(addr_mode, mode);
        flag_k = 2;
        lcd.clear();
        break;
    }
  }
}
void setKecepatan() {
  lcd.clear();
  while (true) {
    if (flag_k == 1) {
      flag_k = 0;
      lcd.clear();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print(">> SETTINGS");
    lcd.setCursor(0, 1);
    lcd.print("1. Kecepatan");
    lcd.setCursor(0, 2);
    lcd.print("2. Langkah");
    lcd.setCursor(0, 3);
    lcd.print("C. Kembali");
    char op = keypad.getKey();
    switch (op) {
      case 'C':
        flag_k = 1;
        lcd.clear();
        break;
      case '1' :
        lcd.clear();
        while (true) {
          if (flag_m == 1) {
            flag_m = 0;
            lcd.clear();
            break;
          }
          lcd.setCursor(0, 0);
          lcd.print(">> SET KECEPATAN");
          lcd.setCursor(0, 1);
          lcd.print("1. KILU");
          lcd.setCursor(0, 2);
          lcd.print("2. KIDA");
          lcd.setCursor(0, 3);
          lcd.print("3. KADA");
          lcd.setCursor(9, 1);
          lcd.print("4. KALU");
          lcd.setCursor(9, 2);
          lcd.print("5. Cek");
          lcd.setCursor(9, 3);
          lcd.print("C. Kmbl");
          char opt = keypad.getKey();
          switch (opt) {
            case 'C' :
              flag_m = 1;
              lcd.clear();
              break;
            case '1':
              setInput(KIRLU_SPEED, ee_kirlu, "Kec KIRLU");
              break;
            case '2':
              setInput(KIRDAL_SPEED, ee_kirdal, "Kec KIRDAL");
              break;
            case '3':
              setInput(KANDAL_SPEED, ee_kandal, "Kec KANDAL");
              break;
            case '4' :
              setInput(KANLU_SPEED, ee_kanlu, "Kec KANLU");
              break;
            case '5':
              cekKecepatan();
              break;
          }
        }
        break;
      case '2':
        lcd.clear();
        while (true) {
          if (flag_m == 1) {
            flag_m = 0;
            lcd.clear();
            break;
          }
          lcd.setCursor(0, 0);
          lcd.print(">> SET LANGKAH");
          lcd.setCursor(0, 1);
          lcd.print("1. Strt");
          lcd.setCursor(0, 2);
          lcd.print("2. CP1");
          lcd.setCursor(0, 3);
          lcd.print("3. CP2");
          lcd.setCursor(9, 1);
          lcd.print("4. CP3");
          lcd.setCursor(9, 2);
          lcd.print("5. Cek");
          lcd.setCursor(9, 3);
          lcd.print("C. Kmbl");
          char opt = keypad.getKey();
          switch (opt) {
            case 'C' :
              flag_m = 1;
              lcd.clear();
              break;
            case '1' :
              setInput(stepStart, ee_stepStart, "Langkah Start");
              break;
            case '2' :
              lcd.clear();
              while (true) {
                if (flag_s == 1) {
                  flag_s = 0;
                  lcd.clear();
                  break;
                }
                lcd.setCursor(0, 0);
                lcd.print("1. CP1 Belok");
                lcd.setCursor(0, 1);
                lcd.print("2. CP1 Lurus");
                lcd.setCursor(0, 2);
                lcd.print("3. CP1 Sand");
                lcd.setCursor(0, 3);
                lcd.print("C. Kembali");
                char opts = keypad.getKey();
                switch (opts) {
                  case 'C':
                    flag_s = 1;
                    lcd.clear();
                    break;
                  case '1':
                    setInput(stepTurn1, ee_stepTurn1, "CP1 Belok");
                    break;
                  case '2':
                    setInput(stepCP1pre, ee_stepCP1pre, "CP1 Lurus");
                    break;
                  case '3':
                    setInput(stepCP1, ee_stepCP1, "CP1 Sand");
                    break;
                }
              }
              break;
            case '3' :
              lcd.clear();
              while (true) {
                if (flag_s == 1) {
                  flag_s = 0;
                  lcd.clear();
                  break;
                }
                lcd.setCursor(0, 0);
                lcd.print("1. CP2 Belok");
                lcd.setCursor(0, 1);
                lcd.print("2. CP2 Lurus");
                lcd.setCursor(0, 2);
                lcd.print("3. CP2 Bel Tussk");
                lcd.setCursor(0, 3);
                lcd.print("4. CP2 Tsk C. <<");
                char opts = keypad.getKey();
                switch (opts) {
                  case 'C':
                    flag_s = 1;
                    lcd.clear();
                    break;
                  case '1':
                    setInput(stepTurn2, ee_stepTurn2, "CP2 Belok");
                    break;
                  case '2':
                    setInput(stepCP2, ee_stepCP2, "CP2 Lurus");
                    break;
                  case '3':
                    setInput(stepTurn3, ee_stepTurn3, "CP2 Belok Tssk");
                    break;
                  case '4':
                    setInput(stepCP3, ee_stepCP3, "CP2 Tussok");
                    break;
                }
              }
              break;
            case '4' :
              lcd.clear();
              while (true) {
                if (flag_s == 1) {
                  flag_s = 0;
                  lcd.clear();
                  break;
                }
                lcd.setCursor(0, 0);
                lcd.print("1. CP3 Belok");
                lcd.setCursor(0, 1);
                lcd.print("2. CP3 Lurus");
                lcd.setCursor(0, 2);
                lcd.print("C. Kembali");
                char opts = keypad.getKey();
                switch (opts) {
                  case 'C':
                    flag_s = 1;
                    lcd.clear();
                    break;
                  case '1':
                    setInput(stepTurn4, ee_stepTurn4, "CP3 Belok");
                    break;
                  case '2':
                    setInput(stepFinal, ee_stepFinal, "CP3 Lurus");
                    break;
                }
              }
              break;
            case '5' :
              cekLangkah();
              break;
          }
        }
        break;
    }
  }
}
void cekLangkah() {
  lcd.clear();
  while (true) {
    if (flag_s == 1) {
      flag_s = 0;
      lcd.clear();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print("STR:   ");
    lcd.setCursor(5, 0);
    lcd.print(stepStart);
    lcd.setCursor(8, 0);
    lcd.print("C. Kmbl");

    lcd.setCursor(0, 1);
    lcd.print("CP1: ");
    lcd.setCursor(5, 1);
    lcd.print(stepTurn1);
    lcd.setCursor(8, 1);
    lcd.print(stepCP1pre);
    lcd.setCursor(11, 1);
    lcd.print(stepCP1);

    lcd.setCursor(0, 2);
    lcd.print("CP2: ");
    lcd.setCursor(5, 2);
    lcd.print(stepTurn2);
    lcd.setCursor(8, 2);
    lcd.print(stepCP2);
    lcd.setCursor(11, 2);
    lcd.print(stepTurn3);
    lcd.setCursor(14, 2);
    lcd.print(stepCP3);

    lcd.setCursor(0, 3);
    lcd.print("CP3: ");
    lcd.setCursor(5, 3);
    lcd.print(stepTurn4);
    lcd.setCursor(8, 3);
    lcd.print(stepFinal);

    char opt = keypad.getKey();
    if (opt == 'C') {
      flag_s = 1;
      lcd.clear();
    }
  }
}
void cekKecepatan() {
  lcd.clear();
  while (true) {
    if (flag_s == 1) {
      flag_s = 0;
      lcd.clear();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print("C : Kembali");
    lcd.setCursor(0, 1);
    lcd.print("KI L,D : KA L,D");
    lcd.setCursor(2, 2);
    lcd.print(KIRLU_SPEED);
    lcd.setCursor(0, 2);
    lcd.print("- ");
    lcd.setCursor(2, 3);
    lcd.print(KIRDAL_SPEED);
    lcd.setCursor(0, 3);
    lcd.print("- ");
    lcd.setCursor(12, 2);
    lcd.print(KANLU_SPEED);
    lcd.setCursor(10, 2);
    lcd.print("- ");
    lcd.setCursor(12, 3);
    lcd.print(KANDAL_SPEED);
    lcd.setCursor(10, 3);
    lcd.print("- ");
    char opt = keypad.getKey();
    if (opt == 'C') {
      flag_s = 1;
      lcd.clear();
    }
  }
}
void setInput(byte &inputan, int &ee_inputan, String print_text) {
  lcd.clear();
  while (true) {
    if (flag_s == 1) {
      flag_s = 0;
      lcd.clear();
      break;
    }
    lcd.setCursor(0, 0);
    lcd.print(print_text);
    lcd.setCursor(0, 1);
    lcd.print(": ");
    lcd.setCursor(0, 3);
    lcd.print("C : Btl  * : Del");
    lcd.setCursor(0, 2);
    lcd.print("         # : OK");

    char inp = keypad.getKey();
    if (inp == 'C') {
      flag_s = 1;
      pos = 2;
      a = 0;
      data[0] = data[1] = data[2] = NULL;
      lcd.clear();
    }
    else if (inp == '*') {
      pos = 2;
      a = 0;
      data[0] = data[1] = data[2] = NULL;
      lcd.setCursor(0, 1);
      lcd.print(":     ");
    }
    else if (inp == '#') {
      if (data[0] != NULL && data[1] == NULL && data[2] == NULL) {
        inputan = (byte) (data[0] - 48);
        EEPROM.write(ee_inputan, inputan);
        pos = 2;
        a = 0;
        data[0] = data[1] =  data[2] = NULL;
        flag_s = 1;
      }
      else if (data[0] != NULL && data[1] != NULL && data[2] == NULL) {
        inputan = (byte) ((data[0] - 48) * 10) + (data[1] - 48);
        EEPROM.write(ee_inputan, inputan);
        pos = 2;
        a = 0;
        data[0] = data[1] =  data[2] = NULL;
        flag_s = 1;
      }
      else if (data[0] != NULL && data[1] != NULL && data[2] != NULL) {
        inputan = (byte) constrain((((data[0] - 48) * 100) + ((data[1] - 48) * 10) + (data[2] - 48)), 0, 255);
        EEPROM.write(ee_inputan, inputan);
        pos = 2;
        a = 0;
        data[0] = data[1] =  data[2] = NULL;
        flag_s = 1;
      }
      else if (data[0] == NULL && data[1] == NULL && data[2] == NULL) {
        pos = 2;
        a = 0;
        data[0] = data[1] =  data[2] = NULL;
        flag_s = 1;
      }
    }
    else if (inp) {
      data[a] = inp;
      lcd.setCursor(pos, 1);
      lcd.print(data[a]);

      if (pos >= 4) pos = 4;
      else pos++;
      if (a >= 3) a = 3;
      else a++;
    }
  }
}
void buttonCheck() {
  int reading = digitalRead(buttonPin);
  if (reading == HIGH) {
    flag_Robot = 1;
    EEPROM.write(ee_flag_robot, 1);
  }
  else if (reading == LOW) {
    flag_Robot = 0;
    EEPROM.write(ee_flag_robot, 0);
  }
}
void kalibrasi() {

  bacaSensor();

  if (bacaPutihKIRLU == 1) {
    digitalWrite(KIRLU1, HIGH);
    digitalWrite(KIRLU6, HIGH);
    analogWrite(KIRLU2, KIRLU_SPEED);
    flag_A = 1;
  }
  if (bacaPutihKANLU == 1) {
    digitalWrite(KANLU1, HIGH);
    digitalWrite(KANLU6, HIGH);
    analogWrite(KANLU2, KANLU_SPEED);
    flag_B = 1;
  }
  if (bacaPutihKIRDAL == 1) {
    analogWrite(KIRDAL1, KIRDAL_SPEED);
    analogWrite(KIRDAL2, KIRDAL_SPEED);
    flag_C = 1;
  }
  if (bacaPutihKANDAL == 1) {
    analogWrite(KANDAL1, KANDAL_SPEED);
    analogWrite(KANDAL2, KANDAL_SPEED);
    flag_D = 1;
  }
}
void kalibrasiHitam() {

  bacaSensor();

  if (bacaHitamKIRLU == 0) {
    digitalWrite(KIRLU1, HIGH);
    digitalWrite(KIRLU6, HIGH);
    analogWrite(KIRLU2, KIRLU_SPEED);
    flag_A = 1;
  }
  if (bacaHitamKANLU == 0) {
    digitalWrite(KANLU1, HIGH);
    digitalWrite(KANLU6, HIGH);
    analogWrite(KANLU2, KANLU_SPEED);
    flag_B = 1;
  }
  if (bacaHitamKIRDAL == 0) {
    analogWrite(KIRDAL1, KIRDAL_SPEED);
    analogWrite(KIRDAL2, KIRDAL_SPEED);
    flag_C = 1;
  }
  if (bacaHitamKANDAL == 0) {
    analogWrite(KANDAL1, KANDAL_SPEED);
    analogWrite(KANDAL2, KANDAL_SPEED);
    flag_D = 1;
  }
}
void bacaSensor() {
  bacaPutihKIRLU  = digitalRead(PUTIH_SENS_KIRLU);
  bacaPutihKANLU  = digitalRead(PUTIH_SENS_KANLU);
  bacaPutihKIRDAL = digitalRead(PUTIH_SENS_KIRDAL);
  bacaPutihKANDAL = digitalRead(PUTIH_SENS_KANDAL);

  bacaHitamKIRLU  = digitalRead(HITAM_SENS_KIRLU);
  bacaHitamKANLU  = digitalRead(HITAM_SENS_KANLU);
  bacaHitamKIRDAL = digitalRead(HITAM_SENS_KIRDAL);
  bacaHitamKANDAL = digitalRead(HITAM_SENS_KANDAL);
}
void konfigurasiPin() {
  for (byte i = 2;   i < 8;  i++)     {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  for (byte i = 18;  i < 22; i++)   {
    pinMode(i, INPUT_PULLUP);
  }
  for (byte i = 22;  i < 30; i += 2)  {
    pinMode(i, INPUT);
  }
  for (byte i = 47;  i < 54; i += 2)  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(geregeSensorPin, INPUT);
  pinMode(mountainSensorPin, INPUT);
  pinMode(buttonPin, INPUT);
}
void speedAwal() {
  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, LOW);
  analogWrite(KIRLU6, KIRLU_SPEED);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, LOW);
  analogWrite(KANLU6, KANLU_SPEED + 20);

  analogWrite(KIRDAL1, KIRDAL_SPEED + 20);
  analogWrite(KANDAL1, KANDAL_SPEED);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, 0);
}

void speedHitam() {
  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, LOW);
  analogWrite(KIRLU6, KIRLU_SPEED);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, LOW);
  analogWrite(KANLU6, KANLU_SPEED + 10);

  analogWrite(KIRDAL1, KIRDAL_SPEED - 60);
  analogWrite(KANDAL1, KANDAL_SPEED - 80);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, 0);
}
void speedPutih() {
  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, LOW);
  analogWrite(KIRLU6, KIRLU_SPEED - 115);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, LOW);
  analogWrite(KANLU6, KANLU_SPEED - 125);

  analogWrite(KIRDAL1, KIRDAL_SPEED + 10);
  analogWrite(KANDAL1, KANDAL_SPEED);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, 0);
}
void speedHitamMount() {
  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, LOW);
  analogWrite(KIRLU6, KIRLU_SPEED + 10);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, LOW);
  analogWrite(KANLU6, KANLU_SPEED + 20);

  analogWrite(KIRDAL1, KIRDAL_SPEED - 50);
  analogWrite(KANDAL1, KANDAL_SPEED - 70);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, 0);
}
void speedPutihMount() {
  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, LOW);
  analogWrite(KIRLU6, KIRLU_SPEED - (115 - 10));

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, LOW);
  analogWrite(KANLU6, KANLU_SPEED - (125 - 10));

  analogWrite(KIRDAL1, KIRDAL_SPEED + 20);
  analogWrite(KANDAL1, KANDAL_SPEED + 10);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, 0);
}
void speedKanan() {
  digitalWrite(KIRLU1, LOW);
  digitalWrite(KIRLU2, HIGH);
  analogWrite(KIRLU6, KIRLU_SPEED);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, HIGH);
  analogWrite(KANLU6, KANLU_SPEED);

  analogWrite(KIRDAL1, KIRDAL_SPEED);
  analogWrite(KANDAL1, KANDAL_SPEED);
  analogWrite(KIRDAL2, KIRDAL_SPEED);
  analogWrite(KANDAL2, 0);
}
void speedKiri() {

  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, HIGH);
  analogWrite(KIRLU6, KIRLU_SPEED);

  digitalWrite(KANLU1, LOW);
  digitalWrite(KANLU2, HIGH);
  analogWrite(KANLU6, KANLU_SPEED);

  analogWrite(KIRDAL1, KIRDAL_SPEED);
  analogWrite(KANDAL1, KANDAL_SPEED);
  analogWrite(KIRDAL2, 0);
  analogWrite(KANDAL2, KANDAL_SPEED);
}
void diam() {
  digitalWrite(KIRLU1, HIGH);
  digitalWrite(KIRLU2, HIGH);
  analogWrite(KIRLU6, KIRLU_SPEED);

  digitalWrite(KANLU1, HIGH);
  digitalWrite(KANLU2, HIGH);
  analogWrite(KANLU6, KIRLU_SPEED);

  analogWrite(KIRDAL1, KIRDAL_SPEED);
  analogWrite(KANDAL1, KANDAL_SPEED);
  analogWrite(KIRDAL2, KIRDAL_SPEED);
  analogWrite(KANDAL2, KANDAL_SPEED);
}
void jalanMode_A() {
  while (true) {
    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahA(0);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaHitamKIRLU == 0)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaHitamKANLU == 0)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaHitamKIRDAL == 0)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaHitamKANDAL == 0) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);

    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahA(1);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaPutihKIRLU == 1)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaPutihKANLU == 1)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaPutihKIRDAL == 1)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaPutihKANDAL == 1) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);
lbl_bottom:
    if (flag_Break == 1) {
      flag_Break = 0;
      flag_gerege_comp = 1;
      break;
    }
  }
}
void jalanMode_B() {
  while (true) {
    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahB(0);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaHitamKIRLU == 0)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaHitamKANLU == 0)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaHitamKIRDAL == 0)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaHitamKANDAL == 0) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);

    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahB(1);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaPutihKIRLU == 1)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaPutihKANLU == 1)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaPutihKIRDAL == 1)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaPutihKANDAL == 1) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);
lbl_bottom:
    if (flag_Break == 1) {
      flag_Break = 0;
      flag_gerege_comp = 1;
      break;
    }
  }
}
void jalanMode_A_() {
  while (true) {
    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahA(0);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaHitamKIRLU == 0)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaHitamKANLU == 0)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaHitamKIRDAL == 0)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaHitamKANDAL == 0) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);

    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahA(1);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaPutihKIRLU == 1)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaPutihKANLU == 1)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaPutihKIRDAL == 1)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaPutihKANDAL == 1) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);
lbl_bottom:
    if (flag_Break == 1) {
      flag_Break = 0;
      break;
    }
  }
}
void jalanMode_B_() {
  while (true) {
    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahB(0);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaHitamKIRLU == 0)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaHitamKANLU == 0)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaHitamKIRDAL == 0)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaHitamKANDAL == 0) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);

    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    hitunglangkahB(1);
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaPutihKIRLU == 1)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaPutihKANLU == 1)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaPutihKIRDAL == 1)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaPutihKANDAL == 1) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        stepCount++;
        break;
      }
    }
    delay(700);
lbl_bottom:
    if (flag_Break == 1) {
      flag_Break = 0;
      break;
    }
  }
}
void jalanLurus() {
  while (true) {
    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    speedHitam();
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();

      if (bacaHitamKIRLU == 0)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaHitamKANLU == 0)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaHitamKIRDAL == 0)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaHitamKANDAL == 0) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        break;
      }
    }
    delay(700);
    interPin();
    if (flag_Break == 1) {
      goto lbl_bottom;
    }
    speedPutih();
    while (true) {
      interPin();
      if (flag_Break == 1) {
        goto lbl_bottom;
      }
      bacaSensor();
      if (bacaPutihKIRLU == 1)   {
        digitalWrite(KIRLU1, HIGH);
        digitalWrite(KIRLU2, HIGH);
        analogWrite(KIRLU6, KIRLU_SPEED);
        flag_A = 1;
      }
      if (bacaPutihKANLU == 1)   {
        digitalWrite(KANLU1, HIGH);
        digitalWrite(KANLU2, HIGH);
        analogWrite(KANLU6, KANLU_SPEED);
        flag_B = 1;
      }
      if (bacaPutihKIRDAL == 1)  {
        analogWrite(KIRDAL1, KIRDAL_SPEED);
        analogWrite(KIRDAL2, KIRDAL_SPEED);
        flag_C = 1;
      }
      if (bacaPutihKANDAL == 1) {
        analogWrite(KANDAL1, KANDAL_SPEED);
        analogWrite(KANDAL2, KANDAL_SPEED);
        flag_D = 1;
      }
      if (flag_A == 1 && flag_B == 1 && flag_C == 1 && flag_D == 1) {

        flag_A = 0;
        flag_B = 0;
        flag_C = 0;
        flag_D = 0;
        break;
      }
    }
    delay(700);
lbl_bottom:
    if (flag_Break == 1) {
      flag_Break = 0;
      lcd.clear();
      break;
    }
  }
}
void hitunglangkahA(bool warna) {
  if (stepCount == stepStart + 1 ||
      stepCount == stepStart + stepTurn1 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + 1) {
    delay(1100);
  }
  if (warna == 0) {
    if (stepCount <= stepStart) {

      EEPROM.write(ee_rst, 'A');
      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1) {

      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre) {

      EEPROM.write(ee_rst, 'B');
      speedAwal();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1) {

      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2) {

      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2) {

      EEPROM.write(ee_rst, 'C');
      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3) {

      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3) {

      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4) {

      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + stepFinal) {

      EEPROM.write(ee_rst, 'D');
      speedHitamMount();
    }
    else {
      serv1.write(180);
      serv2.write(0);
      diam();
    }
  }
  if (warna == 1) {
    if (stepCount <= stepStart) {

      EEPROM.write(ee_rst, 'A');
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1) {

      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre) {

      EEPROM.write(ee_rst, 'B');
      speedAwal();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1) {

      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2) {
      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2) {
      EEPROM.write(ee_rst, 'C');
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3) {
      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3) {
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4) {
      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + stepFinal) {
      EEPROM.write(ee_rst, 'D');
      speedPutihMount();
    }
    else {
      serv1.write(180);
      serv2.write(0);
      diam();
    }
  }
}
void hitunglangkahB(bool warna) {
  if (stepCount == stepStart + 1 ||
      stepCount == stepStart + stepTurn1 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + 1 ||
      stepCount == stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + 1) {
    delay(1100);
  }
  if (warna == 0) {
    if (stepCount <= stepStart) {
      EEPROM.write(ee_rst, 'A');
      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1) {
      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre) {
      EEPROM.write(ee_rst, 'B');
      speedAwal();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1) {
      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2) {
      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2) {
      EEPROM.write(ee_rst, 'C');
      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3) {
      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3) {
      speedHitam();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4) {
      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + stepFinal) {
      EEPROM.write(ee_rst, 'D');
      speedHitamMount();
    }
    else {
      serv1.write(180);
      serv2.write(0);
      diam();
    }
  }
  if (warna == 1) {
    if (stepCount <= stepStart) {
      EEPROM.write(ee_rst, 'A');
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1) {
      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre) {
      EEPROM.write(ee_rst, 'B');
      speedAwal();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1) {
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2) {
      speedKanan();
      flag_C = 1;
      flag_B = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2) {
      EEPROM.write(ee_rst, 'C');
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3) {
      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3) {
      speedPutih();
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4) {
      speedKiri();
      flag_A = 1;
      flag_D = 1;
    }
    else if (stepCount <= stepStart + stepTurn1 + stepCP1pre + stepCP1 + stepTurn2 + stepCP2 + stepTurn3 + stepCP3 + stepTurn4 + stepFinal) {
      EEPROM.write(ee_rst, 'D');
      speedPutihMount();
    }
    else {
      serv1.write(180);
      serv2.write(0);
      diam();
    }
  }
}
void interPin() {
  buttonCheck();
  if (flag_Robot == 0) {
    flag_Break = 1;
  }
}
