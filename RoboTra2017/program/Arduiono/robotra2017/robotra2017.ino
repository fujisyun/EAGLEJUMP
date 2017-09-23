/*
  課題
  ・バックの時の動作の改善。
*/

//＊＊＊AE-PCA9685でサーボ動かす用＊＊＊//
#include <Wire.h>
#include <math.h>
#include "PCA9685.h" //PCA9685用ヘッダーファイル（秋月電子通商作成）
PCA9685 pwm = PCA9685(0x40);    //PCA9685のアドレス指定（AE-PCA9685一枚なのでこれでOK)
char dat[10];
int count = 0;
int cmd = 0;
String cmd_dat = "none";
#define SERVOMIN 150            //最小パルス幅 (SG92Rサーボパルスにあわせて設定)
#define SERVOMAX 600            //最大パルス幅 (SG92Rサーボパルスに設定)
#define ServoDelay 10            //現在角度からターゲット角度へ移行する間にかけるディレイ時間
#define LED_LEFT_PIN 5          //左目
#define LED_RIGHT_PIN 6         //右目

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

int led_left_duty=200;
int led_right_duty=200;

void servo_write(int ch, int ang) { //動かすサーボチャンネルと角度を指定
  ang = map(ang, 0, 270, SERVOMIN, SERVOMAX); //角度（0～180）をPWMのパルス幅（150～600）に変換
  pwm.setPWM(ch, 0, ang);
  //  delay(1);//無くてよい
}

void servo_move(double direction,double theta8_deg){
  phi0 = FluctuationPhase(phi0, resolution, direction);
  phi1 = FluctuationPhase(phi1, resolution, direction);
  phi2 = FluctuationPhase(phi2, resolution, direction);
  phi3 = FluctuationPhase(phi3, resolution, direction);

  const double xline_phi = 0.1;//楕円軌道に乗らずxの値が一定値にさせるために、x座標の値を頭打ちにさせる変数。
  const double x_walk_hight = 1.2;
  const double y_refer = 0.2;
  if (cos(phi0) >= xline_phi) {
    x0 = x_walk_hight;
  } else {
    x0 = x_walk_hight + minor * cos(phi0);
  }
  y0 = major * sin(phi0) - y_refer;

  if (cos(phi1) >= xline_phi) {
    x1 = x_walk_hight;
  } else {
    x1 = x_walk_hight + minor * cos(phi1);
  }
  y1 = major * sin(phi1) - y_refer;

  if (cos(phi2) >= xline_phi) {
    x2 = x_walk_hight;
  } else {
    x2 = x_walk_hight + minor * cos(phi2);
  }
  y2 = major * sin(phi2) - y_refer;

  if (cos(phi3) >= xline_phi) {
    x3 = x_walk_hight;
  } else {
    x3 = x_walk_hight + minor * cos(phi3);
  }
  y3 = major * sin(phi3) - y_refer;

  InKine(x0, y0, &theta0_deg, &theta1_deg);
  InKine(x1, y1, &theta2_deg, &theta3_deg);
  InKine(x2, y2, &theta4_deg, &theta5_deg);
  InKine(x3, y3, &theta6_deg, &theta7_deg);

  servo_output(theta0_deg + theta0_cor_deg, theta1_deg + theta1_cor_deg,
   theta2_deg + theta2_cor_deg, theta3_deg + theta3_cor_deg,
   theta4_deg + theta4_cor_deg, theta5_deg + theta5_cor_deg,
   theta6_deg + theta6_cor_deg, theta7_deg + theta7_cor_deg,
   theta8_deg + theta8_cor_deg);
}

double FluctuationPhase(double phi, double reso, double dire) {
  //direは1.だと前、-1.だと後ろに進む。
  //本当はdireで条件分岐させる形にはしたくないんだけどな・・・。なんか変えられそうだけど。
  if (dire >= 1.) {
    if (phi >= 1. / 2 * M_PI && phi < 3. / 2 * M_PI) {
      phi = phi + 2.0 * M_PI / reso * 3.;
    } else if ((phi < 2.0 * M_PI && phi >= 3. / 2 * M_PI) || phi < 1. / 2 * M_PI) {
      phi = phi + 2.0 * M_PI / reso;
    } else if (phi >= 2.0 * M_PI) {
      phi = 0;
    }
  } else if (dire <= -1.) {
    if (phi >= 1. / 2 * M_PI && (phi > 0. && phi < 3. / 2 * M_PI)) {
      phi = phi - 2.0 * M_PI / reso * 3.;
    } else if ((phi <= 2.0 * M_PI && phi >= 3. / 2 * M_PI) || (phi > 0. && phi < 1. / 2 * M_PI)) {
      phi = phi - 2.0 * M_PI / reso;
    } else if (phi <= 0.) {
      phi = 2.0 * M_PI;
    }
  }
  return phi;
}

void InKine(double cox, double coy, double *angle0_deg, double *angle1_deg) {
  //cox,coyはcoordinate x,yの略。
  double length1 = 1.0;//length1,2は足の長さ.length1はtheta1が付け根。
  double length2 = 0.5;//length2はtheta2が付け根。
  double root_x2_y2 = sqrt(cox * cox + coy * coy);
  double angle0_rad = atan(coy / cox) + acos((-(length2 * length2) + (length1 * length1) + root_x2_y2) / (2 * length1 * root_x2_y2));
  double angle1_rad = M_PI - acos((-(cox * cox + coy * coy) + (length2 * length2) + (length1 * length1)) / (2 * length1 * length2));
  //            Serial.println(coy);
  *angle0_deg = angle0_rad * 180.0 / M_PI + 135;
  *angle1_deg = -angle1_rad * 180.0 / M_PI + 135;
}

