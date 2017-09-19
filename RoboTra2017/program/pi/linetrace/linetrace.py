# coding:utf-8
import cv2
import numpy as np
#import scipy as sp
#import matplotlib.pyplot as plt
#import scipy.optimize
#from scipy import stats
#from numpy import *
#import math

def lsmangle(img):
	Px=[]
	Py=[]

	gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
	edges=cv2.Canny(gray,50,150)

	for x in xrange(len(edges)):
		for y,edge in enumerate(edges[x]):
			if edge!=0:
				Px.append(x)
				Py.append(y)

#for (a,b) in zip(Px,Py):
#print("%d,%d"%(a,b))

#				res=scipy.stats.linregress(Px, Py)
#				angle=math.atan(res[0])*180/np.pi

	return angle

#print edges[0][0]


img = cv2.imread("course03.png")
#lsmangle(img)
print "angle=%f[deg]"%lsmangle(img)
