#include<SoftwareSerial.h>

#define sdpdt 3 //for turning left or right steering
#define sspdt 2 //switch for turning steering

#define mbldc 4 //pin for pwm to bldc motor
#define mspdt 5 //switch for bldc motion governance

#define lspdt 6 //activate left power window
#define ldpdt 7 //dpdt for clock and anti-clock motion of left panel 
//#define lltsw 7 //left limit switch

#define rspdt 8 //activate right power window
#define rdpdt 9 //dpdt for clock and anti-clock motion of right panel 
//#define rltsw 10 //right limit switch

#define tspdt 10 //activate top power window
#define tdpdt 11 //dpdt for clock and anti-clock motion of top panel 
//#define tltsw 1 //top limit switch

#define led 12
#define horn 13
char order; //instruction for various operation of CAR
            /* 
             * C = clockwise motion of steering 
             * A = anticlockwise motion of steering
             * F = forward movement of car
             * R = backward movement of car
             * T = to open top panel
             * D = to open right(daaye) panel
             * B = to open left(baaye) panel
             * E = to open all panels
             * H = to blow horn
             * Q = to turn the lights on
             */
int i=0;     // counter for automatic steering turn
char br;     // breaking system variable for motion of car
int val=90;  // value contains the speed of motion of car
int x=0;     // counter variable for val
int ls=0;     // counter for managing left side psnel
int us=0;     // counter for managing upperside psnel
int rs=0;     // counter for managing right side psnel
int stater=0; // wether right panel is on
int statel=0; // wether left panel is on
int statet=0; // wether top panel is on
int stateall=0;// wether all panels are on 
int alpha = 0; // if it is 0 we will have cwise else stopcwise
int beta = 0;

//functions for movement of steering and automatic turning of steering
void cwise();     // clockwise movement of steering
void stopcwise(); // stop clockwise movement of steering
void awise();     // anti-clockwise movement of steering
void stopawise(); // stop anti-clockwise movement of steering

//function for moving car forward and backward
void runf();         // forward motion of car
void runb();         // backward motion of car
void leftmoving();   // lefgt panel open while moving
void rightmoving();  // right panel open while moving
void topmoving();    // top panel open while moving

//function for opening and closing the panels
void openpaneltop();
void closepaneltop();
void openpanelleft();
void closepanelleft();
void openpanelright();
void closepanelright();
void allopen();

// on HARSH demand
void light();
void buzzer();


void setup() 
{
  Serial.begin(9600);
   
   // car motion
  pinMode(mbldc,OUTPUT);
  analogWrite(mbldc,val);
  pinMode(mspdt, OUTPUT);
   
   // steering
  pinMode(sspdt, OUTPUT);
  pinMode(sdpdt, OUTPUT);
  digitalWrite(sspdt, LOW);

   //left panel
  pinMode(lspdt,OUTPUT);
  pinMode(ldpdt,OUTPUT);
//  pinMode(lltsw,INPUT);

   //right panel
  pinMode(rspdt,OUTPUT);
  pinMode(rdpdt,OUTPUT);
//  pinMode(rltsw,INPUT);

  //top panel
  pinMode(tspdt,OUTPUT);
  pinMode(tdpdt,OUTPUT);
//  pinMode(tltsw,INPUT);

  //HARSH demand
  pinMode(led,OUTPUT);
  pinMode(horn,OUTPUT);
}


void loop() 
{
  i=0;
  if (Serial.available() > 0)
     order = Serial.read();
     
    switch(order)
    {
         // steering
       case 'C': if(alpha == 0)
                    cwise();
                    else
                    stopcwise();
                 delay(100);
                 break;
       case 'A': if(beta == 0)
       awise();
       else
       stopawise();
                 delay(100);
                 break;
                 
         // motion of car
       case 'F': runf();
                 Serial.println("forward");
                 break;         
       case 'R': runb();
                 Serial.println("backward");
                 break; 

         // top panel control
       case 'T': if(us == 0)
                  openpaneltop();
                 else  
                  closepaneltop();                
                 break;
       
        // right panel control
       case 'D': if(rs == 0)
                  openpanelright();
                 else  
                  closepanelright();                
                 break;

         // left panel control
       case 'B': if(ls == 0)
                  openpanelleft();
                 else  
                  closepanelleft();                
                 break;
       
         // left panel control
       case 'E': allopen();             
                break;   

       case 'Q': light();
                 delay(10);
                break;

       case 'H': order = 'k';
                 while(order != 'H')
                   {
                     digitalWrite(horn,HIGH);
                     if (Serial.available() > 0)
                        order = Serial.read();
                    }
                    digitalWrite(horn,LOW);
                   break;
  }
}




void light()
{
  order = 'k';
  while(order != 'Q')
   {
    digitalWrite(led,HIGH);
    if (Serial.available() > 0)
      order = Serial.read();
   }
   digitalWrite(led,LOW);
   order = 'k';
}



