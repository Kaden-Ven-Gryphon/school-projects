#################################################
# File:		guitarApp.py			#
# Author:	Kaden Gryphon             	#
# Description:	The main loop for the app	#
#################################################

from cv2 import cv2
import numpy as np
from hands import process_hands
import gtoiTransform
import json
import pygame
import simpleaudio
import math
import random

pygame_surface_cache = {}

def convert_cv2_img_to_pygame(cv_img):
	"""
	Converts BGR to RGB

	:param cv_img: the BGR image
	"""
	rgb_img = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB).swapaxes(0,1)

	cache_key = rgb_img.shape
	cached_surface = pygame_surface_cache.get(cache_key)

	if cached_surface is None:
		cached_surface = pygame.surfarray.make_surface(rgb_img)
		pygame_surface_cache[cache_key] = cached_surface
	else:
		pygame.surfarray.blit_array(cached_surface, rgb_img)
		
	return cached_surface

def get_chord_g_points(chord_index, chords, fret_centers, strings_y_chords):
	#get guitar chords for chord
	chord_point_finger_list = []
	chord_bar_g_points = np.zeros((2,3))
	chord_number_of_fingers = 0
	chord_finger_g_points = np.zeros((6,3))
	chord_bar_fret = int(chords[chord_index][2])
	if chord_bar_fret != 0:
		chord_bar_g_points[0,0] = fret_centers[chord_bar_fret-1]
		chord_bar_g_points[1,0] = fret_centers[chord_bar_fret-1]
		chord_bar_g_points[0,1] = -2
		chord_bar_g_points[1,1] = -5
	else:
		chord_bar_g_points = None
	for i in range(0,6):
		chord_finger_fret = int(chords[chord_index][3+i])
		chord_finger_number = int(chords[chord_index][9+i])
		if chord_finger_fret != -1 and chord_finger_fret !=0 and chord_finger_fret != chord_bar_fret:
			chord_finger_g_points[chord_number_of_fingers, 0] = fret_centers[chord_finger_fret-1]
			chord_finger_g_points[chord_number_of_fingers, 1] = strings_y_chords[i]
			chord_number_of_fingers = chord_number_of_fingers + 1
			if chord_finger_number != -1:
				chord_point_finger_list.append(chord_finger_number)
	return chord_bar_g_points, chord_point_finger_list, chord_finger_g_points


