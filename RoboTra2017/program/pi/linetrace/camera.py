#coding: utf-8
# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
#camera resolution
RESOLUTION_X = 640/8
RESOLUTION_Y = 480/8

threshold=120
max_value=255
BLACK=0
WHITE=max_value
cv2.namedWindow("Frame")
cv2.namedWindow("Frame2")
 
hsv = np.zeros((RESOLUTION_X,RESOLUTION_Y))
#mouse event functoion
def mouse_event(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        global hsv
        print(hsv[y][x])
 
 
camera = PiCamera()
camera.resolution = (RESOLUTION_X, RESOLUTION_Y)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(RESOLUTION_X, RESOLUTION_Y))
  
# allow the camera to warmup
time.sleep(0.1) 
# capture frames from the camera
# カメラから画像を取得
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
    #mask
# convert RGB image to grayscale
# BRG画像をグレースケールに	※RGBでないことに注意
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# binarization of grayscale
# グレースケールを二値化
    ret, dst = cv2.threshold(gray,threshold,max_value,cv2.THRESH_BINARY)

#   配列の定義    
    middle=[]
    yyy=[]
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
	middle.append(middle,(right_of_line-left_of_line)/2)

	line_width=right_of_line-left_of_line
        print 'pixel_y ='+str(pixel_y)+'	left ='+str(left_of_line)+'	right ='+str(right_of_line)+'	count='+str(line_width)
#        print 'pixel_y ='+str(pixel_y)+'	count ='+str(count)


#    最小二乗法的なもので回帰直線を引く
    func=np.polyfit(middle,yyy,1)
#   回帰直線の傾き
    slope=func.item(0)	
#   回帰直線の切片
    segment=func.item(1)

    cv2.imshow("Frame", dst)
    cv2.imshow("Frame2", gray)
    key = cv2.waitKey(1) & 0xFF

    pixelValue = dst[2, 78]
    print 'value = ' + str(pixelValue)

    rawCapture.truncate(0)
  
    if key == ord("q"):
        break
