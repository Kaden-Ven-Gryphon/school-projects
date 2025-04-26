#####################################################################
# File:         hands.py                                            #
# Author:       Bernard Allotey, Kaden Gryphon                      #
# Description:  takes in an image and returns the location of hand  #
#               landmarks                                           #
#####################################################################
from cv2 import QRCodeDetector, cv2
import mediapipe as mp
from google.protobuf.json_format import MessageToJson
import json
from time import time_ns as ns

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_hands = mp.solutions.hands


def process_hands(image, indicate=False):
    """
    Processes the hands in the image

    :param image: The image to process
    :param indicate: Whether to indicate the hands
    :return: A json string of the hand information, unless indicate is true, then it returns the image as well
    """
    with mp_hands.Hands(
            model_complexity=0,
            min_detection_confidence=0.5,
            min_tracking_confidence=0.5
    ) as hands:
        returnImg = image
        image.flags.writeable = False
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = hands.process(image)
        mhl = results.multi_hand_landmarks
        handedness = results.multi_handedness
        if not mhl:
            return cv2.flip(returnImg, 1), None
        mhl_json = {'multiHandLandmarks': []}
        for hand_landmarks in mhl:
            #print('hand_landmarks:', hand_landmarks)
            if indicate:
                image.flags.writeable = True
                mp_drawing.draw_landmarks(
                    returnImg,
                    hand_landmarks,
                    mp_hands.HAND_CONNECTIONS,
                    mp_drawing_styles.get_default_hand_landmarks_style(),
                    mp_drawing_styles.get_default_hand_connections_style())
        if handedness[0].classification[0].label == 'Right':
            #print((handedness[0].classification[0].index))
            #print(len(handedness))
            return cv2.flip(returnImg, 1), mhl[0]
        elif len(handedness) == 2:
            return cv2.flip(returnImg, 1), mhl[(handedness[1].classification[0].index)]
        else:
            return cv2.flip(returnImg, 1), None


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    while cap.isOpened():
        success, img = cap.read()
        if not success:
            print("Ignoring empty camera frame.")
            break
        x, y = process_hands(img, True)
        if y != "":
            print(y)
        cv2.imshow("img", x)
        cv2.waitKey(17)
