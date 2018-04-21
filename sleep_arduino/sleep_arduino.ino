
#include <avr/sleep.h>  
  
int wakePin = 2;                 // pin used for waking up  
int led=13;  
  
void wakeUpNow() {  
  // execute code here after wake-up before returning to the loop() function  
  // timers and code using timers (serial.print and more...) will not work here.  
  // we don't really need to execute any special functions here, since we  
  // just want the thing to wake up  
}  
  
void setup() {  
  pinMode(wakePin, INPUT_PULLUP);  
  pinMode(led, OUTPUT);   
  attachInterrupt(0, wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function wakeUpNow when pin 2 gets LOW  
}  
  
void sleepNow() {  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here  
    sleep_enable();          // enables the sleep bit in the mcucr register  
    attachInterrupt(0,wakeUpNow, LOW); // use interrupt 0 (pin 2) and run function  
    sleep_mode();            // here the device is actually put to sleep!!  
    // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP  
    sleep_disable();         // first thing after waking from sleep: disable sleep...  
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the wakeUpNow code will not be executed during normal running time.  
}  
  
void loop() {  
  digitalWrite(led, HIGH);  
  delay(1000);  
  digitalWrite(led, LOW);  
  sleepNow();     // sleep function called here  
} 
