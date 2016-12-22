#include "variables.h"

//モーターをPID制御で動かす関数…引数（左の目標値、右の目標値、左の向き、右の向き)
void drive(int vel_goalL,int vel_goalR,bool directionL,bool directionR){

  dvelL[prepre]=dvelL[pre];
  dvelR[prepre]=dvelR[pre];
  dvelL[pre]=dvelL[now];
  dvelR[pre]=dvelR[now];
  dvelL[now]=vel_goalL-constrain(velocityL,0,255);//関数内は値の調整
  dvelR[now]=vel_goalR-constrain(velocityR,0,255);//関数内は値の調整

  pValueL=KpL*(dvelL[now]-dvelL[pre]);
  pValueR=KpR*(dvelR[now]-dvelR[pre]);
  iValueL=KiL*dvelL[now];
  iValueR=KiR*dvelR[now];
  dValueL=KdL*((dvelL[now]-dvelL[pre])-(dvelL[pre]-dvelL[prepre]));
  dValueR=KdR*((dvelR[now]-dvelR[pre])-(dvelR[pre]-dvelR[prepre]));

  dMVL=pValueL+iValueL+dValueL;
  dMVR=pValueR+iValueR+dValueR;
  MVR=0;
  MVL=0;
  MVR=MVR+dMVR;
  MVL=MVL+dMVL;

  Serial.print("driving");
  Serial.print("\t");
  Serial.print(MVL);
  Serial.print("\t");
  Serial.print(MVR);
  Serial.print("\t");

  MVL = constrain(MVL,0,255);//関数内は値の調整(ArduinoではPWMは0~255だから)
  MVR = constrain(MVR,0,255);//関数内は値の調整

  if(vel_goalL==0&&vel_goalR==0){//目標値が0の時はタイヤを止める
    analogWrite(LmoterA,255);
    analogWrite(LmoterB,255);
    analogWrite(RmoterA,255);
    analogWrite(RmoterB,255);
//    delay(1);
  }
  else if(vel_goalL==0&&vel_goalR!=0){//only R motor
    analogWrite(LmoterA,255);
    analogWrite(LmoterB,255);
    if(directionR==FRONT){
      analogWrite(RmoterA,MVR);  
      analogWrite(RmoterB,0);
      }
      else if(directionR==BACK){
      analogWrite(RmoterA,0);
      analogWrite(RmoterB,MVR);       
      }
  }
  else if(vel_goalL!=0&&vel_goalR==0){
    analogWrite(RmoterA,255);
    analogWrite(RmoterB,255);
    if(directionL==FRONT){//両方PWMピンにする
      analogWrite(LmoterA,MVL);
      analogWrite(LmoterB,0);
    }  
    else if(directionL==BACK){
      analogWrite(LmoterA,0);
      analogWrite(LmoterB,MVL);
      }    
  }
  else{
    if(directionL==FRONT&&directionR==FRONT){//両方PWMピンにする
      analogWrite(LmoterA,MVL);
      analogWrite(LmoterB,0);
      analogWrite(RmoterA,MVR);  
      analogWrite(RmoterB,0);
    }  
    else if(directionL==BACK&&directionR==FRONT){
      analogWrite(LmoterA,0);
      analogWrite(LmoterB,MVL);
      analogWrite(RmoterA,MVR);  
      analogWrite(RmoterB,0);
      }
      else if(directionL==FRONT&&directionR==BACK){
      analogWrite(LmoterA,MVL);
      analogWrite(LmoterB,0);
      analogWrite(RmoterA,0);
      analogWrite(RmoterB,MVR);       
      }
      else if(directionL=BACK&&directionR==BACK){
      analogWrite(LmoterA,0);
      analogWrite(LmoterB,MVL);      
      analogWrite(RmoterA,0);
      analogWrite(RmoterB,MVR);  
      }
  }


}

void servo(boolean state){//サーボ用マイコンに信号を送るための関数
  if(state==HIGH)digitalWrite(servopin,HIGH);
  if(state==LOW)digitalWrite(servopin,LOW);
}


//一定距離進む関数(左目標値、右目標値、左進行方向、右進行方向）割としっかり進む。
//本番は500mmで2500くらい<-要調整
void driveDistance(int L,int R,boolean directionL,boolean directionR){
  int drivepower=150;
  volatile unsigned long countR_old=countR;
  volatile unsigned long countL_old=countL;
  boolean Rval=0,Lval=0,ans=1;
  while(ans){

    if(countR-countR_old<R&&countL-countL_old<L){
        drive(drivepower,drivepower,directionR,directionL);
      }
    else if(countR-countR_old>=R&&countL-countL_old<L){
        drive(drivepower,0,directionR,directionL);
      }
    else if(countR-countR_old<R&&countL-countL_old>=L){
        drive(0,drivepower,directionR,directionL);
      }
    else if(countR-countR_old>=R&&countL-countL_old>=L){//指定距離進んだら止まる
        drive(0,0,directionR,directionL);
        ans=0;
      }
    

  }

}


