# Bird-s-eye-view
Master thesis Bird´s-eye view with human detection


Calibration

1. Camera calibration
  - Is done once for each camera
  - Capture 9 images of a chessboard with the chessboard in varied positions and angles. Together covering the field of view of the camera.
  - Run cameraCalibration.py for each camera with corresponding images.
  - Outputs camera calibration parameters, to be used in warping calibration.
  
2. Warping calibration
  - Is done once for each camera
  - Capture one image with a chessboard placed on the floor directly below the camera.
  - For each corner of the vehicle. Place a chessboard on the floor in the overlapping area and capture one image per adjecent camera. I.e. two images per corner. This should result in 8 images. For each camera there should be  two images, one with the chessboard far to the left and one with the chessboard far to the right.
  - Run warpingCalibration for each camera with corresponding images and camera paremeters from camera calibration. 
  - Outputs undistortion parameters map1 and map2, topview homography and two top-view images, to be used in the camera threads.

3. Merge calibration
  - Is done once
  - Run mergeCalibrationWarp.py with the 8 top view images from Warping calibration
  - Outputs four stitch homography and four masks, to be used in the merge thread. 

OBS! make sure the chessboard size in the code matches the size of the chessboard in the images, for each part of the calibration.

If the position of the cameras on the vehicle or the angle of the cameras would be changed the warping calibration and merge calibration has to be re-done.
