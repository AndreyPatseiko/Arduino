/* Params   
 *  initMinDeg: [munber]|  - уставливает начальный угол ( полностью открытая заслонка)
 *  initMaxDeg: [munber]|  - уставливает конечный угол ( полностью закрытая заслонка)
 *  run|                   - запускает выполнение программы с новыми настройками
 *  initMinDeg: 0 | initMaxDeg: 30 | - изменеие срезу нескольктх параметров
 *  После изенеий истанвливется на тот угол который был уставлен.
 */

#include <Servo.h>                    // Библилтека работы с серво приводом
Servo servoDrive; 

String serialString;
char serialChar;

boolean setupServo = true;            // режин настройки привода
float minServoDeg = 0;
float maxServoDeg = 55;                
char buffer;

void setup() {
  Serial.begin(9600);
//  servoDrive.attach(52);              // init pin where conncted servo
  servoDrive.write(minServoDeg);
}

void loop(){  
  int count;  
    
  while (Serial.available()) {   
    char serialChar = Serial.read();
    serialString += serialChar;  
    if (serialChar == '|') {
      Serial.print("String = ");
      Serial.println(serialString);
      serialString = serialString.substring(0, serialString.length()-1);    
      if(serialString.indexOf("initMinDeg:") > -1) {        
        // можно изменить начальный угол поворота        
        String startDeg = serialString.substring(serialString.indexOf('initMinDeg:') + 1);
        initServoAngle(startDeg, true);          
      } else if(serialString.indexOf("initMaxDeg:") > -1) {         
        // изменяем конечный угол поворота       
        String startDeg = serialString.substring(serialString.indexOf('initMaxDeg:') + 1);
        initServoAngle(startDeg, false);          
      } else if(serialString.indexOf('show') > -1) {
         Serial.print("max: " + String(maxServoDeg));
         Serial.println("min: " + String(minServoDeg));
      }
      serialString = "";        
    }
  }
 
//    test move valve
  for (int i = minServoDeg; i <= maxServoDeg; i+= 10){       
    servoDrive.attach(52);
    servoDrive.write(i);
    delay(2000);    
    servoDrive.detach();
    delay(2000);
  }

  
}

void moveValve(){
  
}

void initServoAngle(String value, boolean type) { // true - min, false - max
  float deg = value.toFloat(); 
  if(type){   
    minServoDeg = deg;   
    Serial.println("Set min deg = " + String(minServoDeg) + "deg"); 
  } else {
    maxServoDeg = deg;     
    Serial.println("Set max deg = " + String(maxServoDeg) + "deg"); 
  }
}

