#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50
#define IR_PIN 32

#define BASE_SERVO 0
#define JOINT1_LEFT 1
#define JOINT1_RIGHT 2
#define JOINT2 3
#define GRIPPER_ROTATE 4
#define GRIPPER_OPEN_CLOSE 5

#define TOTAL_TITIK 3

int defaultPosition[TOTAL_TITIK][5] = {
    {535, 425, 350, 290, 500},
    {475, 425, 350, 230, 500},
    {415, 425, 350, 180, 500}};

int posisiAwal[TOTAL_TITIK][5] = {
    {535, 495, 270, 290, 400},
    {475, 495, 270, 230, 400},
    {410, 495, 270, 180, 400}};

int posisiAkhir[TOTAL_TITIK][5] = {
    {105, 495, 270, 290, 500},
    {170, 495, 270, 350, 500},
    {230, 495, 270, 400, 500}};

int currentTitik = 0;

float correctedDistance = 0;
bool objectDetected = false;
bool objectGripped = false;

void approachObject(int index);
void moveToTarget(int index);
void releaseObject();
void ServoMove(uint8_t servoPin, int startPulse, int endPulse, bool mirror = false, uint8_t mirrorPin = 0);
void backtozero();
void defaultposition(int index);
void ServoGrip(uint8_t servoPin, int startPulse);
void reset();
void resetposition();

int stage1 = 0;
int stage2 = 0;
int stage3 = 0;
int stage4 = 0;

int temp = 0;

int pulse1 = 535;
int pulse23 = 425;
int pulse3 = 350;
int pulse4 = 290;
int pulse5 = 500;

int count = 0;
int resetval = 0;

void setup()
{
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  if (!lox.begin())
  {
    Serial.println("Sensor VL53L0X gagal diinisialisasi.");
    while (1)
      ;
  }

  pinMode(IR_PIN, INPUT);

  stage1 = 0;
  stage2 = 0;
  stage3 = 0;
  stage4 = 0;
  temp = 0;

  defaultposition(currentTitik);
}

void loop()
{
  Serial.print(resetval);

  objectDetected = (digitalRead(IR_PIN) == LOW);

  defaultposition(currentTitik);

  if (objectDetected)
  {
    approachObject(currentTitik);
  }

  if (objectGripped)
  {
    backtozero();
    moveToTarget(currentTitik);
    releaseObject();
    backtozero();
    reset();

    currentTitik++;
    if (currentTitik >= TOTAL_TITIK)
    {
      currentTitik = 0;
      count = 0;
    }
    delay(10);
  }

  if (!objectDetected)
  {
    resetval++;
    delay(100);
  }

  if (resetval > 50)
  {
    currentTitik = 0;
    resetposition();
    resetval = 0;
  }
}

void defaultposition(int index)
{
  if (stage1 == 0 && stage2 == 0 && stage3 == 0 && stage4 == 0)
  {
    ServoMove(BASE_SERVO, pulse1, defaultPosition[index][0]);
    ServoMove(JOINT1_LEFT, pulse23, defaultPosition[index][1], true, JOINT1_RIGHT);
    ServoMove(JOINT2, pulse3, defaultPosition[index][2]);
    ServoMove(GRIPPER_ROTATE, pulse4, defaultPosition[index][3]);
    ServoMove(GRIPPER_OPEN_CLOSE, pulse5, defaultPosition[index][4]);

    pulse1 = defaultPosition[index][0];
    pulse23 = defaultPosition[index][1];
    pulse3 = defaultPosition[index][2];
    pulse4 = defaultPosition[index][3];
    pulse5 = defaultPosition[index][4];

    stage1 = 1;
  }
}

