/*
  課題
  ・バックの時の動作の改善。
*/

//＊＊＊AE-PCA9685でサーボ動かす用＊＊＊//
#include <Wire.h>
#include <math.h>
#include <PCA9685.h> //PCA9685用ヘッダーファイル（秋月電子通商作成）
PCA9685 pwm = PCA9685(0x40);    //PCA9685のアドレス指定（AE-PCA9685一枚なのでこれでOK)
char dat[10];
int count = 0;
int cmd = 0;
String cmd_dat = "none";
#define SERVOMIN 150            //最小パルス幅 (SG92Rサーボパルスにあわせて設定)
#define SERVOMAX 600            //最大パルス幅 (SG92Rサーボパルスに設定)
#define ServoDelay 10            //現在角度からターゲット角度へ移行する間にかけるディレイ時間
#define LED_LEFT_PIN 8          //左目
#define LED_RIGHT_PIN 9         //右目

//以下ENUMに変える
#define ADVANCE 1111
#define RIGHT 2222
#define LEFT 3333
#define STOP 4444
#define BACK 5555
#define LED_ON 6666
#define LED_OFF 7777
#define FORWARD 8888
#define NONE 9999

#define DEBUG

static double theta0_rad;
static double theta1_rad;
static double theta2_rad;
static double theta3_rad;
static double theta4_rad;
static double theta5_rad;
static double theta6_rad;
static double theta7_rad;
static double theta0_deg = 135;
static double theta1_deg = 135;
static double theta2_deg = 135;
static double theta3_deg = 135;
static double theta4_deg = 135;
static double theta5_deg = 135;
static double theta6_deg = 135;
static double theta7_deg = 135;
static double theta8_deg = 135; //背骨のサーボ。方向転換用のやつ。

  //中心を135度としたときの各サーボの角度の補正値。
const double theta0_cor_deg = 16.;
const double theta1_cor_deg = -4.;
const double theta2_cor_deg = 10.;
const double theta3_cor_deg = -4.;
const double theta4_cor_deg = 0.;
const double theta5_cor_deg = 0.;
const double theta6_cor_deg = 14.;
const double theta7_cor_deg = 4.;
const double theta8_cor_deg = 0.;

  //足先の座標を格納する変数。x0とy0で一組という感じ。念のための初期値が入ってる。
static double x0 = 1.8;
static double x1 = 1.8;
static double x2 = 1.8;
static double x3 = 1.8;
static double y0 = 0.0;
static double y1 = 0.0;
static double y2 = 0.0;
static double y3 = 0.0;

static double phi0 = 0. / 6 * M_PI; //楕円の軌跡を描くためのパラメータ。ch0,1　左前
static double phi1 = 2. / 6 * M_PI; //10./6　昔の値 ch2,3　左後ろ
static double phi2 = 6. / 6 * M_PI; //1.　昔の値 ch4,5　右前
static double phi3 = 10. / 6 * M_PI; //2./6　昔の値 ch6,7　右後ろ
static double direction = 1.;//進む方向。1なら前、-1なら後ろに進む。
const double major = 0.5;//楕円の長軸。歩幅のストロークになる。y
const double minor = 0.5;//楕円の短軸。足を前に戻すときの高さに関係。x
const double resolution = 30.0;//モーションの分解能。20~30くらいがちょうどいい？


  void setup() 
{  //サーボ用セットアップ ※A4ピンをSDA,A5ピンをSCLに使います
//  pwm.begin();                   //初期設定 (アドレス0x40用)
//  pwm.setPWMFreq(60);            //PWM周期を60Hzに設定 (アドレス0x40用)
  Serial.begin(9600);
  pinMode(LED_LEFT_PIN, OUTPUT);
  pinMode(LED_RIGHT_PIN, OUTPUT);

//  servo_write(0, 135 + 16); //基準は135。そこから補正。
//  servo_write(1, 135 - 4);
//  servo_write(2, 135 + 10);
//  servo_write(3, 135 - 4);
//  servo_write(4, 135 + 0);
//  servo_write(5, 135 + 4);
//  servo_write(6, 135 + 14);
//  servo_write(7, 135 + 4);//こいつだけ未調整．
//  servo_write(8, 135 - 4);
  delay(1000);
}

