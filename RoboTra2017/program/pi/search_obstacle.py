# import the necessary packages
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2
import numpy as np
#<br>#camera resolution
RESOLUTION_X = 640/2
RESOLUTION_Y = 480/2
 
cv2.namedWindow("Frame1")
#cv2.namedWindow("Frame2")
cv2.namedWindow("Frame3")
 
hsv = np.zeros((RESOLUTION_X,RESOLUTION_Y))
#<br>#mouse event functoion
def mouse_event(event, x, y, flags, param):
    if event == cv2.EVENT_LBUTTONDOWN:
        global hsv
        print(hsv[y][x])
 
#set mous event<br>cv2.setMouseCallback("Frame", mouse_event)
 
camera = PiCamera()
camera.resolution = (RESOLUTION_X, RESOLUTION_Y)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(RESOLUTION_X, RESOLUTION_Y))
  
# allow the camera to warmup
time.sleep(0.1)
 
# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    image = frame.array
    #mask
    lower_white = np.array([150, 150, 150])
    upper_white = np.array([255, 255, 255])
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    img_dst = cv2.inRange(image, lower_white, upper_white)
    #horizon analysis
    horizon = np.zeros(RESOLUTION_X)
    for k in range(0,RESOLUTION_Y):
        horizon += img_dst[k]
    amax = np.argmax(horizon)
    print(amax)
    #convert grau->rgb
    ans_image = cv2.cvtColor(img_dst, cv2.COLOR_GRAY2RGB)
    #calc hist
    hist = cv2.calcHist([img_dst],[0],None,[256],[0,256])
    if hist[255] > 1000 :
        cv2.circle(ans_image, (amax,  RESOLUTION_Y), 10, (0, 0, 255), -1)
 
    cv2.imshow("Frame1", ans_image)
#    cv2.imshow("Frame2", img_dst)
    cv2.imshow("Frame3", image)
    key = cv2.waitKey(1) & 0xFF
  
    rawCapture.truncate(0)
  
    if key == ord("q"):
        break
