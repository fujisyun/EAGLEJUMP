
//これが多分一番いいと思います。

//ラインセンサが反応したらすぐモーターを回すプログラム。
//Drive(255,255);は挙動変だったら消すと良い。

//＊＊＊AE-PCA9685でサーボ動かす用＊＊＊//
#include <Wire.h>
#include <PCA9685.h> //PCA9685用ヘッダーファイル（秋月電子通商作成）
PCA9685 pwm = PCA9685(0x40);    //PCA9685のアドレス指定（AE-PCA9685一枚なのでこれでOK)

#define SERVOMIN 150            //最小パルス幅 (標準的なサーボパルスに設定)
#define SERVOMAX 600            //最大パルス幅 (標準的なサーボパルスに設定)
#define Servo1 90            //サーボ右1の中心角度
#define Servo2 126            //サーボ左2の中心角度(なんかずれてるor270サーボの可能性)
#define ServoDelay 6            //現在角度からターゲット角度へ移行する間にかけるディレイ時間

int n1 = 0; //サーボ1動かす時の繰り返し用変数
int n2 = 0; //サーボ2動かす時の繰り返し用変数
//＊＊＊ここまで＋セットアップ＊＊＊//

void setup(){
  //サーボ用セットアップ ※A4ピンをSDA,A5ピンをSCLに使います
  pwm.begin();                   //初期設定 (アドレス0x40用)
  pwm.setPWMFreq(60);            //PWM周期を60Hzに設定 (アドレス0x40用)
  
  Serial.begin(9600);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A0,INPUT);//左のセンサー。L1
  pinMode(A1,INPUT);//右のセンサー。L2
}

//サーボを実際に動かす関数
void servo_write(int ch, int ang){ //動かすサーボチャンネルと角度を指定
  ang = map(ang, 0, 180, SERVOMIN, SERVOMAX); //角度（0～180）をPWMのパルス幅（150～600）に変換
  pwm.setPWM(ch, 0, ang);
  //delay(1);//無くてよい
}

//モーション
void IkaservoOP(){
   //サーボ用プログラム、n1 or n2にターゲット角度   
   //１°ずつ動かさないと電池だと電流不足でカクつきます.最終的にここに落ち着いた
   //最初などターゲット角度と現在角度との間に大きく差がある時はどうしてもカクつきます
   //中心角度からそれぞれ外側に約30°開いてまた戻るだけの関数です
   //足を開くとき   
      for(int i=0 ; i<30 ; i++){
        n1 = Servo1 - i;
        servo_write(0,n1);
        delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
        n2 = Servo2 + i;
        servo_write(1,n2);
        delay(ServoDelay);
         //Serial.println(n1);
         //Serial.println(n2);
      }
}
//モーション
void IkaservoCL(){
    //足を閉じるとき
      for(int i=0 ; i<30 ; i++){
        n1 = n1 + 1;
        servo_write(0,n1);
        delay(ServoDelay);
        n2 = n2 - 1;
        servo_write(1,n2);
        delay(ServoDelay);
         //Serial.println(n1);
         //Serial.println(n2);
      }  
}

void loop(){
//      Serial.println("servo start");
    //サーボモーション
      IkaservoOP();//足のモーション関数実行
//      Serial.println("servo end");
    //ライントレース
      int c = 750;//閾値。黒いと値が大きくなる。
      float L1 = analogRead(A0);//左センサ
      float L2 = analogRead(A1);//右センサ
      Serial.print(L1);
      Serial.print(":");
      Serial.println(L2);
      if(L1>=c){//左センサが黒を検知したとき、
            Drive(-120,0);
            delay(20);
      }else if(L2>=c){//右センサが黒を検知したとき、
            Drive(0,-150);
            delay(20);
      }else{//センサが黒を検知していないとき
            Drive(200,180); 
            delay(20);
      }
      IkaservoCL();
      delay(10);
      
}


