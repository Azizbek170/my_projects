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
