import cv2
print("OpenCV version:")
print(cv2.__version__[0])
assert cv2.__version__[0] >= '3', 'The fisheye module requires opencv version >= 3.0.0'
import numpy as np
import os
import glob

CHECKERBOARD = (6,8)
subpix_criteria = (cv2.TERM_CRITERIA_EPS+cv2.TERM_CRITERIA_MAX_ITER, 30, 0.1)
calibration_flags = cv2.fisheye.CALIB_RECOMPUTE_EXTRINSIC+cv2.fisheye.CALIB_FIX_SKEW#+cv2.fisheye.CALIB_CHECK_COND
objp = np.zeros((1, CHECKERBOARD[0]*CHECKERBOARD[1], 3), np.float32)
objp[0,:,:2] = np.mgrid[0:CHECKERBOARD[0], 0:CHECKERBOARD[1]].T.reshape(-1, 2)
_img_shape = None
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.
images = glob.glob('/home/ccs/Pictures/Cam4Calib/2021-03-26/*.png')
for fname in images:
    img = cv2.imread(fname)
    if _img_shape == None:
        _img_shape = img.shape[:2]
    else:
        assert _img_shape == img.shape[:2], "All images must share the same size."
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

    # Find the chess board corners
    ret, corners = cv2.findChessboardCorners(gray, CHECKERBOARD)
    
    # If found, add object points, image points (after refining them)
    if ret == True:
        objpoints.append(objp)
        cv2.cornerSubPix(gray,corners,(3,3),(-1,-1),subpix_criteria)
        imgpoints.append(corners)
    else:
         print("Corners not found")   
    cv2.waitKey(0)
N_OK = len(objpoints)
K = np.zeros((3, 3))
D = np.zeros((4, 1))
rvecs = [np.zeros((1, 1, 3), dtype=np.float64) for i in range(N_OK)]
tvecs = [np.zeros((1, 1, 3), dtype=np.float64) for i in range(N_OK)]
print("Found " + str(N_OK) + " valid images for calibration")

rms, _, _, _, _ = \
    cv2.fisheye.calibrate(
        objpoints,
        imgpoints,
        gray.shape[::-1],
        K,
        D,
        rvecs,
        tvecs,
        calibration_flags,
        (cv2.TERM_CRITERIA_EPS+cv2.TERM_CRITERIA_MAX_ITER, 30, 1e-6)
    )
print("rms=" + str(rms))    
print("DIM=" + str(_img_shape[::-1]))
print("CameraMatrix = np.array(" + str(K.tolist()) + ")")
print("Distortion coefficients = np.array(" + str(D.tolist()) + ")")
print("rvecs=" + str(rvecs))  
print("tvecs=" + str(tvecs))  



file_name = raw_input ("Enter filename(without file ending): ")
s = cv2.FileStorage('/home/ccs/Documents/'+file_name+'.xml', cv2.FileStorage_WRITE)
s.write('rms', rms)
s.write('cameraMatrix', K)
s.write('distCoeffs', D)
s.release()
print("File saved: /home/ccs/Documents/" + file_name + ".xml") 



