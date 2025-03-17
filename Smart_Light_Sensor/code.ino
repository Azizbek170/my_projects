#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define ledpin 13
#define trigpin 7
#define echopin 6

LiquidCrystal_I2C lcd(0x27, 16, 2);

float duration, cm;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(ledpin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  int lightlevel = analogRead(A0);

  // Управление светодиодом по уровню освещенности
  if (lightlevel < 800) {
    digitalWrite(ledpin, HIGH);
  } else {
    digitalWrite(ledpin, LOW);
  }
  Serial.println(lightlevel);

  delay(100); // Сглаживание частоты обновления

  // Измерение расстояния ультразвуковым датчиком
  digitalWrite(trigpin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH);
  cm = duration / 58.0;

  // Если расстояние меньше 30 см, показать время суток
  if (cm < 30) {
    displayTimeOfDay(lightlevel);
  }
  else {
  	lcd.clear();
  }
}

// Функция для отображения времени суток на LCD
void displayTimeOfDay(int lightlevel) {
  lcd.setCursor(0, 0);
  lcd.print("Time of day:");
  lcd.setCursor(0, 1);

  if (lightlevel < 100) {
    lcd.print(" Night     ");
    digitalWrite(ledpin, LOW);
  } else if (lightlevel >= 800 && lightlevel < 850) {
    lcd.print(" Morning ");
    digitalWrite(ledpin, LOW);
  } else if (lightlevel >= 850 && lightlevel < 900) {
    lcd.print(" Evening ");
    digitalWrite(ledpin, LOW);
  } else {
    lcd.print(" Day  ");
    digitalWrite(ledpin, HIGH);
  }
}
