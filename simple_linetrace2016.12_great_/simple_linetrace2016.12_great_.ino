//これが一番良い。

//ラインセンサが反応したらすぐモーターを回すプログラム。
//Drive(255,255);は挙動変だったら消すと良い。


void setup(){  
  Serial.begin(9600);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(A0,INPUT);//左のセンサー。L1
  pinMode(A1,INPUT);//右のセンサー。L2
}

void loop(){
      int c = 750;//閾値。黒いと値が大きくなる。
      float L1 = analogRead(A0);//左センサ
      float L2 = analogRead(A1);//右センサ
      Serial.print(L1);
      Serial.print(":");
      Serial.println(L2);
      if(L1>=c){//左センサが黒を検知したとき、
            Drive(-200,0);
            delay(50);
      }else if(L2>=c){//左センサが黒を検知したとき、
            Drive(0,-180);
            delay(50);
      }else{//センサが黒を検知していないとき
      Drive(200,180); 
      delay(50);
      }
      delay(10);
}


