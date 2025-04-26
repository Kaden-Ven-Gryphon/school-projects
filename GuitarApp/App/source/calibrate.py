#############################################################################
# File:         calibrate.py                                                #
# Author:       Kaden Gryphon                                               #
# Description:  This will take a short video where the user holds a         #
#               checker board of known size and it will generate a          #
#               camera matrix, and distortion coefficants which are         #
#               saved to a file "calibration.json"                          #
#https://longervision.github.io/2017/03/16/ComputerVision/OpenCV/opencv-internal-calibration-chessboard/
#############################################################################

import numpy as np
import cv2
import json

#termination cirt
crit = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

objp = np.zeros((6*7,3), np.float32)
objp[:,:2] = np.mgrid[0:7,0:6].T.reshape(-1,2)

objpoints = []
imgpoints = []

cap = cv2.VideoCapture(0)
found = 0
while(found < 10):
    ret, img = cap.read()
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ret, corners = cv2.findChessboardCorners(gray, (7, 6), None)

    if ret == True:
        objpoints.append(objp)
        corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), crit)
        imgpoints.append(corners2)

        img = cv2.drawChessboardCorners(img, (7, 6), corners2, ret)
        found += 1

    cv2.imshow('img', img)
    cv2.waitKey(50)


cap.release()
cv2.destroyAllWindows()

ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

data = {'camera_mat': np.asarray(mtx).tolist(), 'distortion_coeff': np.asarray(dist).tolist()}

json_string = json.dumps(data)
with open('calibration.json', 'w') as outfile:
    json.dump(json_string, outfile)
