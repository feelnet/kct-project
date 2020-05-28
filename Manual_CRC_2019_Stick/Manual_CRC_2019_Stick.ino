/*
   CRC 2019
   -----------------------------------
   Type         : Manual Robot
   Author       : Arson Marianus
   Last Update  : Sep/02/2019, 07:42
   -----------------------------------
*/

#define PIN_MOTOR_KIRI_1 13
#define PIN_MOTOR_KIRI_2 12
#define PWM_KIRI 5
#define PIN_MOTOR_KANAN_1 11
#define PIN_MOTOR_KANAN_2 10
#define PWM_KANAN 6
#define GRIPPER_PIN 9
#define MAX_SPEED 100
#define NORMAL_SPEED 60
#define NO_SPEED 0
#define CLK A3
#define SEL A2
#define CMD A1
#define DATA A0
#define SERVO_OPEN 90
#define SERVO_CLOSE 180

#include <PS2X_lib.h>
#include <Servo.h>

PS2X psx;
Servo gripper;

void gerak(bool mkir1, bool mkir2, bool mkan1, bool mkan2, int speed_kiri, int speed_kanan) {
  digitalWrite(PIN_MOTOR_KIRI_1, mkir1);
  digitalWrite(PIN_MOTOR_KIRI_2, mkir2);
  digitalWrite(PIN_MOTOR_KANAN_1, mkan1);
  digitalWrite(PIN_MOTOR_KANAN_2, mkan2);
  analogWrite(PWM_KIRI, speed_kiri);
  analogWrite(PWM_KANAN, speed_kanan);
}
void diam() {
  gerak(1,1,1,1, NORMAL_SPEED, NORMAL_SPEED);
}
void servo(bool kondisi) {
  (kondisi == 1) ? gripper.write(SERVO_CLOSE) : gripper.write(SERVO_OPEN);
}
void setup() {
  pinMode(PIN_MOTOR_KIRI_1, OUTPUT);
  pinMode(PIN_MOTOR_KIRI_2, OUTPUT);
  pinMode(PIN_MOTOR_KANAN_1, OUTPUT);
  pinMode(PIN_MOTOR_KANAN_2, OUTPUT);
  pinMode(PWM_KIRI, OUTPUT);
  pinMode(PWM_KANAN, OUTPUT);

  gripper.attach(GRIPPER_PIN);
  diam();
  servo(0);
  
  
  while(true) {
    int error = psx.config_gamepad(CLK, CMD, SEL, DATA, false, false);
    if(error == 0) {
      delay(500);
      break;
    }
    delay(500);
  }
}
void loop() {
  psx.read_gamepad(false, false);
  if(psx.Button(PSB_PAD_UP) && !psx.Button(PSB_R1)) gerak(1,0,1,0, NORMAL_SPEED, NORMAL_SPEED);
  else if(psx.Button(PSB_PAD_UP) && psx.Button(PSB_R1)) gerak(1,0,1,0, MAX_SPEED, MAX_SPEED);
  else if(psx.Button(PSB_PAD_DOWN) && !psx.Button(PSB_R1)) gerak(0,1,0,1, NORMAL_SPEED, NORMAL_SPEED);
  else if(psx.Button(PSB_PAD_DOWN) && psx.Button(PSB_R1)) gerak(0,1,0,1, MAX_SPEED, MAX_SPEED);
  else if(psx.Button(PSB_PAD_LEFT) && !psx.Button(PSB_R1)) gerak(1,0,1,1, NORMAL_SPEED, NORMAL_SPEED);
  else if(psx.Button(PSB_PAD_LEFT) && psx.Button(PSB_R1)) gerak(1,0,0,1, MAX_SPEED, MAX_SPEED);
  else if(psx.Button(PSB_PAD_RIGHT) && !psx.Button(PSB_R1)) gerak(1,1,1,0, NORMAL_SPEED, NORMAL_SPEED);
  else if(psx.Button(PSB_PAD_RIGHT) && psx.Button(PSB_R1)) gerak(0,1,1,0, MAX_SPEED, MAX_SPEED);
  else if(psx.Button(PSB_CIRCLE)) servo(1);
  else if(psx.Button(PSB_TRIANGLE)) servo(0);
  else diam();
  delay(50);
}