void buzzer()
{
  order = 'k';
  while(order != 'H')
   {
    digitalWrite(horn,HIGH);
    if (Serial.available() > 0)
      order = Serial.read();
   }
   digitalWrite(horn,LOW);
   order = 'k';
}


// run clockwise steering  
void cwise()
{
  order = 'A';
  while(order != 'C')
  {
    digitalWrite(sspdt,HIGH);
    digitalWrite(sdpdt,HIGH);
    Serial.println("clockwise for iteration ");
    Serial.print(i+1); 
    
    if (Serial.available() > 0)
       order = Serial.read();

    i++;
  }
  alpha = 1;
  digitalWrite(sdpdt,LOW);
  digitalWrite(sspdt,LOW);
} 




// stop clockwise steering motion
void stopcwise()
{
  
  order = 'A';
  while(order != 'C')
  {
    digitalWrite(sspdt,HIGH);
    digitalWrite(sdpdt,LOW);
    Serial.println("anti-clockwise for iteration");
    Serial.print(i+1);
  }
  digitalWrite(sspdt, LOW);
  i = 0;
  order = 'K';
  alpha = 0;
  
}




// run anticlockwise steering
void awise()
{
  order = 'C';
  while(order != 'A')
  {
    digitalWrite(sspdt,HIGH);
    digitalWrite(sdpdt,LOW);
    Serial.println("anti-clockwise for iteration");
    Serial.print(i+1); 
    
    if (Serial.available() > 0)
       order = Serial.read();

    i++;
  }
  order = 'K';
  beta = 1;
} 





// stop snti-clockwise steering
void stopawise()
{
  order = 'C';
  while(order != 'A')
  {
    digitalWrite(sspdt,HIGH);
    digitalWrite(sdpdt,HIGH);
    Serial.println("clockwise for iteration ");
    Serial.print(i+1);
  }
  digitalWrite(sspdt, LOW);
  i = 0;
  order = 'K';
  beta = 0;
}




// forward motion of car
void runf()
{
  digitalWrite(led,HIGH);
  digitalWrite(horn,HIGH);
  delay(500);
  digitalWrite(horn,LOW);
  delay(500);
  digitalWrite(led,LOW);
  digitalWrite(mspdt,LOW);
  x = 0;
  val = 110;
  for(;x<val;x++)
  {
    analogWrite(mbldc,x);
    delay(10);
    if(x==val-1)
      while(br != 'R')
      {
        Serial.println("forward highest");
        analogWrite(mbldc,val);
        delay(100);
         if(Serial.available()>0)
            br = Serial.read();

         char temp;
         int j=0;
          switch(br)
          {
            case 'T': if(statet == 0)
                      topmoving();
                      else
                       {
                         digitalWrite(tspdt,HIGH);          
                         digitalWrite(tdpdt,LOW);          
                         delay(3200);
                         br = 'F';
                         digitalWrite(tspdt,LOW);
                         statet =0;
                       }
                      break;
            case 'D':  if(stater == 0)
                      rightmoving();
                      else
                       {
                         digitalWrite(rspdt,HIGH);          
                         digitalWrite(rdpdt,LOW);          
                         delay(3200);
                         br = 'F';
                         digitalWrite(rspdt,LOW);
                         stater =0;
                       }
                      break;
            case 'B':  if(statel == 0)
                      leftmoving();
                      else
                       {
                         digitalWrite(lspdt,HIGH);          
                         digitalWrite(ldpdt,LOW);          
                         delay(3200);
                         br = 'F';
                         digitalWrite(lspdt,LOW);
                         statel =0;
                       }
                      break;
            case 'E': allopen();
                      stateall=1;
                      break;
            case 'C': temp = 'A';
                       while(temp != 'C')
                       {
                         digitalWrite(sspdt,HIGH);
                         digitalWrite(sdpdt,HIGH);
                         if (Serial.available() > 0)
                            temp = Serial.read();
                       }
                       temp = 'A';
                       digitalWrite(sspdt,LOW);
                           temp = 'p';
                           br = 'F';  
                       break;
            case 'A': temp = 'C';
                      while(temp != 'A')
                       {
                         digitalWrite(sspdt,HIGH);
                         digitalWrite(sdpdt,LOW);
                         if (Serial.available() > 0)
                            temp = Serial.read();
                          j++;
                       }
                       temp = 'C';
                       digitalWrite(sspdt,LOW);
                           temp = 'p';
                           br = 'F';
                        
                       break;
          }
      }
  }    
  Serial.println("forward stop");
  x = 0;
  val = 0;
  analogWrite(mbldc,val);
  br = 'P';
  order = 'K';
}

