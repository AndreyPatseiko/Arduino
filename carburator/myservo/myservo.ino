/*
 * При первой установке нужно - либо установить параметр setupServo = true, или передать через терминал команду setup: true|  
 * При первоначальной настройке.
 * 
 * Привод ХХ
 * нужно подключить сервопривод и установить угол 90 - это будет нулевой точкой (уже прописано в setup: true)
 * Диапазон регулировки от 180 до 15 градусов. Рабочий диапазон от 100 до 55. 
 * Если значяение меньше 55 то может проскочить через ограничитель!
 *  
 * Привод Воздушной заслонки.
 * При servoSetup = true, устанавилвается в положение 0. При этом нужно перевести серво в режим полной открытой 
 * воздушной заслонки.
 *  
 * Регулировка параметров нужно использовать. 
 * c: (0-50) |        угол воздушной завслонки
 * h: ((100-180)-55)| угол подпора ХХ
 * setup: true|       переключение в режим настройки\работы. При каждом вызове setup: true| - уставливает сервоприводы в необходимые положения.
 * wt: int |          изменить время прогревочного цикла ( параметр в мск) 
 * 
*/

#include <Servo.h>                    // Библилтека работы с серво приводом
Servo sHh; 
Servo sChoke; 
int hhPin = 9;                        // пин для управления двигателем ХХ  - 9
int chokePin = 10;                    // пин для управления двигателем воздушной заслонки - 10
int enginPin = 4;                     // пин для получения сигнала что ключь в зажигании
int enginRunPin = 5;                  // пин для получения сигнала что двигатель заведен

float minServoHhDeg = 55;             // XX - максимальные прогревочные
float maxServoHhDeg = 100;            // XX - прогревочные выключены
float sHhDeg = minServoHhDeg;         // текуший угол прогревочного серво
float sHhOnePercDeg = (maxServoHhDeg - minServoHhDeg) / 100; // угол одно процента 

float minServoChokeDeg = 0;           // Воздушная заслонка - полностью открыта
float maxServoChokeDeg = 50;          // Воздушная заслонка - польностью закрыта
float sChokeDeg = maxServoChokeDeg;   // текщий угол воздушной заслонки
float sChokeOnePercDeg = (maxServoChokeDeg - minServoChokeDeg) / 100; // угол одно процента 

boolean setupServo = false;           // режин настройки привода
boolean enginIsHot = false;           // индикатор прогрева двигателя
boolean engin = false;                                   
boolean enginRun = false;

String serialString; // для чтения данных их буффера

// Чтение сопротивления датчика темперетуры
int analogPin = 0; // пин для подключения датчика температуры
int raw = 0;
int Vin = 5; // опорное напряжение
float Vout = 0;
float R1 = 1000; // сопротивление известного резистора
float R2 = 0; // сопротивление измеряемого резистора (нашего датчика температуры)
float buffer = 0;

unsigned long checkResistTimer = 0;
unsigned long checkResistInterval = 3000; // инервал когда нужно змерять сопротивление, через каждые 15 сек
     
//Настроки циклов для усреднненой программы
unsigned long runProgTime; // время прошедшее после старта программы
float warmUpTime = 180000; // полное время цикла в милисекундах (3мин)

float steps = 10;         // количество шагов до прогрева
float percCompleted = 0;  // сколько процентов выполннено

void setup(){    
  Serial.begin(9600);
  sHh.write(maxServoHhDeg);             
  sHh.attach(hhPin);  
  sChoke.write(minServoChokeDeg);
  sChoke.attach(chokePin);                 
  delay(1500);
  sHh.detach();            
  sChoke.detach();

}

