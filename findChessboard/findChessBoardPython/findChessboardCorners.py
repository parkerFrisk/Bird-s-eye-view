import cv2
import numpy as np
import os
import glob


img1 = cv2.imread('img1.png')
img2 = cv2.imread('img2.png') 

ret1, img1_corners = cv2.findChessboardCorners(img1, (4,3), None) 
ret2, img2_corners = cv2.findChessboardCorners(img2, (4,3), None) 

print("success img1: " + str(ret1)) 
print("img1_corners: " + str(img1_corners))

print("success img2: " + str(ret2))  
print("img2_corners: " + str(img2_corners)) 