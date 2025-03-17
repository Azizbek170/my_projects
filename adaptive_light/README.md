# Adaptive Light (Arduino)
Проект адаптивного освещения с изменяемой частотой мигания светодиода.

## Оборудование:
- **Arduino Uno**
- **Фоторезистор**
- **Резистор 10кОм**
- **Светодиод**

## Как это работает?
1. Фоторезистор измеряет уровень освещённости.
2. Если света мало → светодиод горит постоянно.
3. Если света много → светодиод начинает мигать с увеличивающейся задержкой (до 500 мс), затем сбрасывается.

## Код:
```cpp
int delayP = 100;

void setup() {
    pinMode(9, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int lightlevel = analogRead(A0);
    
    if (lightlevel < 800) {
        digitalWrite(9, HIGH);
    } else {
        digitalWrite(9, HIGH);
        delay(delayP);
        digitalWrite(9, LOW);
        delay(delayP);
        
        delayP = delayP + 50;
        if (delayP == 500) {
            delayP = 100;
        }
    }
    
    delay(300);
}
