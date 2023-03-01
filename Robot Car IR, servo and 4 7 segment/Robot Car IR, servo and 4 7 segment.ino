#include <IRremote.h> // Include IR Remote Library

#include <Servo.h>

#define MA 7 //pin for direction of motor A
#define MB 8 //pin for direction of motor B
#define SA 5 //pin for speed of motor A
#define SB 6 //pin for speed of motor B

IRrecv irrecv(9); // Define IR Receiver and Results Objects
decode_results results; 

unsigned long lastCode; //variable to assign the lastCode 

int latch = 4; //pin to latch
int cs = 2; //clockpin
int data = 0; //pin to pass the information
int dPins[4] = {13, 12, 11, 10};  //pins that control the digits, the go in that order, 4-3-2-1
int step=0;
unsigned long prevMillis=0;
int time=5;

Servo myservo;  // create servo object to control a servo

const byte digits[10] = {  //It indicates which pins have to be on and off to display each number
  B00111111,  // 0
  B00000110,  // 1
  B01011011,  // 2
  B01001111,  // 3
  B01100110,  // 4
  B01101101,  // 5
  B01111101,  // 6
  B0000111,  // 7
  B01111111,  // 8
  B01101111,   // 9
};

bool wait(unsigned long mill){ //funtion to wait and change the steps in the switch case
  if(millis()-prevMillis>=mill){
  step++;
  prevMillis = millis();
    }
  
}
void print_num(int w, int x, int y, int z){
  switch (step){
case 0:
//First digit
	digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(latch, LOW);
 	shiftOut(data, cs, MSBFIRST, digits[z]);
 	digitalWrite(latch, HIGH);

  //delay(5);
wait(time);
break;
case 1:
//Secon digit
	digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(latch, LOW);
 	shiftOut(data, cs, MSBFIRST, digits[y]);
 	digitalWrite(latch, HIGH);   
  
 // delay(5);

wait(time);
break;
case 2:
//Third digit
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(latch, LOW);
 	shiftOut(data, cs, MSBFIRST, digits[x]);
 	digitalWrite(latch, HIGH);
  
  //delay(5);
wait(time);
break;
case 3:
//Four digit
digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(latch, LOW);
 	shiftOut(data, cs, MSBFIRST, digits[w]);
 	digitalWrite(latch, HIGH);
   
  //delay(5);
wait(time);
break;
default:
step=0;
break;

};
} 



void move (bool a, int s1, bool b, int s2){ //general function for the car movement
	digitalWrite(MA,a); //digitalWrite in motor A the direction in accordance to the movement
	analogWrite(SA,s1); //analogWrithe the velocity for motor A
	
	digitalWrite(MB,b); //digitalWrite in motor B the direction in accordance to the movement
	analogWrite(SB,s2); //analogWrithe the velocity for motor B
}

void forward (int x){ //function for moving forward with x speed
  move(1,x,1,x); //use the move function and pass the values to the function 
}

void backward (int x){ //function for moving backwards with x speed
  move(0,x,0,x); //use the move function and pass the values to the function 
}

void stop (){ //function for stopping
  move(1,0,1,0); //any movement with 0 speed will stop the car
}

void left (int x){ //function for moving left with x speed
  move(1,x,0,x); //use the move function and pass the values to the function 
}

void right (int x){ //function for moving rigth with x speed
  move(0,x,1,x); //use the move function and pass the values to the function 
}

void leftforward (int x){ //function for moving leftforward with x speed
  move(1,x,1,x/2); //use the move function and pass the values to the function 
}

void rightforward (int x){ //function for moving rightforward with x speed
  move(1,x/2,1,x); //use the move function and pass the values to the function 
}

void setup() {
 	//Serial.begin(115200);
 	pinMode(latch, OUTPUT);
 	pinMode(cs, OUTPUT);
 	pinMode(data, OUTPUT);
 	pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT); 

  pinMode(MA,OUTPUT);
  pinMode(MB,OUTPUT);
  pinMode(SA,OUTPUT);
  pinMode(SB,OUTPUT);
  pinMode(1,OUTPUT); //this pin enables the movement of the motors
  
  irrecv.enableIRIn(); //Enable Infrared sensor

  myservo.attach(3);  // attaches the servo on pin 3 to the servo object
  
}
void loop() {
 	//Count from 0 to 9 on each digit
print_num(1,2,3,4);

myservo.write(90); 
//myservo.write(180);            

digitalWrite(1,1); //enables pin for moving the car
  
  if (irrecv.decode(&results)){
    // Print Code to debug
        Serial.println(results.value, HEX); //print in HEX 
        
        if(results.value == 0xFFFFFFFF) //FFFFFFFF is used to indicate when a value is repated
        {
          // If Repeat then use last code received
          results.value = lastCode;        
        }
        if(results.value==0xFF629D){ //move forward if the top arrow of the control is pressed
            lastCode = results.value;  //store the value of the code read in lastCode        
            forward(100); //move forward 
            myservo.write(0); 
            delay(500);
           
           
                     
           
        }

        if(results.value==0xFFA857){ //move backward if the bottom arrow of the control is pressed
            lastCode = results.value; //store the value of the code read in lastCode        
           backward(150); //move backward
           delay(250); //move backward only for 1/4 second
           myservo.write(0); 
         
                       
          
           
        }
             
        if(results.value==0xFFC23D){ //move to the right if the right arrow of the control is pressed
            lastCode = results.value; //store the value of the code read in lastCode 
               
            right(200); //move to the right
            delay(250); //move only for 1/4 secon
           
        }
        if(results.value==0xFF22DD){ //move to the left if the left arrow of the control is pressed
            lastCode = results.value;  //store the value of the code read in lastCode
           
           left(200); //move to the left
           delay(250); //move only for 1/4 second
           
        }
        
        irrecv.resume(); //continue reading the IR 

} else {
stop();  //stop if there's no reading
}
 	
}
