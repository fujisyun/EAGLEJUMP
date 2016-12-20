void forest(){//Aが左、Bが右の超音波センサー。
  float Ad,Bd;
  Ad = getDistance(trigPinA,echoPinA);
  Bd = getDistance(trigPinB,echoPinB);
  Serial.print(Ad);
  Serial.print(",");
  Serial.println(Bd);
  
  
  
  
  
  delay(500);
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
