import cv2 
assert cv2.__version__[0] >= '3', 'The fisheye module requires opencv version >= 3.0.0'
import numpy as np
import os
import glob

from trimMask import *

chessboardSize = (6,4) #6,4   8,14  8,7
padBottom = 500
padRight = 0

####################### Read images ########################################
img1_left = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam1_L.png')
img1_right = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam1_R.png')
img2_left = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam2_L.png') 
img2_right = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam2_R.png') 
img3_left = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam3_R.png')
img3_right = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam3_L.png') 
img4_left = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam4_L.png') 
img4_right = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-14/topViewImgCam4_R.png') 



####################### Right corner ########################################


#Add padding to images to fit all images in one
img1_right = cv2.copyMakeBorder( img1_right, 0, padBottom, padRight/2, padRight/2, cv2.BORDER_CONSTANT)#top, bottom, left, right, borderType)
img2_left = cv2.copyMakeBorder( img2_left, 0, padBottom, 0,  padRight, cv2.BORDER_CONSTANT)

cv2.imshow("img1", img1_right)
cv2.waitKey(0)

ret1, img1_corners_temp = cv2.findChessboardCorners(img1_right, chessboardSize, None) 

ret2, img2_corners_temp = cv2.findChessboardCorners(img2_left, chessboardSize, None) 


#Get homography frim img2 to img1
H_img2, M = cv2.findHomography(img2_corners_temp, img1_corners_temp)

cv2.destroyAllWindows()

#Switch out image2 to move chessboard
img2_right = cv2.copyMakeBorder( img2_right, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)

#warp image2 new
rows,cols,ch = img2_right.shape
img2_right = cv2.warpPerspective(img2_right, H_img2, (cols, rows))

cv2.imshow("img2_right", img2_right)
cv2.waitKey(0)
########################### Left corner ####################################

#Add padding to images to fit all images in one
img1_left = cv2.copyMakeBorder( img1_left, 0, padBottom, padRight/2, padRight/2, cv2.BORDER_CONSTANT)#top, bottom, left, right, borderType)
img4_right = cv2.copyMakeBorder( img4_right, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)

ret1_L, img1_left_corners = cv2.findChessboardCorners(img1_left, chessboardSize, None) 
ret4_R, img4_corners = cv2.findChessboardCorners(img4_right, chessboardSize, None)
print("corner1 left", ret1_L)
print("corner4 right", ret4_R)

#Get homography from img4 to img1
H_img4, M = cv2.findHomography(img4_corners, img1_left_corners)

#Switch out image4 to move chessboard
img4_left = cv2.copyMakeBorder( img4_left, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)

#warp img4_left
rows,cols,ch = img4_left.shape
img4_left = cv2.warpPerspective(img4_left, H_img4, (cols, rows))


##################### Bottom Image ################################

#Right corners img1_2_4
ret4_L, left_corners = cv2.findChessboardCorners(img4_left, chessboardSize, None)
print("corner4 left", ret1_L)


#Left corners img3
img3_left = cv2.copyMakeBorder( img3_left, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)
#TEST# 
rotation_180 = np.eye(3,3)
rotation_180[:,:-1]= rotation_180[:,:-1]*(-1) 
rotation_180[0,2] = 640
rotation_180[1,2] = 480
rows,cols,ch = img3_left.shape
img3_left = cv2.warpPerspective(img3_left, rotation_180, (cols, rows))

#TEST#
ret3_L, left_corners_img3 = cv2.findChessboardCorners(img3_left, chessboardSize, None)
print("corner3 left", ret3_L)


#Right corners img3
img3_right = cv2.copyMakeBorder( img3_right, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)
#TEST#
rotation_180 = np.eye(3,3)
rotation_180[:,:-1]= rotation_180[:,:-1]*(-1) 
rotation_180[0,2] = 640
rotation_180[1,2] = 480
rows,cols,ch = img3_right.shape
img3_right = cv2.warpPerspective(img3_right, rotation_180, (cols, rows))
 
#TEST#

ret3_R, right_corners_img3 = cv2.findChessboardCorners(img3_right, chessboardSize, None)
print("corner3 right", ret3_R)

#Right corners img1_2_4
ret2_R, right_corners =cv2.findChessboardCorners(img2_right, chessboardSize, None)
print("corner2 right", ret2_R)

#Corners img3
img3_corners = np.concatenate((right_corners_img3,left_corners_img3), axis=0)

#corners img1_2_4
corners = np.concatenate((right_corners,left_corners), axis=0)

#Get homography frim img3 to img1_2_4
H_img3, M = cv2.findHomography(img3_corners, corners)

rows,cols,ch = img3_right.shape
img3_right = cv2.warpPerspective(img3_right, H_img3, (cols, rows))



################ Create masks ###################
# create mask for img1
img1gray = cv2.cvtColor(img1_right,cv2.COLOR_BGR2GRAY)
ret, mask_img1 = cv2.threshold(img1gray, 10, 255, cv2.THRESH_BINARY)

#create mask for image2
img2gray = cv2.cvtColor(img2_right,cv2.COLOR_BGR2GRAY)
ret, mask_img2 = cv2.threshold(img2gray, 10, 255, cv2.THRESH_BINARY)
img2_right = cv2.bitwise_and(img2_right,img2_right,mask = mask_img2)