//サーボにまとめて角度を指定する関数。サーボの数だけ入力を増やすこと。
//左右のサーボで正の方向が逆なので注意すること。（180-〇はその理由のため）
//ちなみに、入力の値はすべてのサーボが右側にあると仮定した状態での計算結果。
void servo_output(double t0, double t1, double t2, double t3, double t4, double t5, double t6, double t7, double t8) { //サーボに実際に角度を指定する関数。
  servo_write(1, (int)t1); //先に足先のサーボの方に命令が行くように順番を並べ替えた。
  servo_write(0, (int)t0);
  servo_write(3, (int)t3);
  servo_write(2, (int)t2);
  servo_write(5, (int)t5);
  servo_write(4, (int)t4);
  servo_write(7, (int)t7);
  servo_write(6, (int)t6);
  servo_write(8, (int)t8);
}

void ServoAngle(int angle) {
  servo_write(0, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(1, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(2, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(3, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(4, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(5, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(6, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(7, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(8, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(9, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(10, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(11, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
  servo_write(12, angle);
  delay(ServoDelay);//必ず一つのサーボを動かしたらdelay(~)を入れる！
}


void setup() 
{  //サーボ用セットアップ ※A4ピンをSDA,A5ピンをSCLに使います
  pwm.begin();                   //初期設定 (アドレス0x40用)
  pwm.setPWMFreq(60);            //PWM周期を60Hzに設定 (アドレス0x40用)
  Serial.begin(9600);
  pinMode(LED_LEFT_PIN, OUTPUT);
  pinMode(LED_RIGHT_PIN, OUTPUT);
  servo_write(0, 135 + 16); //基準は135。そこから補正。
  servo_write(1, 135 - 4);
  servo_write(2, 135 + 10);
  servo_write(3, 135 - 4);
  servo_write(4, 135 + 0);
  servo_write(5, 135 + 4);
  servo_write(6, 135 + 14);
  servo_write(7, 135 + 4);//こいつだけ未調整．
  servo_write(8, 135 - 4);
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
    servo_move(direction,theta8_deg);
#ifdef DEBUG
    Serial.println("advance");
#endif
    break;
    case 'r'://right.右を向く。
    theta8_deg=150.;
    if (theta8_deg>150)
    {
      theta8_deg=150;
    }
    servo_move(direction,theta8_deg);
#ifdef DEBUG
    Serial.println("right");
#endif
    break;
    case 'f'://forward.前を向く。
     theta8_deg = 135.;
    servo_move(direction,theta8_deg);
#ifdef DEBUG
    Serial.println("forward");
#endif
    break;
    case 'l'://left.左を向く。
     theta8_deg--;
     if (theta8_deg<95)
     {
       theta8_deg=95;
     }
    servo_move(direction,theta8_deg);
#ifdef DEBUG
    Serial.println("left");
#endif
    break;
    case 'b'://back.後進。
    direction = -1.;
    servo_move(direction,theta8_deg);
#ifdef DEBUG
    Serial.println("back");
#endif
    break;
    case 's'://stop.待て。
    servo_write(0, 135 + 16); //基準は135。そこから補正。
    servo_write(1, 135 - 4);
    servo_write(2, 135 + 10);
    servo_write(3, 135 - 4);
    servo_write(4, 135 + 0);
    servo_write(5, 135 + 4);
    servo_write(6, 135 + 14);
    servo_write(7, 135 + 4);//こいつだけ未調整．
    servo_write(8, 135 - 4);
    servo_write(15, 0);

#ifdef DEBUG
    Serial.println("stop");
#endif
    break;
    case 'o'://LED点灯
    led_right_duty++;
    led_left_duty++;
    if (led_right_duty>250)
    {
      led_right_duty=250;
    }
    if (led_left_duty>250)
    {
      led_left_duty=250;
    }
    analogWrite(LED_RIGHT_PIN, led_right_duty);
    analogWrite(LED_LEFT_PIN, led_left_duty);
#ifdef DEBUG
    Serial.println("led_on");
#endif
    break;
    case 'm':
    led_right_duty--;
    led_left_duty--;
    if (led_right_duty<125)
    {
      led_right_duty=125;
    }
    if (led_left_duty<125)
    {
      led_left_duty=125;
    }
    analogWrite(LED_RIGHT_PIN, led_right_duty);
    analogWrite(LED_LEFT_PIN, led_left_duty);
    break;
    case 'd'://LED消灯
    led_right_duty--;
    led_left_duty--;
    if (led_right_duty<125)
    {
      led_right_duty=125;
    }
    if (led_left_duty>125)
    {
      led_left_duty=125;
    }
    analogWrite(LED_RIGHT_PIN, 0);
    analogWrite(LED_LEFT_PIN, 0);
#ifdef DEBUG
    Serial.println("led_off");
#endif
    break;
    case 'u'://下向く
     servo_write(15, 45);
#ifdef DEBUG
    Serial.println("look down");
#endif
    break;
    case 't'://上向く
     servo_write(15, 0);
#ifdef DEBUG
    Serial.println("look up");
#endif
    break;
    default://シリアルが上記以外の時何もしない
    break;
  }

}
