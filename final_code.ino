#include <LiquidCrystal.h>
#include <Servo.h>

// LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Servo
Servo servo;

// Ultrasonic
#define trigPin 9
#define echoPin 10

// Sensors
#define IR_PIN 7
#define PIR_PIN 8
#define TEMP_PIN 13   // Digital temp sensor

// Threshold
#define DIST_THRESHOLD 10

long duration;
int distance;

void setup() {
  lcd.begin(16, 2);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);

  servo.attach(6);
  servo.write(0);

  lcd.setCursor(0, 0);
  lcd.print("Sanitizer Ready");
  lcd.setCursor(0, 1);
  lcd.print("Place Hand");
  delay(2000);
}

void loop() {

  distance = getDistance();

  if (distance < DIST_THRESHOLD) {

    lcd.setCursor(0, 0);
    lcd.print("Detecting...   ");
    delay(500);

    int irState = digitalRead(IR_PIN);
    int pirState = digitalRead(PIR_PIN);
    int tempState = digitalRead(TEMP_PIN);

    lcd.setCursor(0, 0);
    lcd.print("Validating...  ");
    delay(500);

    // ✅ Updated validation logic
    if (pirState == HIGH && irState == LOW && tempState == HIGH) {

      dispense();

    } else {
      lcd.setCursor(0, 0);
      lcd.print("Invalid Object ");
      lcd.setCursor(0, 1);
      lcd.print("Try Again      ");
      delay(2000);
    }

  } else {
    lcd.setCursor(0, 0);
    lcd.print("Place Hand     ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  delay(300);
}


// 🔹 Distance function
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;

  return dist;
}


// 🔹 Dispense function
void dispense() {
  lcd.setCursor(0, 0);
  lcd.print("Dispensing...  ");
  lcd.setCursor(0, 1);
  lcd.print("Please Wait    ");

  servo.write(90);
  delay(1000);

  servo.write(0);
  delay(500);

  lcd.setCursor(0, 0);
  lcd.print("Thank You!     ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  delay(3000);
}