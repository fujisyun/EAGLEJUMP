/*************************************************************************************
今回のプログラムの変数。2値しかないものは基本的にboolean型で定義している。
エンコーダ関係の変数は正の数のみ使うため、unsigned指定子(?）を付けている。

*************************************************************************************/
#ifndef now
#define now 2
#define pre 1
#define prepre 0
#define fix 11
#define FRONT 0
#define BACK 1
#define left 0
#define right 1

volatile unsigned long countL=0;//Lエンコーダディスクのカウント。flash関数を参照。intだと足りなくなりそうだったのでlongに
volatile unsigned long countR=0;//Rエンコーダディスクのカウント。flash関数を参照。intだと足りなくなりそうだったのでlongに
int velocityL=0;//L速度
int velocityR=0;//R速度
int vel_goalL=0;//L速度目標値
int vel_goalR=0;//R速度目標値
unsigned long distanceR=0;
unsigned long distanceL=0;
unsigned long countR_old;
unsigned long countL_old;
int MVL=0;//Lモーター出力
int MVR=0;//Rモーター出力
int dMVL=0;//Lモーター偏差
int dMVR=0;//Rモーター偏差

float  KpL = 1.5;//P制御の係数L
float  KpR = 1.5;//P制御の係数R
float  KiL = 0.75;//I制御の係数L
float  KiR = 0.75;//I制御の係数R
float  KdL = 0.5;//D制御の係数L
float  KdR = 0.5;//D制御の係数R

//PID制御の途中の式で使っている変数。float型にした方がよいかも
int  pValueL=0;
int  pValueR=0;
int  iValueL=0;
int  iValueR=0;
int  dValueL=0;
int  dValueR=0;

const int trigPinL=7;
const int echoPinL=8;
const int trigPinR=12;   //トリガを12ピンに設定
const int echoPinR=13;   //エコーを13ピンに設定

const int LineSensorDig1=(1+14);//ラインセンサ。A1ピンを使うためこのように書いた
const int LineSensorAna2=2;
const int LineSensorDig3=(3+14);
const int LineSensorAna4=4;
const int LineSensorDig5=(5+14);

//それぞれモーター用のピン
const int RmoterA=5;
const int RmoterB=6;
const int LmoterA=9;
const int LmoterB=10;

const int servopin=11;//サーボを動かすArduino UNOに信号を送るためのピン番号

//ラインセンサの状態を入れる変数。
bool LSD1=0;
int LSA2=0;
bool LSD3=0;
int LSA4=0;
bool LSD5=0;
int LSmode=0;

int situation=0;//状況を示す関数。
bool T=0;//T字判定


//boolean state=0;

//偏差を蓄積するための配列。先頭からen-2,en-1,en
int dvelL[3]={
  0,0,0};
int dvelR[3]={
  0,0,0};
#endif
