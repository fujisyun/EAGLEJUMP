//正なら前転、負なら後転する。

void Drive(int x,int y){
   if(x>0&&y>0){
      analogWrite(5,x);
      analogWrite(6,0); 
      analogWrite(10,y);
      analogWrite(11,0);
  }else if(x<=0&&y>0){
      analogWrite(5,0);
      analogWrite(6,abs(x));
      analogWrite(10,y);
      analogWrite(11,0);
  }else if(x>0&&y<=0){
      analogWrite(5,x);
      analogWrite(6,0); 
      analogWrite(10,0);
      analogWrite(11,abs(y));
  }if(x<=0&&y<=0){
     analogWrite(5,0);
      analogWrite(6,abs(x)); 
      analogWrite(10,0);
      analogWrite(11,abs(y));
  }
}
