# -*- coding: utf-8 -*-
import serial
import time

def main():

    ser = serial.Serial('/dev/ttyACM0', 9600)
    time.sleep(1.5)
    while True:
        flag = raw_input()
        ser.write(flag)
        if(flag == 'a'): # aが入力されたら通信終了
            break;
    ser.close()

if __name__ == '__main__':
    main()
