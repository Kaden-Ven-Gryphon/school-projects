#################################################################
# File:		gtoiTransform.py				#
# Author:	Kaden Gryphon					#
# Description:	This is a collection of functions to conver	#
#		guitar cordinates to image cordinates		#
#################################################################

import cv2
from cv2 import aruco
import numpy as np

def get_gtoi_matrix(img, camera_matrix, dist_coeffs, markerLength = 2.875):
	"""
	This gets the rvec and tvec matrixs from an image

	:param img: the img to find the matrics of
	:param camera_matrix: camera_matrix from calibration
	:param dist_coeffs: the camera's distortion coeffs from calibration
	:param makerLength: the real life length of the marker
	"""
	
	img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
	aruco_dict = aruco.Dictionary_get( aruco.DICT_4X4_250 )
	aruco_param = aruco.DetectorParameters_create()
	corners, marker_ids, rejected = aruco.detectMarkers(img_gray, aruco_dict, parameters=aruco_param)
	if marker_ids is not None:
		rvec, tvec, _objPoints = aruco.estimatePoseSingleMarkers(corners, markerLength, camera_matrix, dist_coeffs)
		return rvec, tvec
	else:
		return None, None


def guitar_to_image_transform(objpts, rvec, tvec, camera_matrix, dist_coeffs, mirror = True, width = 400):
	"""
	Transforms a set of points from guitar cords to image cords

	:param objpts: the points to be transformed, measured relative to the makers location and size
	:param rvec: from get_gtoi_matrix
	:param tvex: from get+gtoi_matrix
	:param camera_matrix: camera_matrix from calibration
	:param dist_coeffs: the camera's distortion coeffs from calibration
	"""
	if rvec.shape == np.zeros((1,1,3)).shape:
		imgpts, jac = cv2.projectPoints(objpts, rvec, tvec, camera_matrix, dist_coeffs)
		if mirror:
			#print(imgpts.shape)
			for i in range(0, imgpts.shape[0]):
				imgpts[i, 0, 0] = (-1*(imgpts[i, 0, 0]-(width/2)))+(width/2)
		return imgpts.astype(int)
	else:
		return None
