#include <Servo.h>

//SERVO
Servo myservo;
int pos = 0;
//SERVO
//LDR AND LED's
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int ldrPin = A0;
//LDR AND LED's
//PRESSURE SENSOR
int Pin1 = A6;
int Pin2 = A2;
int Pin3 = A3;
//ULTRASONIC SENSOR
int trigPin = 5;
int echoPin = 4;
int maxDistance = 0;
//ULTRASONIC SENSOR
//FLAP
int flagFlap = 0;
//FLAP
//BLUETOOOTH
#define max_char 13
String r_char;               // reads each character
int index = 0;            // defines the position into your array
String redTempValue;
String greenTempValue;
String blueTempValue;
char currentColor;
int PressureRed = 0;
int PressureGreen = 0;
int PressureBlue = 0; 

//BLUETOOTH
void setup() 
{
  //for LED's
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin, OUTPUT);
  //for LED's
  //for UltraSonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(Pin1, HIGH);
  digitalWrite(Pin2, HIGH);
  digitalWrite(Pin3, HIGH);
  //
  //Servo Motor
  Serial.begin(9600);
  myservo.attach(3);
  for (int i = 0; i < 50 ;i++){
    maxDistance += (ultra())/50;
    delay(100);
  }
  Serial.println(maxDistance);
  digitalWrite(Pin1, HIGH);
  digitalWrite(Pin2, HIGH);
  digitalWrite(Pin3, HIGH);
  PressureRed = analogRead(Pin1);
  PressureGreen = analogRead(Pin2);
  PressureBlue = analogRead(Pin3); 
  Serial.println(PressureRed);
  Serial.println(PressureGreen);
  Serial.println(PressureBlue);
  motorOff();
}

void setColor(int red, int green, int blue){
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

int ultra() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.017;
  Serial.println(distance);
  return distance;
}
int automatic(){
  float brightness = analogRead(ldrPin);
  float value;
  if (brightness > 400){
  value = (brightness/1024) * 255;
  }
  else {
    value = 0; 
  }
  return value; 
}

void motorOn()
{
  for (int i = (myservo.read()/10)*10; i <= 180; i += 10){
      myservo.write(i);
      delay(100);
    }      
}
void motorOff()
{
  for (int i = (myservo.read()/10)*10; i >= 90; i -= 10){
      myservo.write(i);
      delay(100);
    }      
}

void loop()
{
    if (analogRead(ldrPin) < 500 && flagFlap == 1 && Serial.available() <= 0)
    {
      flagFlap = 0;
      setColor(0,0,0);
      motorOff();
      
    }

    if (analogRead(ldrPin) > 600 && flagFlap == 0 && ultra() < maxDistance-50)
    {
      flagFlap = 1;
      motorOn();
     
    }
    if (Serial.available()>0)
    {
      while (Serial.available() > 0)
      {
        
        r_char = Serial.readString();
        char Mode = r_char.charAt(0);
        int len = r_char.length();
        if (Mode == 'L')
        {
            int R = r_char.indexOf('R');
            int G = r_char.indexOf('G');
            int B = r_char.indexOf('B');
            String redTempValue = r_char.substring(R+1,G);
            String greenTempValue = r_char.substring(G+1,B);
            String blueTempValue = r_char.substring(B+1);
            int v1 = redTempValue.toInt();
            int v2 = greenTempValue.toInt();
            int v3 = blueTempValue.toInt();
            analogWrite(redPin, v1);
            analogWrite(greenPin, v2);
            analogWrite(bluePin, v3);
            
            delay(4000);            
        }
      
        else if (Mode == 'P')
        {
          setColor(0,0,0);
          while(Serial.read()!='S')
          {
            int Red = abs((PressureRed - analogRead(Pin1)/6));
            int Green = abs((PressureGreen - analogRead(Pin1)/6));
            int Blue = abs((PressureBlue - analogRead(Pin1)/2));
            setColor(Red, Green, Blue);
            delay(1000);
            setColor(0,0,0);
          }
        }

        
        else if (Mode == 'D')
        {
          char Disco = r_char.charAt(1);
          if (Disco == '1')
          {
            while(Serial.read() != 'S')
            {
              int redVal = 255;
              int blueVal = 0;
              int greenVal = 0;
              for( int i = 0 ; i < 255 ; i += 1 )
              {
                greenVal += 1;
                redVal -= 1;
                analogWrite(redPin, redVal);
                analogWrite(greenPin, greenVal);
                analogWrite(bluePin, blueVal);
                delay(10);
              }
 
              redVal = 0;
              blueVal = 0;
              greenVal = 255;
              for( int i = 0 ; i < 255 ; i += 1 )
              {
                blueVal += 1;
                greenVal -= 1;
                analogWrite(redPin, redVal);
                analogWrite(greenPin, greenVal);
                analogWrite(bluePin, blueVal);
                delay(10);
              }
 
        redVal = 0;
        blueVal = 255;
        greenVal = 0;
        for( int i = 0 ; i < 255 ; i += 1 )
        {
          redVal += 1;
          blueVal -= 1;
          analogWrite(redPin, redVal);
          analogWrite(greenPin, greenVal);
          analogWrite(bluePin, blueVal);
          delay(10);
        }
        
      }
      }
      else if (Disco == '2')
      {
        while(Serial.read() != 'S')
        {
        int a = rand() % 256;
        int b = rand() % 256;
        int c = rand() % 256;
        analogWrite(redPin, a);
        analogWrite(greenPin, b);
        analogWrite(bluePin, c);
        delay(1000);
      }
      }
    }
  }
  }
  if(flagFlap==1 && Serial.available() <= 0)
    {
      Serial.println(automatic());
      setColor(automatic(),automatic(),automatic());
    
    }
    delay(1000);
}

