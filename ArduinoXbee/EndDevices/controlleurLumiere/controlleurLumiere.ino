#include <DynamixelSerial.h>

#define ID_1 1
#define ID_2 10
#define LED 13

unsigned char c;
unsigned char data[5];

int progress = 0;
int success = 0;
int received = 0;

void ouvertureMoteur(int id){
   Dynamixel.ledStatus(id,ON);
   Dynamixel.moveSpeed(id,400,400);
   //Dynamixel.move(id,400);
   delay(1000);
   Dynamixel.ledStatus(id,OFF);
} 

void fermetureMoteur(int id){
   Dynamixel.ledStatus(id,ON);
   Dynamixel.moveSpeed(id,800,400);
   delay(1000);
   Dynamixel.ledStatus(id,OFF);
}


void setup(){
    Dynamixel.begin(1000000,2);
    Serial1.begin(4800);
    delay(1000);
    //Dynamixel.setEndless(ID_1,OFF);
    //Dynamixel.setEndless(ID_2,OFF);
}


void allumageLumiere(){
   ouvertureMoteur(ID_1);
   delay(1000);
   fermetureMoteur(ID_1);
   delay(1000); 
}

void eteindreLumiere(){
   ouvertureMoteur(ID_2);
   delay(1000);
   fermetureMoteur(ID_2);
   delay(1000); 
}

void loop(){
    while(success == 0){
        if(Serial1.available() > 0){
            c = (char)Serial1.read();
            if(c == '['){
               progress = 1;
               digitalWrite(LED,HIGH);
            }
            else if(c != ']' && progress == 1){
               data[received] = c;
               received++;
            }
            else if(c == ']' && progress == 1){
               progress = 0;
               success = 1;
               received = 0;
               digitalWrite(LED,LOW);
            }

            delay(100);
        }
    }
    if (data[0] == 0x41){
        if(data[2] == 0x31){ allumageLumiere(); }
        if(data[2] == 0x30){ eteindreLumiere(); }
        delay(100);
  
    }
    success = 0;
    delay(100);
}


