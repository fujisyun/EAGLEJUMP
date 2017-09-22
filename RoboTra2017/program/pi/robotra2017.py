#coding: utf-8
# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import serial
import time
import cv2
import numpy as np
#camera resolution
RESOLUTION_X = 640/8
RESOLUTION_Y = 480/8

threshold=50
max_value=255
BLACK=0
WHITE=max_value
cv2.namedWindow("Frame")
cv2.namedWindow("Frame2")
 
hsv = np.zeros((RESOLUTION_X,RESOLUTION_Y))
#mouse event functoion
# def mouse_event(event, x, y, flags, param):
#     if event == cv2.EVENT_LBUTTONDOWN:
#         global hsv
#         print(hsv[y][x])

# シリアルの設定 
ser = serial.Serial('/dev/ttyACM0', 9600)
# Arduinoがシリアルを受け取れるようになるまで待つ 
time.sleep(1.5)

# def send_serial_data(string):
#     if string=='TTTTTTTT':
#         ser.write('TTTTTTTT')
 
camera = PiCamera()
camera.resolution = (RESOLUTION_X, RESOLUTION_Y)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(RESOLUTION_X, RESOLUTION_Y))
  
# allow the camera to warmup
time.sleep(0.1) 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
# カメラから画像を取得
    image = frame.array
    #mask
# convert RGB image to grayscale
# BRG画像をグレースケールに	※RGBでないことに注意
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# binarization of grayscale
# グレースケールを二値化
    ret, dst = cv2.threshold(gray,threshold,max_value,cv2.THRESH_BINARY)

#       
    middle=[]
    yyy=[]
    black_row=0
    half_black_row=0
    line_status='none'
    serial_data=''
    Right_angle_direction=-1
#   上から順に
    for pixel_y in range(RESOLUTION_Y-1):
    	count=0
	   prev_color=-1
    	left_of_line=-1
	   right_of_line=-1
#	左からみて黒かどうか確かめる
    	for pixel_x in range(RESOLUTION_X-1):
#	    画素をチェック
    	    if dst.item(pixel_y,pixel_x) == BLACK:
        		if prev_color==BLACK:
                    left_of_line=pixel_x
#		    line_color=BLACK
        		    break
    		prev_color=BLACK
#	右からみて黒かどうか確かめる
    	for pixel_x in range(RESOLUTION_X-1):
        	if dst.item(pixel_y,RESOLUTION_X-1-pixel_x) == BLACK:
                if prev_color==BLACK:
                    right_of_line=RESOLUTION_X-1-pixel_x
                    break
            prev_color=BLACK
#	各座標データとラインの中心の座標を配列にいれる
        yyy.append(pixel_y)
        line_width=right_of_line-left_of_line
        middle.append(line_width/2)
#	一行あたりのラインの黒具合？
        if line_width > RESOLUTION_X/2:
            if line_width > RESOLUTION_X-5:
                black_row=black_row+1
            else:
                half_black_row=half_black_row+1
                Right_angle_direction=left_of_line+line_width/2
# 一行すべて黒の行が3つ以上ある場合
    if black_row > 2:
    	line_status='TTTTTTTT'
    elif half_black_row > 2:
# 直角が右か左か
        if Right_angle_direction > RESOLUTION_X/2:
    	    line_status='Right_angle_right'
        else:
            line_status='Right_angle_left'
    else:
#    最小二乗法的なもので回帰直線を引く
        func=np.polyfit(middle,yyy,1)
#   回帰直線の傾き
        slope=func.item(0)  
#   回帰直線の切片
        segment=func.item(1)
#   回帰直線の式を表示
#        print 'y = ' + str(slope)+' x '+ str(segment)
#   右に曲がるか左に曲がるか
        if slope>1:
            line_status='right'
        elif slope<-1:
            line_status='left'


    print 'line status ='+ line_status
#+'		'+str(Right_angle_direction)
#        print 'pixel_y ='+str(pixel_y)+'	left ='+str(left_of_line)+'	right ='+str(right_of_line)+'	count='+str(line_width)
#        print 'pixel_y ='+str(pixel_y)+'	count ='+str(count)


    cv2.imshow("Frame", dst)
    cv2.imshow("Frame2", gray)
    key = cv2.waitKey(1) & 0xFF

    rawCapture.truncate(0)
  
    if key == ord("q"):
        break
