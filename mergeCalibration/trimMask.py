import cv2
assert cv2.__version__[0] >= '3', 'The fisheye module requires opencv version >= 3.0.0'
import numpy as np
import os
import glob



def trimMask(mask1, mask2, corner):
    #Since the max value is 225 the masks 
    maskOverlap1_2 = (mask1/2+mask2/2)

    ret, maskOverlap1_2 = cv2.threshold(maskOverlap1_2, 200, 255, cv2.THRESH_BINARY) 

    # Find skeleton of mask
    img = maskOverlap1_2.copy()
    size = np.size(img)
    skel = np.zeros(img.shape,np.uint8)
    ret,img = cv2.threshold(img,5,255,0)
    element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3))
    done = False
    while( not done):
        eroded = cv2.erode(img,element)
        temp = cv2.dilate(eroded,element)
        temp = cv2.subtract(img,temp)
        skel = cv2.bitwise_or(skel,temp)
        img = eroded.copy() 
        zeros = size - cv2.countNonZero(img)
        if zeros==size: done = True
    kernel = np.ones((2,2), np.uint8)
    skel = cv2.dilate(skel, kernel, iterations=1)
    skeleton_contours, _ = cv2.findContours(skel, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    largest_skeleton_contour = max(skeleton_contours, key=cv2.contourArea)
    c = largest_skeleton_contour

    cnts, _ = cv2.findContours(maskOverlap1_2.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contour = max(cnts, key=cv2.contourArea)
    M = cv2.moments(contour)
    centroid_x = int(M['m10']/M['m00'])
    centroid_y = int(M['m01']/M['m00'])
    
    # Find center point and inner corner of overlapping mask 
    if corner == 1 or corner == 4:
        point1 = tuple([centroid_x,centroid_y])
        point2 = tuple(c[c[:, :, 1].argmax()][0])
    else:
        point1 = tuple(c[c[:, :, 1].argmin()][0])
        point2 = tuple([centroid_x,centroid_y])

    #Draw points
    maskOverlap1_2 = cv2.cvtColor(maskOverlap1_2,cv2.COLOR_GRAY2RGB)
    cv2.circle(maskOverlap1_2, point1, 8, (0, 0, 255), -1)
    cv2.circle(maskOverlap1_2, point2, 8, (0, 255, 0), -1)

    # cv2.imshow('Mask points', maskOverlap1_2) 
    # cv2.waitKey(0) 

    # Fit a line to the two points
    rows,cols = maskOverlap1_2.shape[:2]
    k = float(point1[1]-point2[1])/(point1[0]-point2[0])
    m = point2[1]-k*point2[0]

    # Find boundry of mask to remove overlap
    #min x
    y1 = k*0+m
    #max x
    y2 = k*(cols-1)+m
    #min y
    x1 = (0-m)/k
    #max y
    x2 = ((rows-1)-m)/k

    if corner == 1 or corner == 3:
        maskBoundry = np.array([[0,0]])
        if y1 <= (rows-1):
            lineStartPoint = (0, int(y1))
        else:
            lineStartPoint = (int(x2) ,rows-1)
            maskBoundry = np.append(maskBoundry,[[0,rows-1]], axis=0)

        if x1 <= (cols-1):
            lineEndpoint = (int(x1), 0)
        else:
            lineEndpoint= (cols-1, int(y2))
            maskBoundry = np.insert(maskBoundry,0,[[cols-1,0]], axis=0)

    else:
        maskBoundry = np.array([[0,rows-1]])
        if y1 >= 0:
            lineStartPoint = (0, int(y1))
            
        else:
            lineStartPoint = (int(x1) ,0)
            maskBoundry = np.append(maskBoundry,[[0,0]], axis=0)
        if x2 <= cols-1:
            lineEndpoint = (int(x2),rows-1)
            
        else :
            lineEndpoint = ( cols-1, int(y2))
            maskBoundry = np.insert(maskBoundry,0,[[cols-1,rows-1]], axis=0)

    maskBoundry = np.append(maskBoundry,[lineStartPoint], axis=0)
    maskBoundry = np.append(maskBoundry,[lineEndpoint], axis=0)


    cv2.circle(maskOverlap1_2, lineStartPoint, 12, (0, 0, 255), -1)
    cv2.circle(maskOverlap1_2, lineEndpoint, 12, (0, 255, 0), -1)
 
    maskOverlap1_2 = cv2.line(maskOverlap1_2,lineStartPoint,lineEndpoint,(0,0,255),1)
    mask1 = cv2.line(mask1,lineStartPoint,lineEndpoint,(0,0,255),1)
    mask2 = cv2.line(mask2,lineStartPoint,lineEndpoint ,(0,0,255),1)

    # cv2.namedWindow('maskOverlap1_2', cv2.WINDOW_KEEPRATIO)
    # cv2.imshow('maskOverlap1_2',maskOverlap1_2)  
    # cv2.imshow('mask1  ',mask1) 
    # cv2.imshow('mask2  ',mask2) 

    
    # Append mask boundry
    tempMask = np.zeros((rows,cols), np.uint8)
    _=cv2.drawContours(tempMask, np.int32([maskBoundry]),0, 255, -1)

    if corner == 1:
        mask1[tempMask==0] = 0
        mask2[tempMask>0] = 0
    else:
        mask1[tempMask>0] = 0
        mask2[tempMask==0] = 0

          
    # cv2.imshow('mask1 removed overlap ',mask1) 
    # cv2.imshow('mask2 removed overlap ',mask2) 
    cv2.waitKey(0)

    cv2.destroyAllWindows()

    return mask1, mask2