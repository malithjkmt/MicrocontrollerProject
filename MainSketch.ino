#include<AFMotor.h>
#define DEBUG true

AF_DCMotor motor1(1,MOTOR12_1KHZ);
AF_DCMotor motor2(2,MOTOR12_1KHZ);
AF_DCMotor motor3(3,MOTOR34_1KHZ);
AF_DCMotor motor4(4,MOTOR34_1KHZ);

//Sonar control pins 
//int trigPinL=36;
//int echPinL=37;
//int trigPinR=43;
//int echPinR=42;

int morterSpeed;
 
void setup() 
{

    Serial.begin(9600);     // communication with the host computer
    pinMode(22,OUTPUT);
    pinMode(23,OUTPUT);
    digitalWrite(22,LOW);
    digitalWrite(23,LOW);

    //Power for the Sonar modules
//    pinMode(52,OUTPUT);
//    pinMode(53,OUTPUT);
//    pinMode(30,OUTPUT);
//    pinMode(31,OUTPUT);
//
//    digitalWrite(52,LOW); //GND Andower for sonars
//    digitalWrite(53,LOW);
//    digitalWrite(30,HIGH);
//    digitalWrite(31,HIGH);
//
//    //controls from sonars
//    pinMode(trigPinL,OUTPUT);
//    pinMode(echPinL,INPUT);
//    pinMode(trigPinR,OUTPUT);
//    pinMode(echPinR,INPUT); 
   
    // Start the software serial for communication with the ESP8266
    Serial2.begin(115200);  

    setMortorSpeed(255);//Set moter speed
 
    sendData("AT+RST\r\n",2000,DEBUG);
    //sendData("AT+CIPSTA=\"192.168.137.138\"\r\n",2000,DEBUG);
    sendData("AT+CWJAP=\"Malith\",\"qqqqqqqq\"\r\n",10000,DEBUG);
    sendData("AT+CIPSTA=\"192.168.137.138\"\r\n",2000,DEBUG);
    sendData("AT+CWMODE=1\r\n",2000,DEBUG);
    
    sendData("AT+CIPMUX=1\r\n",2000,DEBUG);
    sendData("AT+CIFSR\r\n",2000,DEBUG);
    sendData("AT+CIPSERVER=1,1336\r\n",2000,DEBUG);  
    Serial.println("Server is ready");
}
 
void loop() 
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( Serial2.available()>0 )   {  
      String s = Serial2.readStringUntil('\n');
      Serial.println(s); 
      
      if(s.substring(0,15) == "WIFI DISCONNECT"){
         stopMortors();
         Serial.println("motores stoped");
         digitalWrite(22,LOW);
         
      }
         
  
      if(s.substring(9,11) == "cc"){
        char cmd = s[11];
        Serial.println(cmd);
        switch(cmd){
          case '0':
            Serial.println("stop");
            stopMortors();
            break;
          case '1':
            Serial.println("forward");
            moveForward();
            break;
          case '2':
            Serial.println("left");
            turnLeft();
            break;
          case '3':
            Serial.println("right");
            turnRight();
            break;
          case '4':
            Serial.println("back");
            moveBackward();
            break;
          case 's':
            Serial.println("Speed-Slow");
            setMortorSpeed(180);
            break;
          case 'm':
            Serial.println("Speed-Mediam");
            setMortorSpeed(200);
            break;
          case 'f':
            Serial.println("Speed-Fast");
            setMortorSpeed(255);
            break;
          default:
            Serial.println(cmd);
            break;
              
          
        }
      }
      
      }

  
    // listen for user input and send it to the ESP8266
    if ( Serial.available() )       {
      //Serial2.print("AT+CIPSEND=0,8\r\n");
      
       sendData("AT+CIPSEND=0,8\r\n",2000,DEBUG);
       String response = "";
     while(Serial.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial.read(); // read the next character.
        response+=c;
      } 
      response += "\r\n";
      Serial2.print(response);
    }

    //check forobstacals
    //checkObstacal();
     
}
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial2.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial2.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = Serial2.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
        if(response.length()>53){
        if(response.substring(31,45)=="WIFI CONNECTED"){
          
          Serial.println("---Connected------");
          digitalWrite(22,HIGH);
        }
      }
    }
    
    return response;
}

String sendDataClient(String message)
{
  Serial2.print("AT+CIPSEND=0,8\r\n");
  delay(1000);
  Serial2.print(message);
}


void setMortorSpeed(int val){
  int m1s=val-((val*2)/10);
  motor1.setSpeed(m1s);// set the speed to 200/255
  motor2.setSpeed(val);// set the speed to 200/255
  motor3.setSpeed(val);// set the speed to 200/255
  motor4.setSpeed(val);// set the speed to 200/255
}

void moveForward(){
  motor1.run(FORWARD);
  motor3.run(FORWARD);
  motor2.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward(){
  motor1.run(BACKWARD);
  motor3.run(BACKWARD);
  motor2.run(BACKWARD);
  motor4.run(BACKWARD);
}

void stopMortors(){
  motor1.run(RELEASE);
  motor3.run(RELEASE);
  motor2.run(RELEASE);
  motor4.run(RELEASE);
}

void turnRight(){
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void turnLeft(){
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}


bool checkObstacal(){
//  long dl;
//  long dr;
//
//  digitalWrite(trigPinL, LOW);
//  delayMicroseconds(5);
//  digitalWrite(trigPinL, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPinL, LOW);
//
//  pinMode(echPinL, INPUT);
//  dl = pulseIn(echPinL, HIGH);
//
//  Serial.println("dl : "+dl);
//
//  digitalWrite(trigPinR, LOW);
//  delayMicroseconds(5);
//  digitalWrite(trigPinR, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trigPinR, LOW);
//
//  pinMode(echPinR, INPUT);
//  dr = pulseIn(echPinR, HIGH);
//
//  Serial.println("dl : "+dr);
  
}