def main():
	#set up sounds
	strong_beat_sa = simpleaudio.WaveObject.from_wave_file('../sounds/strong_beat.wav')
	weak_beat_sa = simpleaudio.WaveObject.from_wave_file('../sounds/weak_beat.wav')

	#set colors
	color_button_light = (170,170,170)
	color_button_dark = (100,100,100)
	color_white = (255,255,255)

	color_cv2_blue = (255,0,0)
	color_cv2_green = (0,255,0)
	color_cv2_red = (0,0,255)
	color_cv2_cyan = (255,255,0)
	color_cv2_yellow = (0,255,255)
	color_cv2_magenta = (255,0,255)

	

	
	#set up pygame
	pygame.init()
	clock = pygame.time.Clock()
	
	#set up camera
	capture = cv2.VideoCapture(0)
	
	#find the height and width of camera
	_, frame = capture.read()
	width = frame.shape[1]
	height = frame.shape[0]

	#set up pygame window to be same height and width
	window = pygame.display.set_mode((width+200, height))
	pygame.display.set_caption("Guitar App")

	
	#set up font
	smallfont = pygame.font.SysFont('Corbel',35)
	

	#get the camera calibration data
	with open('../resources/calibration.json') as json_file:
		data = json.load(json_file)
	data_dic = json.loads(data)
	camera_matrix = np.asarray(data_dic['camera_mat'])
	dist_coeffs = np.asarray(data_dic['distortion_coeff'])

	#get the guitar poitns from file
	with open('../resources/fretslocations.txt') as f:
		lines = f.readlines()
	number_of_frets = 0
	frets_x_chords = []
	strings_y_chords = []
	stage = 0
	i = 0
	for line in lines:
		#ignore comments
		if line.startswith('#'):
			continue
		#get the number of frets
		if stage == 0:
			number_of_frets = int(line)
			stage = stage + 1
		#get to x chords of the frets
		elif stage == 1:
			frets_x_chords.append(float(line))
			i = i + 1
			if i >= number_of_frets:
				stage = stage + 1
		#get the y chords of the strings
		elif stage == 2:
			strings_y_chords.append(float(line))
	frets_x_chords.reverse()
	#strings_y_chords.reverse()
	#create the points for drawing the frets
	fret_g_points = np.zeros((number_of_frets * 2, 3))
	for i in range(0, number_of_frets*2, 2):
		fret_g_points[i, 0] = frets_x_chords[int(i/2)]
		fret_g_points[i+1, 0] = frets_x_chords[int(i/2)]
		fret_g_points[i,1] = -2
		fret_g_points[i+1, 1] = -5
	

	#get the guitar points for the finger markers
	fret_centers_g_points = np.zeros((number_of_frets))
	fret_centers_g_points[0] = frets_x_chords[0] + 0.8
	for i in range(1, 19):
		fret_centers_g_points[i] = (frets_x_chords[i-1] - frets_x_chords[i])/2 + frets_x_chords[i]


	#load in the list of chords
	with open('../resources/chords.txt') as f:
		lines = f.readlines()
	chords = []
	for line in lines:
		if line.startswith('#'):
			continue
		chord = line.split()
		chords.append(chord)
	print(chords)


	#set initial chord
	chord_index = 8
	chord_bar_g_points, chord_finger_list, chord_finger_g_points = get_chord_g_points(chord_index, chords, fret_centers_g_points, strings_y_chords)
	text_chord_name = smallfont.render('Chord: ' + chords[chord_index][1], True, color_white)
	
	met_temp_bpm = 120
	met_temp_ms = int((60/met_temp_bpm)*1000)
	met_beats_per_measure = 4
	met_beat = 1
	met_last_tick = pygame.time.get_ticks()

        # Pre render text for button labels
	text_button_met = smallfont.render('Met', True, color_white)
	text_button_hands = smallfont.render('Hands', True, color_white)
	text_button_next = smallfont.render('Next', True, color_white)
	text_button_back = smallfont.render('Back', True, color_white)
	text_button_strings = smallfont.render('Strings', True, color_white)
	text_button_auto = smallfont.render('Auto', True, color_white)

	show_hands = False
	mute = True
	draw_strings = True
	auto_chords = False

	end_game = False
	while True:
		clock.tick(60)
		imgpts = None

		#event handling ######################################################
		for ev in pygame.event.get():
			if ev.type == pygame.QUIT:
				pygame.quit()
				end_game = True
				break
			if ev.type == pygame.MOUSEBUTTONDOWN:
				if width+10 <= mouse[0] <= width+190 and 10 <= mouse[1] <= 50:
					show_hands = not show_hands
				elif width+10 <= mouse[0] <= width+190 and 60 <= mouse[1] <= 100:
					mute = not mute
				elif width+10 <= mouse[0] <= width+190 and 110 <= mouse[1] <= 150:
					chord_index = chord_index + 1
					if chord_index > 20:
						chord_index = 20
					chord_bar_g_points, chord_finger_list, chord_finger_g_points = get_chord_g_points(chord_index, chords, fret_centers_g_points, strings_y_chords)
					text_chord_name = smallfont.render('Chord: ' + chords[chord_index][1], True, color_white)
				elif width+10 <= mouse[0] <= width+190 and 160 <= mouse[1] <= 200:
					chord_index = chord_index - 1
					if chord_index < 0:
						chord_index = 0
					chord_bar_g_points, chord_finger_list, chord_finger_g_points = get_chord_g_points(chord_index, chords, fret_centers_g_points, strings_y_chords)
					text_chord_name = smallfont.render('Chord: ' + chords[chord_index][1], True, color_white)
				elif width+10 <= mouse[0] <= width+190 and 210 <= mouse[1] <= 250:
					draw_strings = not draw_strings
				elif width+10 <= mouse[0] <= width+190 and 260 <= mouse[1] <= 310:
					auto_chords = not auto_chords
		
		if end_game:
			break
		mouse = pygame.mouse.get_pos()

		
		
		#get frame from camera #########################################
		success, frame = capture.read()
		if not success:
			print("Failed to read capture")
			break
		#get marker data from frame
		rvec, tvec = gtoiTransform.get_gtoi_matrix(frame, camera_matrix, dist_coeffs)
		#get finger data from frame
		image, finger_pos = process_hands(frame, show_hands)

		#get a list of the finger tip positions
		finger_tips = []
		
		if finger_pos is not None:
			finger_tips.append((int((-1*((finger_pos.landmark[8].x * width)-(width/2)))+(width/2)), int(finger_pos.landmark[8].y * height)))
			finger_tips.append((int((-1*((finger_pos.landmark[12].x * width)-(width/2)))+(width/2)), int(finger_pos.landmark[12].y * height)))
			finger_tips.append((int((-1*((finger_pos.landmark[16].x * width)-(width/2)))+(width/2)), int(finger_pos.landmark[16].y * height)))
			finger_tips.append((int((-1*((finger_pos.landmark[20].x * width)-(width/2)))+(width/2)), int(finger_pos.landmark[20].y * height)))
		else:
			finger_tips.append((0,0))
			finger_tips.append((0,0))
			finger_tips.append((0,0))
			finger_tips.append((0,0))

		
		
		if rvec is not None:
			#convert guitar fret point to img chords
			imgpts = gtoiTransform.guitar_to_image_transform(fret_g_points, rvec, tvec, camera_matrix, dist_coeffs, width=width)
			if chord_bar_g_points is not None:
				imgpts_chord_bar = gtoiTransform.guitar_to_image_transform(chord_bar_g_points, rvec, tvec, camera_matrix, dist_coeffs, width=width)
			imgpts_chord_fingers = gtoiTransform.guitar_to_image_transform(chord_finger_g_points, rvec, tvec, camera_matrix, dist_coeffs, width=width)

		sum = 0
		found = False
		
		if imgpts is not None:
			#draw frets on img
			for x in range(0,13,2):
				cv2.line(image, tuple(imgpts[x].ravel()), tuple(imgpts[x+1].ravel()), (0,0,255), 1)

			#draw chord bar
			if chord_bar_g_points is not None:
				cv2.line(image, tuple(imgpts_chord_bar[0].ravel()), tuple(imgpts_chord_bar[1].ravel()), (255,0,0), 5)

			
			#draw chord fingers
			#and calc the score
			for i in range(0,len(chord_finger_list)):
				found = True
				if chord_finger_list[i] == 1:
					buf_color = color_cv2_green
					distance = math.sqrt((imgpts_chord_fingers[i].ravel()[0] - finger_tips[0][0])**2 + ((imgpts_chord_fingers[i].ravel()[1] - finger_tips[0][1])**2))
					sum = sum + distance ** 2

					if draw_strings and finger_pos is not None:
						cv2.line(image, tuple(imgpts_chord_fingers[i].ravel()), finger_tips[0], buf_color, 1)
					
				elif chord_finger_list[i] == 2:
					buf_color = color_cv2_cyan
					distance = math.sqrt((imgpts_chord_fingers[i].ravel()[0] - finger_tips[1][0])**2 + ((imgpts_chord_fingers[i].ravel()[1] - finger_tips[1][1])**2))
					sum = sum + distance ** 2

					if draw_strings and finger_pos is not None:
						cv2.line(image, tuple(imgpts_chord_fingers[i].ravel()), finger_tips[1], buf_color, 1)
                                                
				elif chord_finger_list[i] == 3:
					buf_color = color_cv2_magenta
					distance = math.sqrt((imgpts_chord_fingers[i].ravel()[0] - finger_tips[2][0])**2 + ((imgpts_chord_fingers[i].ravel()[1] - finger_tips[2][1])**2))
					sum = sum + distance ** 2

					if draw_strings and finger_pos is not None:
						cv2.line(image, tuple(imgpts_chord_fingers[i].ravel()), finger_tips[2], buf_color, 1)
                                                
				elif chord_finger_list[i] == 4:
					buf_color = color_cv2_yellow
					distance = math.sqrt((imgpts_chord_fingers[i].ravel()[0] - finger_tips[3][0])**2 + ((imgpts_chord_fingers[i].ravel()[1] - finger_tips[3][1])**2))
					sum = sum + distance ** 2

					if draw_strings and finger_pos is not None:
						cv2.line(image, tuple(imgpts_chord_fingers[i].ravel()), finger_tips[3], buf_color, 1)
                                                
				else:
					buf_color = (0,0,0)
				cv2.circle(image, tuple(imgpts_chord_fingers[i].ravel()), radius=4, color=buf_color, thickness=-1)
				
		sum = int(sum / len(chord_finger_list))

		if auto_chords and found:
			if sum < 150:
				chord_index = random.randrange(0,21)
				chord_bar_g_points, chord_finger_list, chord_finger_g_points = get_chord_g_points(chord_index, chords, fret_centers_g_points, strings_y_chords)
				text_chord_name = smallfont.render('Chord: ' + chords[chord_index][1], True, color_white)



		#draw the finger tips
		cv2.circle(image,finger_tips[0], radius=4, color=color_cv2_green,thickness=-1)
		cv2.circle(image,finger_tips[1], radius=4, color=color_cv2_cyan,thickness=-1)
		cv2.circle(image,finger_tips[2], radius=4, color=color_cv2_magenta,thickness=-1)
		cv2.circle(image,finger_tips[3], radius=4, color=color_cv2_yellow,thickness=-1)
				
		#comvert cv2 img to RGB
		pyg_img = convert_cv2_img_to_pygame(image)
		#display frame
		window.fill((0,0,0))
		window.blit(pyg_img, (0,0))

		#add gui
		#hands button
		if width+10 <= mouse[0] <= width+190 and 10 <= mouse[1] <= 50:
			pygame.draw.rect(window,color_button_light,[width+10,10,180,40])
		else:
			pygame.draw.rect(window,color_button_dark,[width+10,10,180,40])
		#mute button
		if width+10 <= mouse[0] <= width+190 and 60 <= mouse[1] <= 100:
			pygame.draw.rect(window,color_button_light,[width+10,60,180,40])
		else:
			pygame.draw.rect(window,color_button_dark,[width+10,60,180,40])
		#chord up button
		if width+10 <= mouse[0] <= width+190 and 110 <= mouse[1] <= 150:
			pygame.draw.rect(window,color_button_light,[width+10,110,180,40])
		else:
			pygame.draw.rect(window,color_button_dark,[width+10,110,180,40])
		#chord down button
		if width+10 <= mouse[0] <= width+190 and 160 <= mouse[1] <= 200:
			pygame.draw.rect(window,color_button_light,[width+10,160,180,40])
		else:
			pygame.draw.rect(window,color_button_dark,[width+10,160,180,40])
		#strings button
		if width+10 <= mouse[0] <= width+190 and 210 <= mouse[1] <= 250:
			pygame.draw.rect(window,color_button_light,[width+10,210,180,40])
		else:
			pygame.draw.rect(window,color_button_dark,[width+10,210,180,40])
		#auto button
		if width+10 <= mouse[0] <= width+190 and 260 <= mouse[1] <= 310:
			pygame.draw.rect(window,color_button_light,[width+10,260,180,40])
		else:
			pygame.draw.rect(window,color_button_dark,[width+10,260,180,40])


		window.blit(text_button_hands, (width+12, 13))
		window.blit(text_button_met, (width+12, 63))
		window.blit(text_button_next, (width+12, 113))
		window.blit(text_button_back, (width+12, 163))
		window.blit(text_button_strings, (width+12, 213))
		window.blit(text_button_auto, (width+12, 263))

		#chord name
		window.blit(text_chord_name, (width+10, 320))
		#render error val
		text_chord_error = smallfont.render(str(sum), True, color_white)
		window.blit(text_chord_error, (width+10, 350))
		
		#publish frame
		pygame.display.update()

		#play met sounds
		if met_last_tick < pygame.time.get_ticks() - met_temp_ms:
			#print(pygame.time.get_ticks()-met_last_tick)
			met_last_tick = pygame.time.get_ticks()
			if met_beat == 1 and not mute:
				strong_beat_sa.play()
			elif not mute:
				weak_beat_sa.play()
			met_beat = met_beat + 1
			if met_beat > met_beats_per_measure:
				met_beat = 1
		
		

	capture.release()
	cv2.destroyAllWindows()
	pygame.quit()

if __name__ == "__main__":
	main()
