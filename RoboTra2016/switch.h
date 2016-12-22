int DIPswitch(){
  boolean pinnum_1,pinnum_2,pinnum_4;
  int position;
  
  pinnum_1 = !digitalRead(A0); 
  pinnum_2 = !digitalRead(1);
  pinnum_4 = !digitalRead(0);

  position = pinnum_1 + 2*pinnum_2 + 4*pinnum_4;
  
  return position;
}
