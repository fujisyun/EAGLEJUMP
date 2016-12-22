#include "drivePID.h"

void flashL(){//エンコーダディスクの状況を読み取る。
  countL++;
}

void flashR(){
  countR++;
}



void speedmeter(){//速度velocityを出している。単位は知らん
  static long oldL=0;
  static long oldR=0;
  velocityL=(countL-oldL);//（countを初期化したらoldも初期化）
  velocityR=(countR-oldR);//（countを初期化したらoldも初期化）
  distanceL+=velocityL;
  distanceR+=velocityR;
  velocityL*=fix;
  velocityR*=fix;

  if(countL>2000000000){
    countL=0;//ある程度大きくなったら初期化
    distanceL=0;
  }
  if(countR>2000000000){
    countR=0;//ある程度大きくなったら初期化
    distanceR=0;
  }
  oldL=countL;
  oldR=countR;
}

//一定角度旋回するための関数（回転方向）
void quadturn(boolean muki){
  int drivepower=50;//強さ
  int value=250;//どれくらい曲がるか
  switch(muki){
  case left://左に回りたいとき
    countL_old=countL;
    while(abs(countL-countL_old)<value){
      Serial.println(countL-countL_old);
      drive(drivepower,0,BACK,FRONT);
    }
    countR_old=countR;
    while(abs(countR-countR_old)<value){
      Serial.println(countR-countR_old);
      drive(0,drivepower,FRONT,FRONT);
    }
    drive(0,0,FRONT,FRONT);
    delay(1000);
    break;

  case right://右に回りたいとき
    countR_old=countR;
    while(abs(countR-countR_old)<value){
      Serial.println(countR-countR_old);
      drive(0,drivepower,BACK,FRONT);
    }
    countL_old=countL;
    while(abs(countL-countL_old)<value){
      Serial.println(countL-countL_old);
      drive(drivepower,0,FRONT,FRONT);
    }
    drive(0,0,FRONT,FRONT);
    delay(1000);
    break;
  }
}

//void forest(){//迷いの森(値など要修正）
//  float Ad,Bd;
//  boolean A=0,B=0;
//  static boolean turn=left;
//  int mode,countR_old,countL_old;
//  Ad = getDistance(trigPinA,echoPinA);//右
//  Bd = getDistance(trigPinB,echoPinB);//左
//
//  //三項演算子で書いてしまいました。可読性が下がる…
//  //距離が3以下になったら障害物検知
//  A=(Ad>3)?1:0;//A>3なら1、そうでないなら0を返す
//  B=(Bd>3)?1:0;
//  mode=A+B*2;
//
//
////  Serial.println(mode);
//
//  switch(mode){//両方検知しない
//  case 0://両方検知しない
//    //  while()
//    drive(50,50,FRONT,FRONT);
//    break;
//
//  case 1://右側検知
//    quadturn(left);
//    break;
//
//  case 2://左側検知
//    quadturn(right);
//    break;
//
//  case 3://両側検知
//    if((Ad-Bd)<0){
//      quadturn(left);
//    }
//    else{
//      quadturn(right);
//    }
//
//    break;
//
//  default://(お守り)
//    break;
//  }
//}



void line_sensor(){//ラインセンサの値を入れる
  //一部ポート操作で書いてます。digitalReadと同じ処理だが、処理速度が1/10くらいになる。
  //各ポートによって立つビットが違うので、＆で調整してます。
  LSD1=!(PINC & _BV(1)&&2);//digitalRead(A1);と同じ
  LSA2=analogRead(LineSensorAna2);
  LSD3=!(PINC & _BV(3)&&8);//digitalRead(A3);と同じ
  LSA4=analogRead(LineSensorAna4);
  LSD5=!(PINC & _BV(5)&&32);//digitalRead(A3);と同じ
  LSmode=LSD1+LSD3*4+LSD5*16;
  Serial.println(LSmode);
}

//ライントレースのプログラム (ライントレースする向き)
//（FRONT)で通常のライントレース。
//（BACK)の時はT字までライントレース。->T字を見つけたら止まる。
void line_trace(){
  line_sensor();
  volatile unsigned long countL_old;
  volatile unsigned long countR_old;
      Serial.print(situation);
      Serial.print("\t");

  int drivepower=150;
  T=false;
    switch(LSmode){
    case 1://左のみ
      drive(0,drivepower,FRONT,FRONT);
      Serial.print("case 1");
      Serial.print("\t");
      break;

    case 5://中心と左
      drive(0,0,BACK,BACK);
      //    delay(1000);
      countL_old=countL;
      countR_old=countR;
      while((countL-countL_old)<100&&(countR-countR_old)<100){
          Serial.print("xxxxxxxxxxxxxxxxxxxx");
          Serial.print("\t");          
        drive(drivepower,drivepower,FRONT,FRONT);
  LSD5=!(PINC & _BV(5)&&32);
          if(LSD5){//右が反応したら

          T=true;
          situation++;
//          driveDistance(500,500,FRONT,FRONT);
          drive(0,0,FRONT,FRONT);
          delay(5000);
//         break;
        }
      }
//T字を見つけたらループを抜ける
        if(T==true)break;

      T=false;
      countR_old=countR;
      while(abs(countR-countR_old)<100){
        LSD1=PINC & _BV(1)&&2;
        Serial.print(countR-countR_old);
        Serial.print("\t");
        Serial.println("aaaaaaaaaaaaaa");
        drive(0,drivepower,FRONT,FRONT);
      }
      drive(0,0,FRONT,FRONT);

      while(LSD3){
        Serial.println("ccccccccccccc");
        LSD3=PINC & _BV(3)&&8;
        drive(0,drivepower,FRONT,FRONT);
      }
      break;
      


    case 4://中心のみ
      drive((LSA4>>2)+drivepower-50,(LSA2>>2)+drivepower-50,FRONT,FRONT);
      Serial.print("case 4");
      Serial.print("\t");
      break;

    case 16://右のみ
      drive(drivepower,0,FRONT,FRONT);
      Serial.print("case 16");
      Serial.print("\t");
      break;



    case 20://真ん中と右
      countL_old=countL;
      countR_old=countR;
        //ある程度前進
      while((countL-countL_old)<100&&(countR-countR_old)<100){
   LSD1=!(PINC & _BV(1)&&2);
          drive(drivepower,drivepower,FRONT,FRONT);
          Serial.print(LSD1);
          Serial.print("\t");          
        if(LSD1){
          T=true;
          situation++;
          drive(0,0,FRONT,FRONT);
          delay(5000);
//          break;
        }
      }
      if(T==true)break;
      T=false;
      drive(0,0,BACK,BACK);

      countL_old=countL;
        //現在の値からロータリーエンコーダが200カウントするまで
      while(abs(countL-countL_old)<200){
        LSD1=PINC & _BV(5)&&2;
        Serial.print(countL);
        Serial.print("\t");
        Serial.println("aaaaaaaaaaaaaa");
        drive(drivepower,0,FRONT,FRONT);
      }
      drive(0,0,FRONT,FRONT);

      while(LSD3){
        Serial.println("ccccccccccccc");
        LSD3=PINC & _BV(3)&&8;
        drive(drivepower,0,FRONT,FRONT);
      }
      break;
      

    case 21:
      drive(0,0,FRONT,FRONT);
      Serial.print("case 21");
      Serial.print("\t");
      break;


    default:
        //ラインセンサーから読み取った値を使ってPID制御する。
      drive((LSA4>>3)+drivepower-50,(LSA2>>3)+drivepower-50,FRONT,FRONT);
      Serial.print("default");
      Serial.print("\t");
      break;
  }
}




