/*
二つの超音波センサーは10cmくらい離せばOK。
ただ、1つのセンサーの検知範囲が扇形のようになっているので、
車体前方に障害物があるときは、両方のセンサーが検値をする。
また、前方約30cm以内に障害物が入らないと距離の値を出力しないようになっている。
*/


//Aが左、Bが右の超音波センサー。
const int trigPinA=7;   //トリガを12ピンに設定
const int echoPinA=8;   //エコーを13ピンに設定
const int trigPinB=12;
const int echoPinB=13;


void setup(){
  pinMode(trigPinA,OUTPUT);
  pinMode(echoPinA,INPUT);
  pinMode(trigPinB,OUTPUT);
  pinMode(echoPinB,INPUT);
  Serial.begin(9600);
}
 


void loop(){ 
  avoid(); 
}//loop()
