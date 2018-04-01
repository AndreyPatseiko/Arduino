/*
 * Для калибровки сервопривода воздушной заслонки 
 * нужно подключить сервопривод и установить угол 90 - это будет нулевой точкой
 * Диапазон регулировки от 180 до 15 градусов. Рабочий диапазон от 100 до 55. 
 * Если значяение меньше 55 то может проскочить через ограничитель!
 * 
 * Для регулировки нужно использовать. 
 * c: (0-50) |  угол дросельной завслонки
 * h: ((100-180)-55)| угол подпора ХХ
*/

#include <Servo.h>                    // Библилтека работы с серво приводом
Servo sHh; 
Servo sChoke; 

int signal
int hhPin = 9;                        // пин для управления серво ХХ
int chokePin = 10;                    // пин для управления серво воздушной заслонки

int val = 0;
float minServoDeg = 55;
float maxServoDeg = 100;  
boolean setupServo = true;            // режин настройки привода

String serialString; // для чтения данных их буффера

int analogPin= 0;
int raw= 0;
int Vin= 5;
float Vout= 0;
float R1= 1000;
float R2= 0;
float buffer= 0;



void setup(){
  
Serial.begin(9600);
Serial.println("Start!");
sHh.attach(hhPin);              // init pin where conncted servo
sHh.write(90);
sChoke.attach(chokePin);            
sChoke.write(0);
}

void loop(){

while (Serial.available()) {      // читаем данные из терминала - для настройки
    char serialChar = Serial.read();
    serialString += serialChar;  
    if (serialChar == '|') {
      Serial.print("String = ");          
      serialString = serialString.substring(0, serialString.length()-1);      
      if(serialString.indexOf("h:") > -1) {        
        // изменяем угол ХХ        
        String newDeg = serialString.substring(serialString.indexOf('h:') + 1);
        sHh.write(newDeg.toInt());  
                       
        Serial.println("hh degree = " + serialString);    
      } else if(serialString.indexOf("c:") > -1) {         
        // изменяем угол дроселя   
        String startDeg = serialString.substring(serialString.indexOf('c:') + 1);
        sChoke.write(startDeg.toInt());  
        Serial.println("choke deg = " + serialString);                   
      }        
      serialString = "";      
    }
  }

//  read data from termo sensor
  raw = analogRead(analogPin);
  if(raw) {
    buffer= raw * Vin;
    Vout= (buffer)/1024.0;
    buffer= (Vin/Vout) - 1;
    R2= R1 * buffer;
    Serial.print("Vout: ");
    Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2);
    delay(1000);
  }
 

//  servoDrive.attach(9);
//  servoDrive.write(0);
//  delay(5000);    
//  servoDrive.detach();
//  delay(5000);

}
