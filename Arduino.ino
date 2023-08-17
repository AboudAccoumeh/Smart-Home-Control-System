#include <Servo.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

struct Door {
  unsigned long prevTime = 99999999999999999999;
  int interpolation = -999;
  float duration;
  int startingAngle;//when door is closed
  int endingAngle;//when door is opened
  bool atStart = true;//state of the door
  bool oldAtStart = true;//old state of the door
  int pin;
  Servo servo;
};
Door door1;

int waterValue = 0;
int waterPin = 12;
int waterAnalogPin = A5;
unsigned long waterPrevTime = 0;
unsigned long waterDelay = 1000;

SoftwareSerial CommunicationSerial(11,10);
unsigned long srPrevTime = 0;
int srDelay = 200;

const String password = "123";
const byte ROWS = 4;
const byte COLS = 4;
String userInput = "";
const char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13,8,7,6};
byte colPins[COLS] = {5,4,9,2};
Keypad myKeypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

void setup() {
  Serial.begin(9600);
  CommunicationSerial.begin(19200);
  
  pinMode(waterPin, OUTPUT);
  digitalWrite(waterPin, LOW);

  door1.duration = 1000;
  door1.startingAngle = 0;
  door1.endingAngle = 180;
  door1.pin = 3;
  door1.servo.attach(door1.pin);
  door1.servo.write(door1.startingAngle);
  delay(800);
}

void loop() {
  unsigned long currMillis = millis();
  if(currMillis - srPrevTime >= srDelay){
    srPrevTime = currMillis;
    CommunicationSerial.print("userInput:"+userInput+",");
    CommunicationSerial.print("humidity:"+String(int((float(waterValue)/208)*100))+",");
    CommunicationSerial.print("door1:"+String(door1.atStart)+",");
    if (CommunicationSerial.available()>0) {
      String message = CommunicationSerial.readStringUntil(',');
      message.trim(); // Remove leading and trailing whitespaces
      int colonIndex = message.indexOf(":"); // Find the index of the colon character
      String part1 = message.substring(0, colonIndex);
      String part2 = message.substring(colonIndex + 1);
      Serial.println(part1+" "+part2);
      if(part1 == "userInputFromWebsite"){
        userInput = part2;
        if(door1.atStart == true && userInput == password){
          door1.atStart = false;
          userInput = "";
        }else if(door1.atStart == false){
          door1.atStart = !door1.atStart;
        }
      }else if (part1 == "door1Close"){
        door1.atStart = true;
      }
    }
  }
  
  char pressedKey = myKeypad.getKey();
  if(pressedKey){
    unsigned long currMillis = millis(); 
    if(pressedKey == 'D'){//Delete user input
      userInput = "";
    }else if(pressedKey == 'C'){// close or open door
      if(currMillis - door1.prevTime >= door1.duration){
        if(door1.atStart == true && userInput == password){
           door1.atStart = !door1.atStart;
           userInput = "";
        }else if(door1.atStart == false){
           door1.atStart = !door1.atStart;
        }
      }
    }else{
      userInput += pressedKey;
    }
    Serial.println(userInput);
  }
  if(currMillis-waterPrevTime >= waterDelay){
    digitalWrite(waterPin, HIGH);  // turn the sensor ON
    delay(10);// wait 10 milliseconds
    waterValue = analogRead(waterAnalogPin); // read the analog value from sensor
    digitalWrite(waterPin, LOW);   // turn the sensor OFF

    //Serial.print("Sensor value: ");
    //Serial.println(waterValue);
    waterPrevTime = currMillis;
  }
  currMillis = millis();
  if (currMillis - door1.prevTime >= door1.duration && door1.atStart && !door1.oldAtStart) {//close
    Serial.println("i want to close the door1");
    door1.oldAtStart = door1.atStart = true;
    door1.prevTime = currMillis;
    door1.interpolation = door1.endingAngle;
  }
  if (currMillis - door1.prevTime >= door1.duration && !door1.atStart && door1.oldAtStart) {//open
    Serial.println("i want to open the door1");
    door1.oldAtStart = door1.atStart = false;
    door1.prevTime = currMillis;
    door1.interpolation = door1.startingAngle;
  }
  if(door1.interpolation == door1.startingAngle){//open
    float progress = float(currMillis - door1.prevTime) / door1.duration;
    int targetPosition = door1.startingAngle + int(float(door1.endingAngle - door1.startingAngle) * progress);
    targetPosition = constrain(targetPosition, door1.startingAngle, door1.endingAngle);
    door1.servo.write(targetPosition);
    if(progress >= 1.0){
      door1.interpolation = -999;
      door1.servo.write(door1.endingAngle);
    }
  }else if(door1.interpolation == door1.endingAngle){//close
    float progress = float(currMillis - door1.prevTime) / door1.duration;
    int targetPosition = door1.endingAngle + int(float(door1.startingAngle - door1.endingAngle) * progress);
    targetPosition = constrain(targetPosition, door1.startingAngle, door1.endingAngle);
    door1.servo.write(targetPosition);
    if(progress >= 1.0){
      door1.interpolation = -999;
      door1.servo.write(door1.startingAngle);
    }
  }
}