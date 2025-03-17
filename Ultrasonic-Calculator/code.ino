#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Пины ультразвукового датчика
#define TRIG_PIN 11
#define ECHO_PIN 10

// Настройки дисплея LCD 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Размерность клавиатуры
const int ROW_NUM = 4;
const int COLUMN_NUM = 4;

// Определение клавиш
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Пины для клавиатуры
byte rowPins[ROW_NUM] = {9, 8, 7, 6};
byte colPins[COLUMN_NUM] = {5, 4, 3, 2};

// Объект клавиатуры
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROW_NUM, COLUMN_NUM);

// Переменные для калькулятора
String input = "";
int num1 = 0, num2 = 0;
char operation = 0;
bool enteringSecondNum = false;
bool showUltrasonic = false;

// Переменные для датчика расстояния
float duration, distance;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  showMainMenu();
}

void loop() {
  char key = keypad.getKey();
  
  if (showUltrasonic) {
    updateUltrasonic();
  }

  if (key) {
    handleKeyPress(key);
  }
}

// Отображение главного меню
void showMainMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1:Ult  2:Calc");
  lcd.setCursor(0, 1);
  lcd.print("*:Back  #:=");
}

// Обновление данных с ультразвукового датчика
void updateUltrasonic() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 58.0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  delay(500);
}

// Обработка нажатий клавиш
void handleKeyPress(char key) {
  if (key == '*') {  // Сброс калькулятора
    resetCalculator();
    showMainMenu();
  } else if (key == '#') {  // Вычисление
    calculate();
  } else if (key == '1') {  // Включение ультразвукового датчика
    showUltrasonic = true;
  } else if (key == '2') {  // Отключение ультразвукового датчика
    showUltrasonic = false;
    lcd.clear();
  } else {
    processCalculatorInput(key);
  }
}

// Обновление экрана калькулятора
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(num1);
  if (operation) {
    lcd.print(" ");
    lcd.print(getOperatorSymbol(operation));
  }
  lcd.print(" ");
  lcd.print(input);

  lcd.setCursor(0, 1);
  lcd.print("*:C  #:=");
}

// Обработка ввода чисел и операций в калькуляторе
void processCalculatorInput(char key) {
  if (key >= '0' && key <= '9') {
    input += key;
  } else if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    if (!enteringSecondNum && !input.isEmpty()) {
      num1 = input.toInt();
      operation = key;
      input = "";
      enteringSecondNum = true;
    }
  }
  updateLCD();
}

// Вычисление результата
void calculate() {
  if (enteringSecondNum && !input.isEmpty()) {
    num2 = input.toInt();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(num1);
    lcd.print(" ");
    lcd.print(getOperatorSymbol(operation));
    lcd.print(" ");
    lcd.print(num2);

    lcd.setCursor(0, 1);
    lcd.print("= ");
    
    switch (operation) {
      case 'A': lcd.print(num1 + num2); break;
      case 'B': lcd.print(num1 - num2); break;
      case 'C': lcd.print(num1 * num2); break;
      case 'D': 
        if (num2 != 0) lcd.print(num1 / num2);
        else lcd.print("Err");
        break;
    }

    delay(2000);
    resetCalculator();
  }
}

// Сброс калькулятора
void resetCalculator() {
  num1 = 0;
  num2 = 0;
  operation = 0;
  input = "";
  enteringSecondNum = false;
  showUltrasonic = false;
  showMainMenu();
}

// Получение символа операции
char getOperatorSymbol(char op) {
  switch (op) {
    case 'A': return '+';
    case 'B': return '-';
    case 'C': return '*';
    case 'D': return '/';
    default: return ' ';
  }
}