void loop() {

  int serial_input = Serial.read();
  if (serial_input == -1) {
  	return;
  }
  char input_char = (char)serial_input;
  Serial.write(input_char);

  switch (input_char) { //引数は、inputchar
    case 'a'://advance.前進。
    direction = 1.;
//      servo_output(theta0_deg + theta0_cor_deg, theta1_deg + theta1_cor_deg,
//                   theta2_deg + theta2_cor_deg, theta3_deg + theta3_cor_deg,
//                   theta4_deg + theta4_cor_deg, theta5_deg + theta5_cor_deg,
//                   theta6_deg + theta6_cor_deg, theta7_deg + theta7_cor_deg,
//                   theta8_deg + theta8_cor_deg);
#ifdef DEBUG
    Serial.println("advance");
#endif
    break;
    case 'r'://right.右を向く。
//      theta8_deg = 175.;
//      servo_output(theta0_deg + theta0_cor_deg, theta1_deg + theta1_cor_deg,
//                   theta2_deg + theta2_cor_deg, theta3_deg + theta3_cor_deg,
//                   theta4_deg + theta4_cor_deg, theta5_deg + theta5_cor_deg,
//                   theta6_deg + theta6_cor_deg, theta7_deg + theta7_cor_deg,
//                   theta8_deg + theta8_cor_deg);
#ifdef DEBUG
    Serial.println("right");
#endif
    break;
    case 'f'://forward.前を向く。
//      theta8_deg = 135.;
//      servo_output(theta0_deg + theta0_cor_deg, theta1_deg + theta1_cor_deg,
//                   theta2_deg + theta2_cor_deg, theta3_deg + theta3_cor_deg,
//                   theta4_deg + theta4_cor_deg, theta5_deg + theta5_cor_deg,
//                   theta6_deg + theta6_cor_deg, theta7_deg + theta7_cor_deg,
//                   theta8_deg + theta8_cor_deg);
#ifdef DEBUG
    Serial.println("forward");
#endif
    break;
    case 'l'://left.左を向く。
//      theta8_deg = 95.;
//      servo_output(theta0_deg + theta0_cor_deg, theta1_deg + theta1_cor_deg,
//                   theta2_deg + theta2_cor_deg, theta3_deg + theta3_cor_deg,
//                   theta4_deg + theta4_cor_deg, theta5_deg + theta5_cor_deg,
//                   theta6_deg + theta6_cor_deg, theta7_deg + theta7_cor_deg,
//                   theta8_deg + theta8_cor_deg);
#ifdef DEBUG
    Serial.println("left");
#endif
    break;
    case 'b'://back.後進。
    direction = -1.;
//      servo_output(theta0_deg + theta0_cor_deg, theta1_deg + theta1_cor_deg,
//                   theta2_deg + theta2_cor_deg, theta3_deg + theta3_cor_deg,
//                   theta4_deg + theta4_cor_deg, theta5_deg + theta5_cor_deg,
//                   theta6_deg + theta6_cor_deg, theta7_deg + theta7_cor_deg,
//                   theta8_deg + theta8_cor_deg);
#ifdef DEBUG
    Serial.println("back");
#endif
    break;
    case 's'://stop.待て。

      //      servo_output(theta0_deg+theta0_cor_deg,theta1_deg+theta1_cor_deg,
      //                   theta2_deg+theta2_cor_deg,theta3_deg+theta3_cor_deg,
      //                   theta4_deg+theta4_cor_deg,theta5_deg+theta5_cor_deg,
      //                   theta6_deg+theta6_cor_deg,theta7_deg+theta7_cor_deg,
      //                   theta8_deg+theta8_cor_deg);
#ifdef DEBUG
    Serial.println("stop");
#endif
    break;
    case 'o'://LED点灯
    digitalWrite(LED_RIGHT_PIN, HIGH);
    digitalWrite(LED_LEFT_PIN, HIGH);
#ifdef DEBUG
    Serial.println("led_on");
#endif
    break;
    case 'd'://LED消灯
    digitalWrite(LED_RIGHT_PIN, LOW);
    digitalWrite(LED_LEFT_PIN, LOW);
#ifdef DEBUG
    Serial.println("led_off");
#endif
    break;
    case 'u'://下向く
//      servo_write(15, 45);
#ifdef DEBUG
    Serial.println("look down");
#endif
    break;
    case 't'://上向く
//      servo_write(15, 0);
#ifdef DEBUG
    Serial.println("look up");
#endif
    break;
    default://シリアルが上記以外の時何もしない
    break;
  }

}