void approachObject(int index)
{
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4)
  {

    uint16_t rawDistance = measure.RangeMilliMeter;
    correctedDistance = 0.9697 * rawDistance - 34.13975;
    Serial.print("Distance: ");
    Serial.println(correctedDistance);

    if (stage1 == 1 && stage2 == 0 && stage3 == 0 && stage4 == 0 && correctedDistance > 60 && count < 30 && objectGripped == false && temp == 0)
    {

      // Serial.print("Stage 1: Approach to point ");
      // Serial.println(index + 1);

      ServoMove(BASE_SERVO, pulse1, posisiAwal[index][0]);
      ServoMove(JOINT2, pulse3, posisiAwal[index][2]);
      ServoMove(JOINT1_LEFT, pulse23, posisiAwal[index][1], true, JOINT1_RIGHT);
      ServoMove(GRIPPER_ROTATE, pulse4, posisiAwal[index][3]);
      temp = 1;
    }
    if (stage1 == 1 && stage2 == 0 && stage3 == 0 && stage4 == 0 && correctedDistance < 60 && correctedDistance >= 15 && pulse5 > 100 && count < 30 && temp == 1)
    {
      pwm.setPWM(GRIPPER_OPEN_CLOSE, 0, pulse5);
      // Serial.println("Object close enough. Stopping slow grip.");
    }
    else if (correctedDistance < 15 && count < 30)
    {
      count++;
    }
    else if (count >= 30)
    {
      ServoGrip(GRIPPER_OPEN_CLOSE, pulse5);

      objectGripped = true;
      stage2 = 1;
    }

    pulse5--;
  }
  pulse1 = posisiAwal[index][0];
  pulse23 = posisiAwal[index][1];
  pulse3 = posisiAwal[index][2];
  pulse4 = posisiAwal[index][3];
}

void backtozero()
{
  ServoMove(JOINT1_LEFT, pulse23, 380, true, JOINT1_RIGHT);
  ServoMove(JOINT2, pulse3, 350);
  ServoMove(GRIPPER_ROTATE, pulse4, 290);

  pulse23 = 380;
  pulse3 = 350;
  pulse4 = 290;
  stage3 = 1;
}

void moveToTarget(int index)
{
  if (stage3 == 1 && stage4 == 0 && stage1 == 1 && stage2 == 1)
  {
    // Serial.print("Stage 3: Move to target ");
    // Serial.println(index + 1);

    ServoMove(BASE_SERVO, pulse1, posisiAkhir[index][0]);
    ServoMove(GRIPPER_ROTATE, pulse4, posisiAkhir[index][3]);
    ServoMove(JOINT2, pulse3, posisiAkhir[index][2]);
    ServoMove(JOINT1_LEFT, pulse23, posisiAkhir[index][1], true, JOINT1_RIGHT);
    stage4 = 1;
  }

  pulse1 = posisiAkhir[index][0];
  pulse23 = posisiAkhir[index][1];
  pulse3 = posisiAkhir[index][2];
  pulse4 = posisiAkhir[index][3];
}

void releaseObject()
{
  // Serial.println("Stage 4: Release object");
  // Serial.println(stage4);

  for (pulse5; pulse5 <= 550; pulse5++)
  {
    pwm.setPWM(GRIPPER_OPEN_CLOSE, 0, pulse5);
    delay(20);
  }
}

void ServoMove(uint8_t servoPin, int startPulse, int endPulse, bool mirror, uint8_t mirrorPin)
{
  int step = (startPulse < endPulse) ? 1 : -1;
  int pulse;
  for (pulse = startPulse; pulse != endPulse + step; pulse += step)
  {
    pwm.setPWM(servoPin, 0, pulse);
    if (mirror)
    {
      pwm.setPWM(mirrorPin, 0, 626 - pulse);
    }
    delay(10);
  }
}

void ServoGrip(uint8_t servoPin, int startPulse)
{
  // Serial.println("Grip");
  int pulse = startPulse;
  unsigned long startTime = millis();

  while (millis() - startTime < 3000)
  {
    pwm.setPWM(GRIPPER_OPEN_CLOSE, 0, pulse);
    pulse--;
    delay(10);
  }
}

void reset()
{
  stage1 = 0;
  stage2 = 0;
  stage3 = 0;
  stage4 = 0;
  count = 0;
  objectDetected = false;
  objectGripped = false;
  temp = 0;
  pulse5 = 550;
  resetval = 0;
}

void resetposition()
{
  ServoMove(BASE_SERVO, pulse1, 535);
  ServoMove(GRIPPER_ROTATE, pulse4, 290);
  ServoMove(JOINT1_LEFT, pulse23, 425, true, JOINT1_RIGHT);
  ServoMove(JOINT2, pulse3, 350);
  currentTitik = 0;
  pulse1 = 535;
  pulse23 = 425;
  pulse3 = 350;
  pulse4 = 290;
  pulse5 = 500;
}