void topmoving()
{
  digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(tdpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1 
  delay(3200);
  br = 'F';
  digitalWrite(tspdt,LOW);
  statet =1;
}

void rightmoving()
{
  digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(rdpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1 
  delay(3200);
  br = 'F';
  digitalWrite(rspdt,LOW);
  stater =1;
}
void leftmoving()
{
  digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(ldpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1 
  delay(3200);
  br = 'F';
  digitalWrite(lspdt,LOW);
  statel =1;
}
void allopen()
{
  digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(tdpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1 
  delay(100);
  
  digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(ldpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1 
  delay(100);

  digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(rdpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1 
  delay(2950);

  digitalWrite(tspdt,LOW);
  delay(100);
  digitalWrite(lspdt,LOW);
  delay(100);
  digitalWrite(rspdt,LOW);
  delay(3000);

    //CLOSING ALL PANELS
  digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(tdpdt,LOW);          // DPDT becomes active and motor rotate in dir.2 
  delay(100);
  
  digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(ldpdt,LOW);          // DPDT becomes active and motor rotate in dir.2
  delay(100);

  digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
  digitalWrite(rdpdt,LOW);          // DPDT becomes active and motor rotate in dir.2 
  delay(2950);

  digitalWrite(tspdt,LOW);
  delay(100);
  digitalWrite(lspdt,LOW);
  delay(100);
  digitalWrite(rspdt,LOW);
  if(order == 'E')
  order = 'k';
}




   // backward motion of car
void runb()
{
  if(statet == 1)
    {
      digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
      digitalWrite(tdpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
      delay(3200);
      digitalWrite(tspdt,LOW);
      statet = 0;
    }

    if(statel == 1)
    {
      digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
      digitalWrite(ldpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
      delay(3200);
      digitalWrite(lspdt,LOW);
      statel = 0;
    }

    if(stater == 1)
    {
      digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
      digitalWrite(rdpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
      delay(3200);
      digitalWrite(rspdt,LOW);
      stater= 0;     
    }

    if(stateall == 1)
    {
      digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
      digitalWrite(tdpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
      delay(3200);
      digitalWrite(tspdt,LOW);
      digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
      digitalWrite(ldpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
      delay(3200);
      digitalWrite(lspdt,LOW); 
      digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
      digitalWrite(rdpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
      delay(3200);
      digitalWrite(rspdt,LOW); 
    }
    
  digitalWrite(mspdt,HIGH);
  val = 90;
  for(;x<val;x++)
  {
    analogWrite(mbldc,x);
    delay(10);
    if(x==val-1)
      while(br != 'F')
      {
        Serial.println("backward highest");
        analogWrite(mbldc,val);
        delay(1000);
         if(Serial.available()>0)
            br = Serial.read();
      }
  }
  Serial.println("backward stop");
  x = 0;
  val = 0;
  analogWrite(mbldc,val);
  br = 'P';
  order = 'K';
}




    // opening top panel
void openpaneltop()
{
  order = 'k';
  while (order != 'T') // || (digitalRead(tltsw) == HIGH))
  {
    digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
    digitalWrite(tdpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1
    
     if (Serial.available() > 0) 
         order = Serial.read();
  }
  us = 1;
  digitalWrite(tspdt,LOW);
  order = 'K';

}

void closepaneltop()
{
  order = 'k';
  while (order != 'T') // || (digitalRead(tltsw) == HIGH))
  {
    digitalWrite(tspdt,HIGH);          // Turns ON SPDT : C-NO
    digitalWrite(tdpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
    
     if (Serial.available() > 0)       
         order = Serial.read();
  }
  order = 'K';
  us = 0;
  digitalWrite(tspdt,LOW);
}





// opening left panel
void openpanelleft()
{
  order = 'k';
  while (order != 'B') // || (digitalRead(lltsw) == HIGH))
  {
    digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
    digitalWrite(ldpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1
    
     if (Serial.available() > 0) 
         order = Serial.read();
  }
  ls = 1;
  digitalWrite(lspdt,LOW);
  order = 'K';
}

void closepanelleft()
{
  order = 'k';
  while (order != 'B') 
  {
    digitalWrite(lspdt,HIGH);          // Turns ON SPDT : C-NO
    digitalWrite(ldpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
    
     if (Serial.available() > 0)       
         order = Serial.read();
  }
  order = 'K';
  ls = 0;
  digitalWrite(lspdt,LOW);
}








// opening right panel
void openpanelright()
{
  order = 'k';
  while (order != 'D') // || (digitalRead(rltsw) == HIGH))
  {
    digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
    digitalWrite(rdpdt,HIGH);          // DPDT becomes active and motor rotate in dir.1
    
     if (Serial.available() > 0) 
         order = Serial.read();
  }
  order = 'K';
  rs = 1;
  digitalWrite(rspdt,LOW);
}

void closepanelright()
{
  order = 'k';
  while (order != 'D') // || (digitalRead(rltsw) == HIGH))
  {
    digitalWrite(rspdt,HIGH);          // Turns ON SPDT : C-NO
    digitalWrite(rdpdt,LOW);          // DPDT becomes active and motor rotate in dir.1
    
     if (Serial.available() > 0)       
         order = Serial.read();
  }
  order = 'K';
  rs = 0;
  digitalWrite(rspdt,LOW);
}



// Copyright 2019, Rishabh Singhal,Harsh Saxena, Umangdeep, All rights reserved.

