#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <Stepper.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
void runServo(int id,int s_min, int s_max,int dir=1){
    if(dir==1){
    for(int pulselen = s_min; pulselen<=s_max;pulselen++){
      pwm.setPWM(id,0,pulselen);
    }
    }else{
      for(int pulselen = s_max; pulselen>s_min;pulselen--){
      pwm.setPWM(id,0,pulselen);
    }      
    }
  }
class FileSorter{
  int pivotPin0=0,pivotPin1=1;
  int buzzerPin=13;
  int ir0=22,ir1=23,ir3=24, ir4=25;
  int locations[2]={-1,-1};
  void init(){
    pinMode(buzzerPin,OUTPUT);
    pinMode(ir0,INPUT);
    pinMode(ir1,INPUT);
    pinMode(ir2,INPUT);
    pinMode(ir3,INPUT);

  }
  void getFile(String id){
    int x=id[0]-'0';
    int y=id[1]-'0';
    if(locations[y]==-1){
      digitalWrite(buzzerPin,HIGH);
      delay(100);
      digitalWrite(buzzerPin,LOW);
    }else{
      runServo(1,150,600,1);
      delay(200);
      runServo(locations[y],150,600,1);
    }
  }
  void close(){
    runServo(2,150,600,-1);
    delay(100);
    runServo(3,150,600,-1);
    delay(100);
    runServo(1,150,600,-1);
    delay(200);
  }
  void setFileLocations(){
    int a=digitalRead(ir0),b=digitalRead(ir1),c=digitalRead(ir2),d=digitalRead(ir3);
    if(a==1 && b==1){
      locations[0]=2;
    }
    if(a==0 && b==1){
      locations[1]=2;
    }
    if(c==1 && d==1) locations[0]=3;
    if(c==0 && d==1) locations[1]=3;
  }
};

class JuiceMachine{
   int icePin=5,juicePin=4;
   int buzzerPin=13;
   int ir0=26,ir1=27;
   void init(){
     pinMode(buzzerPin,OUTPUT);
     pinMode(ir0,INPUT);
     pinMode(ir1,INPUT);
   }
   int checkCup(int id){
     if(id==0){
       if(ir0==0){
         digitalWrite(buzzerPin,HIGH);
         delay(100);
         digitalWrite(buzzerPin,LOW);
         return 0;
       }else{
         return 1;
       }
     }else{
       if(ir1==0){
         digitalWrite(buzzerPin,HIGH);
         delay(100);
         digitalWrite(buzzerPin,LOW);
         return 0;
       }else{
         return 1;
       }
     }
   }
   void getJuice(){
     if(checkCup(0)){
       runServo(juicePin,150,600);
       delay(400);
       runServo(juicePin,150,600,-1);
     }
   }
   void getIce(){
     if(checkCup(1)){
       runServo(icePin,150,600);
       delay(400);
       runServo(icePin,150,600,-1);
     }
   }
};
class PillBox{
  Stepper pillStepper(2048,8,9,10,11);
  void getPill(int delay_amm){
    delay(delay_amm);
    pillStepper.step(512);
  }
};
class PrayerMat{
  int servoPin=6;
  void init(){
    pinMode(serbvoPin(6));
  }
  void pullDown(){
    runServo(servoPin,150,600,1);
    delay(100);
  }
  void pullUp(){
    runServo(servoPin,150,600,-1);
    delay(100);
  }

};
FileSorter fs;
JuiceMachine jm;
PillBox pb;
PrayerMat pm;
class Controller{
  int btns[]={28,29,30,31,32,33};
  int states[]={0,0,0,0,0,0};
  void init(){
    for(int i=0;i<6;i++){
      pinMode(btns[i],INPUT);
    }
  }
  void run(){
    if(btns[0]){
      if(states[0]){
        pm.pullUp();
      }else{
        pm.pullDown();
      }
      states[0]=!states[0]
    }
    if(btns[1]){
      pb.getPill(300);
    }
    if(btns[2] && btns[3]){
      fs.getFile("11");
    }
    if(!btns[2] && btns[3]){
      fs.getFile("01")
    }

  }
}
Controller cr;
void setup(){
  Serial.begin(9600);
  fs.init();
  jm.init();
  pb.init();
  pm.init();
  cr.init();
}

void loop(){
  cr.run();
}