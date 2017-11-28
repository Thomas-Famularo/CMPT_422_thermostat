//Final Project
//Include the Servo Libary
#include <Servo.h>
//Include the LCD scrren Libary
#include <LiquidCrystal.h>

//Setup the Pins for the LCD Screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Setup the Pin that reads data from the thermometer
const int tempSensorPin = A0;

//Place to store the target temp
int targetTemp = 78;

//Array for averageing the Temp
float arrayCurTemp[10] = {targetTemp, targetTemp, targetTemp, targetTemp, targetTemp, targetTemp, targetTemp, targetTemp, targetTemp, targetTemp};
//Array coutner
int arrayCounter = 0;

//Place to store the current temp
float curTemp = 0;


int upTemp = 0;
int downTemp = 0;

int sampleCounter = 0;

Servo myservo;

void setup() {
  //Set up the Serial Monitor to print to
  Serial.begin(9600);
  //Set up the LCD's number of coloms and rows 
  lcd.begin(16, 2);
  //Set up the Temp UP button
  pinMode(6, INPUT);
  //Set up the Temp Down Button
  pinMode(7, INPUT);
  //Test the Servo motor on startup 
  myservo.attach(9);
  myservo.write(5);
  delay(1000);
  myservo.write(174);
  delay(1000);
  //Detach after test
  myservo.detach();
}

void loop(){
  //Setup the temperture sensor pin to read an analog signal 
  int tempSensorVal = analogRead(tempSensorPin);

  //Read the output form the temperture sensor pin and convert it to voltage
  float voltage = (tempSensorVal / 1024.0) * 5.0;

  //Convert the voltage to a temperature Celsius
  float curTemperatureC = (voltage - .5) * 100;

  //Convert the temperture Celsius to Fahrenheit
  float curTemperatureF = ((curTemperatureC * 9) / 5) + 32;

  //Process the current Temperture averageing array
  arrayCurTemp[arrayCounter] = curTemperatureF;
  arrayCounter = arrayCounter + 1;
  if(arrayCounter >= 10){
    arrayCounter = 0;
  }
  float x = 0;
  for(int i = 0; i < 10; i++){
    x = x + arrayCurTemp[i];
  }
  curTemp = x/10;
  //Print the Current Temperature
  Serial.print("CurAvgTemp: ");
  Serial.print(curTemp);

  //Process the buttons
  upTemp = digitalRead(6);
  downTemp = digitalRead(7);
  if(upTemp == HIGH){
    targetTemp = targetTemp + 1;
  }
  if(downTemp == HIGH){
    targetTemp = targetTemp - 1;
    //Min Target temp is absolute zero
    if(targetTemp < -460){
      targetTemp = -460;
    }
  }

  //Print the Target Temperature
  Serial.print(", targetTemperature: ");
  Serial.print(targetTemp);
  Serial.print('\n');

  //Once every 5 seconds run this
  if(sampleCounter == 0){
    //Attack the morot
    myservo.attach(9);
    if(curTemp < targetTemp){
      //Set the motor to 5
      myservo.write(5);
      delay(1000);
      //detach the motor
      myservo.detach();
    } else if (curTemp > targetTemp) {
      //Set the motor to 174
      myservo.write(174);
      delay(1000);
      //detach the motor
      myservo.detach();
    } else {
      if(myservo.read() == 174){
        //Set the motor to 95
        myservo.write(95);
        delay(1000);
        //detach the motor
        myservo.detach();
      } else if(myservo.read() == 5){
        //Set the motor to 85
        myservo.write(85);
        delay(1000);
        //Detach the motor
        myservo.detach();
      }
    }
    sampleCounter = sampleCounter + 1;
  } else {
    sampleCounter = sampleCounter + 1;
    if(sampleCounter > 5){
      sampleCounter = 0;
    }
  }
  
  //Clear the Screen
  lcd.clear();
  //Print the current temp to the LCD screen
  lcd.print("CurrentTemp:");
  lcd.print(curTemp);
  //Move the cursor to the second row
  lcd.setCursor(0,1);
  //Print the target temp to the LCD screen
  lcd.print("TargetTemp: ");
  lcd.print(targetTemp);
  //Padding to make the output look good
  lcd.print(".0");

  //Only Process once every second
  delay(1000);
}

