#include "lcd.h"
#include <inttypes.h>

LCD lcd;

void setup() 
{
  Serial.begin(9600);
  Serial.println("START"); 
}

void loop () 
{
  char str[2] = " ";
  
  while(Serial.available()<1){};
  
  str[0] = Serial.read();
  lcd.print(str);
}