# create mask for image3
img3gray = cv2.cvtColor(img3_right,cv2.COLOR_BGR2GRAY)
ret, mask_img3 = cv2.threshold(img3gray, 10, 255, cv2.THRESH_BINARY)
img3_right = cv2.bitwise_and(img3_right,img3_right,mask = mask_img3)

#create mask for img4
img4gray = cv2.cvtColor(img4_left,cv2.COLOR_BGR2GRAY)
ret, mask_img4 = cv2.threshold(img4gray, 10, 255, cv2.THRESH_BINARY)
img4_left = cv2.bitwise_and(img4_left,img4_left,mask = mask_img4)

############# Trim masks ################
#Trim the masks to remove overlapping area between the masks
corner = 1 
mask_img1, mask_img2 = trimMask(mask_img1, mask_img2, corner)

corner = 2
mask_img2, mask_img3 = trimMask(mask_img2, mask_img3, corner)

corner = 3
mask_img3, mask_img4 = trimMask(mask_img3, mask_img4, corner)

corner = 4
mask_img1, mask_img4 = trimMask(mask_img1, mask_img4, corner)



#################### Apply masks ##########################

img1_right = cv2.bitwise_and(img1_right,img1_right,mask = mask_img1)
img2_right = cv2.bitwise_and(img2_right,img2_right,mask = mask_img2)
img3_right = cv2.bitwise_and(img3_right,img3_right,mask = mask_img3)
img4_left = cv2.bitwise_and(img4_left,img4_left,mask = mask_img4)


#################### Merge images ############################
# Merge img1 and img2
img1_2 = cv2.add(img1_right,img2_right)
cv2.imshow("img1_2_new", img1_2)
cv2.waitKey(0)

# Merge img4 and yo img1_2
img1_2_4 = cv2.add(img4_left,img1_2)
cv2.imshow("img1_2_4_new", img1_2_4)
cv2.waitKey(0)


# Merge img3 and with the total image
img1_2_4_3 = cv2.add(img1_2_4,img3_right)
cv2.imshow('img1_2_4_3',img1_2_4_3)
cv2.waitKey(0)




# #####Save to file######
H_img1 = np.eye(3,3)

#Add rotation to the translation
rotation_180 = np.eye(3,3)
rotation_180[:,:-1]= rotation_180[:,:-1]*(-1) 
rotation_180[0,2] = 640
rotation_180[1,2] = 480
H_img3 = np.dot(H_img3,rotation_180)


file_name = raw_input ("Enter filename(without file ending): ")
s = cv2.FileStorage('/home/ccs/Pictures/mergeParams/'+file_name+'.xml', cv2.FileStorage_WRITE)
s.write('stitchH1', H_img1)
s.write('stitchH2', H_img2)
s.write('stitchH3', H_img3)
s.write('stitchH4', H_img4)

s.write('mask1', mask_img1)
s.write('mask2', mask_img2)
s.write('mask3', mask_img3)
s.write('mask4', mask_img4)

s.release()
print("File saved: /home/ccs/Pictures/mergeParams/" + file_name + ".xml") 


# ############### Test #########################

test1 = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-20/topViewImgCam1.png') 
test2 = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-20/topViewImgCam2.png') 
test3 = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-20/topViewImgCam3.png') 
test4 = cv2.imread('/home/ccs/Pictures/topViewImages/2021-04-20/topViewImgCam4.png') 


test1 = cv2.copyMakeBorder( test1, 0, padBottom, padRight/2, padRight/2, cv2.BORDER_CONSTANT)
test2 = cv2.copyMakeBorder( test2, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)
test3 = cv2.copyMakeBorder( test3, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)
test4 = cv2.copyMakeBorder( test4, 0, padBottom, 0, padRight, cv2.BORDER_CONSTANT)


#Warp image2
rows,cols,ch = test2.shape
test2 = cv2.warpPerspective(test2, H_img2, (cols, rows))

#Warp image3
rows,cols,ch = test3.shape
test3 = cv2.warpPerspective(test3, H_img3, (cols, rows))


#Warp image4
rows,cols,ch = test4.shape
test4 = cv2.warpPerspective(test4, H_img4, (cols, rows))


# Apply masks
test1 = cv2.bitwise_and(test1,test1,mask = mask_img1)
test2 = cv2.bitwise_and(test2,test2,mask = mask_img2)
test3 = cv2.bitwise_and(test3,test3,mask = mask_img3)
test4 = cv2.bitwise_and(test4,test4,mask = mask_img4)

# Merge img1 and masked img2
img = cv2.add(test1,test2)
cv2.imshow('img',img) 
cv2.waitKey(0) 


# Merge img and masked img3 
img = cv2.add(img,test3)
cv2.imshow('img',img)
cv2.waitKey(0)


# Merge img and masked img4
img = cv2.add(img,test4)
cv2.imshow('img',img)
cv2.waitKey(0)


file_name = raw_input ("Enter filename(without file ending): ")
cv2.imwrite('/home/ccs/Pictures/stitchedImages/test/'+file_name+'.png', img)

print("/home/ccs/Pictures/stitchedImages/" + file_name + ".png") 