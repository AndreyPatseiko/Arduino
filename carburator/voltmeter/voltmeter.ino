float temp;
 
void setup() 
{   
 Serial.begin(9600);   
} 
 
void loop() 
{       
 temp=analogRead(A0)/40.92;        
 Serial.print(temp);
 Serial.println("V"); 
 
 delay(1000); 
}
