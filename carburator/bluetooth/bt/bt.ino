int val;
int LED = 13;
int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  i++;
  if (Serial.available())  {
    val = Serial.read();
    if (val == '1')  {// При символе "1" включаем светодиод
      digitalWrite(LED, HIGH);
      Serial.println("ON");
    }
    if ( val == '0')  {// При символе "0" (ноль) выключаем
      digitalWrite(LED, LOW);
      Serial.println("OFF");
     }
  }
  Serial.println("{\"temperature\": "+String(i)+", \"complited\": "+String(i*0.1)+" }|");
  delay(2000);
}
