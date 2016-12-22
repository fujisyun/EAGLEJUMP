/*
二つの超音波センサーは10cmくらい離せばOK。
ただ、1つのセンサーの検知範囲が扇形のようになっているので、
車体前方に障害物があるときは、両方のセンサーが検値をする。
また、前方約30cm以内に障害物が入らないと距離の値を出力しないようになっている。
*/

void avoid(){//Lが左、Rが右の超音波センサー。
  float Ld,Rd;
//  static int Lcount=0;
//  static int Rcount=0;
  static int var = 1;
  static int angle = 0;
  static int timeout = 0;
  const int timeoutborder = 10;//timeoutがtimeourborderを超えたら、マシンの向きを正面に戻す。
  
  Ld = getDistance(trigPinL,echoPinL);
  Rd = getDistance(trigPinR,echoPinR);
  Serial.print(Ld);
  Serial.print(",");
  Serial.println(Rd);
  Serial.print("angle=");
  Serial.print(angle);
  Serial.print(",timeout=");
  Serial.println(timeout);



  //障害物なし。前進。
  if(Rd==0&&Ld==0){
//   if(Lcount==0&&Rcount==0){
      drive(2000,2000,FRONT,FRONT);
//    }
//    if(Lcount!=0){
//      drive(2000,2000,FRONT,FRONT);
//    }
//    if(Rcount!=0){
//      drive(2000,2000,FRONT,FRONT);
//    }

    timeout++;
  }

//新しいプログラム（ダメだったら、元からのプログラムに変更する。）
//  //正面に障害物あり。
//  if(Rd!=0||Ld!=0){
//    drive(0,0,FRONT,FRONT);
//    Serial.println("Stop!");
//    delay(100);
//    if(angle>=0){//0が正面、マイナスが右よりに向いてる。
//      while(Ld!=0){
//        drive(2000,0,FRONT,BACK);
//        Serial.println("R");
//        Ld = getDistance(trigPinL,echoPinL);
//        angle--;
//      }
//    }else if(angle<0){
//      while(Rd!=0){
//        drive(0,2000,BACK,FRONT);
//        Serial.println("L");
//        Rd = getDistance(trigPinR,echoPinR);
//        angle++;
//      }
//    }
//  
//  }

 
//元からのプログラム
//  //正面に障害物あり。
//  if(Rd!=0||Ld!=0){
//    drive(0,0,FRONT,FRONT);
//    Serial.println("Stop!");
//    delay(200);
//    switch(var){
//    case 1://右を見る
//      Serial.println("Practice case 1. Turn Right.");
//      Rcount=0;
//      Lcount=1;
//      while(Ld!=0){
//        drive(2000,0,FRONT,BACK);
//        Serial.println("R");
//        Ld = getDistance(trigPinL,echoPinL);
//      }
//      var = 2;  
//      break;
//    case 2://左を見る
//      Serial.println("Practice case 2. Turn Left.");
//      Rcount=1;
//      Lcount=0;
//      while(Rd!=0){
//        drive(0,2000,BACK,FRONT);
//        Serial.println("L");
//        Rd = getDistance(trigPinR,echoPinR);
//      }
//      var = 1;  
//    }
//  }//switchのかっこ

//新しいプログラムその2
  //正面に障害物あり。
  if(Rd!=0||Ld!=0){
    drive(0,0,FRONT,FRONT);
    Serial.println("Stop!");
    delay(200);
    if(angle<=0){
      var = 2;
    }else{
      var = 1;
    }
    switch(var){
    case 1://右を見る
      Serial.println("Practice case 1. Turn Right.");
      while(Ld!=0&&Rd!=0){
        drive(0,1000,FRONT,BACK);
        Serial.println("R");
        Rd = getDistance(trigPinR,echoPinR);
        Ld = getDistance(trigPinL,echoPinL);
        angle--;
      }
      break;
    case 2://左を見る
      Serial.println("Practice case 2. Turn Left.");
      while(Ld!=0&&Rd!=0){
        drive(0,1000,BACK,FRONT);
        Serial.println("L");
        Rd = getDistance(trigPinR,echoPinR);
        Ld = getDistance(trigPinL,echoPinL);
        angle++;
      }
      break; 
    }//switchのかっこ
  }

//曲がった後に正面に向きを戻すためのもの。
  if(timeout>=timeoutborder){
    timeout = 0;
//    drive(0,0,FRONT,FRONT);
    Serial.println("timeout reset");
    delay(200);
    
    if(angle==0){
      var = 3;
    }else if(angle<0){
      var = 2;
    }else{
      var = 1;
    }
    switch(var){
    case 1://右を見る
      Serial.println("Practice case 1. Turn Right.");
      while(angle!=0){
        drive(0,1000,FRONT,BACK);
        Serial.println("R");
        Rd = getDistance(trigPinR,echoPinR);
        Ld = getDistance(trigPinL,echoPinL);
        angle--;
        delay(10);
      }
      break;
    case 2://左を見る
      Serial.println("Practice case 2. Turn Left.");
      while(angle!=0){
        drive(0,1000,BACK,FRONT);
        Serial.println("L");
        Rd = getDistance(trigPinR,echoPinR);
        Ld = getDistance(trigPinL,echoPinL);
        angle++;
        delay(10);
      }
      break; 
    case 3://何もしない。
      break; 
    }
  }//switchのかっこ

  delay(100);
}


//エコー信号の長さからセンチメートル単位で距離を計算する
float getDistance(int trigPin,int echoPin){
  long echoLen;
  float distance;
  
  //トリガにマイクロ秒のhighパルスを出力
  digitalWrite(trigPin,LOW);    
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  //エコー信号の長さをマイクロ秒単位で測る
  echoLen = pulseIn(echoPin,HIGH,2000);//ここの数字で最短距離を調整可能。
  distance = (float)echoLen*0.017;
  delay(10);//他の超音波センサと干渉しないように少し待つ
  return distance;
}