void loop() {  
  // Чтение данные из терминала - для настройки и изменеия параметров
  while (Serial.available()) {
    char serialChar = Serial.read();
    serialString += serialChar;  
    if (serialChar == '|') {
      Serial.print("String = ");          
      serialString = serialString.substring(0, serialString.length()-1);      
      if(serialString.indexOf("h:") > -1) {        
        // изменяем угол ХХ        
        String newDeg = serialString.substring(serialString.indexOf("h:") + 1);
        sHh.write(newDeg.toInt());                         
        Serial.println("hh degree = " + serialString);    
      } else if(serialString.indexOf("c:") > -1) {         
        // изменяем угол дроселя   
        String startDeg = serialString.substring(serialString.indexOf("c:") + 1);
        sChoke.write(startDeg.toInt());  
        Serial.println("choke deg = " + serialString);                   
      } else if(serialString.indexOf("wt:") > -1) {         
        // изменяем времени цикла прогрева если вышел из строя датчик   
        String setTime = serialString.substring(serialString.indexOf("wt:") + 1);
        warmUpTime = setTime.toInt();  
        Serial.println("warmUpTime = " + setTime);                   
      } else if(serialString.indexOf("setup:") > -1) {
        // переключение режима работы
        String startDeg = serialString.substring(serialString.indexOf("setup:") + 1);   
        if(startDeg == "true"){
          setupServo = true;
          servoToInstallPosition();     
          Serial.println("Setup enabled");    
        } else {
          setupServo = false;
          Serial.println("Setup disabled");
        }             
         
      }
      serialString = "";      
    }
  }

  engin = digitalRead(enginPin);
  enginRun = digitalRead(enginRunPin);  
  
  
    
  if(!setupServo ) { // рабочий цикл    
    // Чтение данные датчика температуры
    raw = analogRead(analogPin);
    if(raw && isCheckResistance() && !enginIsHot) {
      buffer = raw * Vin;
      Vout = (buffer)/1024.0;
      buffer = (Vin/Vout) - 1;
      R2 = R1 * buffer;    
      if ( Vout <= .1 ) { // не исправен датчик температуры - запуск усреднной программы
        runAverageValues();       
      } else {
        
      }
    Serial.print("engin D4: ");
    Serial.print(engin);
    Serial.print(" enginRun D5: ");
    Serial.println(enginRun);
      
//         Serial.print("Vout: ");
//         Serial.print(Vout, 4);
//         Serial.print(" R2: ");
//         Serial.println(R2);      
    }
  }
}

void servoToInstallPosition(){
  sHh.write(90);             
  sHh.attach(hhPin);  
  
  sChoke.write(0);
  sChoke.attach(chokePin);    
            
  delay(1500);
  sHh.detach();            
  sChoke.detach();
}

bool isCheckResistance(){
  if(millis() > checkResistTimer + checkResistInterval){
    checkResistTimer = millis();
    return true;
  } else {
    return false;
  }
}

void runAverageValues(){ // если датчик не работате стартуем по усреднным значениям  
  if( millis() > warmUpTime ) { // Прогрелось
    finish();
  } else { // нужно прогревать    
    float now = millis();    
    float proc = 100 * (now / warmUpTime); // сколько процентов выплонилось   
    // измнеие положения серво машинок при измнеий на каждые 10%    
    if(proc > percCompleted + steps && proc < 100) {
      percCompleted += steps;
      sChokeDeg = maxServoChokeDeg - ( percCompleted * sChokeOnePercDeg );
      sHhDeg = minServoHhDeg + ( percCompleted * sHhOnePercDeg);      
      sHh.write(sHhDeg);  
      sChoke.write(sChokeDeg); 
      if( !sHh.attached() ) {
        sHh.attach(hhPin);
      }
      
      if( !sChoke.attached() ) {
        sChoke.attach(chokePin);
      }
      
      delay(1500);      
      Serial.print(" sChokeDeg ");
      Serial.print(sChokeDeg);
       Serial.print(" sHhDeg ");
      Serial.println(sHhDeg);

      
      Serial.print("Процент выполнения программы ");
      Serial.print(percCompleted);
      Serial.println(" %");
      
    } else if(proc == 100){
      finish();
    }
  
  }
  
}

void finish(){
  sHh.write(maxServoHhDeg);  // полностью открываем воздушную заслонку;
  sChoke.write(minServoChokeDeg);  // Выключаем прогревочные;  
  enginIsHot = true;
    
  if( !sHh.attached() ) {
    sHh.attach(hhPin);
  }
  if( !sChoke.attached() ) {
    sChoke.attach(chokePin);
  }
      
  delay(1500);
  
  sChoke.detach();
  sHh.detach();
  Serial.println("Я прогрелся!!!");
}





