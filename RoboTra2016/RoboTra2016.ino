#include <Servo.h>
#include <MsTimer2.h>
#include <TimerOne.h>
#include "fanctions.h"
#include "switch.h"



void setup(){
  pinMode(trigPinA,OUTPUT);
  pinMode(echoPinA,INPUT);
  pinMode(trigPinB,OUTPUT);
  pinMode(echoPinB,INPUT);
  pinMode(LineSensorDig1,INPUT);
  pinMode(LineSensorDig3,INPUT);
  pinMode(LineSensorDig5,INPUT);

  /*
  pinMode(A0,INPUT_PULLUP);//ロータリースイッチの端子1と接続
   pinMode(1,INPUT_PULLUP);//ロータリースイッチの端子2と接続
   pinMode(0,INPUT_PULLUP);//ロータリースイッチの端子4と接続#
   
   */
//10msで割り込み。MsTimer2を使うとなぜか３番ピンからPWM出力ができなくなった
  MsTimer2::set(10,speedmeter);
  MsTimer2::start();

  //  Timer1.initialize(100);//100usで割り込み
  //  Timer1.attachInterrupt(flash);
 //以下二つはピン変化割り込み。
 //ピンの状態がHIGH->LOWあるいはLOW->HIGHになったときに指定された関数を実行する。  
  attachInterrupt(0,flashL,RISING);//pin2
  attachInterrupt(1,flashR,RISING);//pin3
  Serial.begin(9600);

}



void loop(){
  
    line_sensor();

//    drive(100,100,FRONT,FRONT);

    driveDistance(2000,2000,FRONT,FRONT);
    delay(1000);
    driveDistance(2000,2000,BACK,BACK);
    delay(1000);


/*  Serial.print(LSD1);
  Serial.print("\t");
  Serial.print(LSA2);
  Serial.print("\t");
  Serial.print(LSD3);
  Serial.print("\t");
  Serial.print(LSA4);
  Serial.print("\t");
  Serial.print(LSD5);
  Serial.print("\t");
  Serial.print("\t");*/



//  sendSerial();


    /*
  switch(situation){//何回目のT字か
  //1,3,5,7,9でそれぞれ1,2,3,4,5回目
  //読みずらいので要修正かと
  case 1://1回目
    driveDistance(1000,1000,BACK,FRONT);
    while(LSD3){
      LSD3=PINC & _BV(3)&&8;
      drive(50,50,BACK,FRONT);
    }
    while(T==0){//T字を検知するまで
    line_trace(BACK);
    }
    situation++;
    break;

  case 3://2回目
      driveDistance(1000,1000,BACK,BACK);
      delay(5000);
      driveDistance(1200,1200,FRONT,FRONT);
      situation++;
    break;

  case 5://3
    drive(0,0,BACK,BACK);

    countL_old=countL;
    while(abs(countL-countL_old)<200){
      LSD1=PINC & _BV(1)&&2;
      Serial.print(countL-countL_old);
      drive(30,0,FRONT,FRONT);
    }
    drive(0,0,FRONT,FRONT);

    while(LSD3){
      LSD3=PINC & _BV(3)&&8;
      drive(30,0,FRONT,FRONT);
    }
    situation++;
    break;

  case 7://4回目
    driveDistance(100,100,FRONT,FRONT);
    forest();//障害物除け
    situation++;
    break;

  case 9:
    while(T==0){
    line_trace(FRONT);
    }
    countL_old=countL;
    while(abs(countL-countL_old)<200){
      LSD1=PINC & _BV(1)&&2;
      Serial.print(countL-countL_old);
      drive(30,0,FRONT,FRONT);
    }
    drive(0,0,FRONT,FRONT);

    while(LSD3){
      LSD3=PINC & _BV(3)&&8;
      drive(30,0,FRONT,FRONT);
    }
    countR_old=countR;
    while(countR-countR_old<7000){//要調整
      line_trace(FRONT);
    }
    servo(HIGH);
    delay(5000);
    servo(LOW);
    
    situation++;  
  break;
  
  default:
    line_trace(FRONT);
    break;
  }
  */

/*    
  driveDistance(200,200,FRONT,FRONT);
  drive(0,0,FRONT,FRONT);
  delay(2000);
  */
}



void   sendSerial(){
}


