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
#define CLK 17
#define SEL 16
#define CMD 15
#define DATA 14
#define SERVO_OPEN 90
#define SERVO_CLOSE 180

#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial blue(A3, A2);
Servo gripper;

char data;
bool mode;

void gerak(bool mkir1, bool mkir2, bool mkan1, bool mkan2, int speed_kiri, int speed_kanan) {
  digitalWrite(PIN_MOTOR_KIRI_1, mkir1);
  digitalWrite(PIN_MOTOR_KIRI_2, mkir2);
  digitalWrite(PIN_MOTOR_KANAN_1, mkan1);
  digitalWrite(PIN_MOTOR_KANAN_2, mkan2);
  analogWrite(PWM_KIRI, speed_kiri);
  analogWrite(PWM_KANAN, speed_kanan);
}
void diam() {
  gerak(1, 1, 1, 1, NORMAL_SPEED, NORMAL_SPEED);
}
void jalan(int spee) {
  if (data == 'F') gerak(1, 0, 1, 0, spee, spee);
  else if (data == 'G') gerak(1, 0, 1, 1, spee, spee);
  else if (data == 'I') gerak(1, 1, 1, 0, spee, spee);
  else if (data == 'H') gerak(0, 1, 1, 1, spee, spee);
  else if (data == 'J') gerak(1, 1, 0, 1, spee, spee);
  else if (data == 'B') gerak(0, 1, 0, 1, spee, spee);
  else if (data == 'L') gerak(1, 0, 0, 1, spee, spee);
  else if (data == 'R') gerak(0, 1, 1, 0, spee, spee);
  else if (data == 'W') servo(1);
  else if (data == 'w') servo(0);
  else diam();
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

  blue.begin(9600);
  Serial.begin(9600);

  while (!Serial);
  gripper.attach(GRIPPER_PIN);
  diam();
  servo(0);
}
void loop() {
  if (blue.available() > 0) data = blue.read();
  if (data == 'X') mode = 1;
  if (data == 'x') mode = 0;
  (mode == 1) ? jalan(MAX_SPEED) : jalan(NORMAL_SPEED);
  delay(15);
}
