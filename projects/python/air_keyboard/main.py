import cv2
import mediapipe as mp
import numpy as np
import pyautogui
import math
import time

# =========================
# MEDIAPIPE SETUP
# =========================
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(
    max_num_hands=1,
    model_complexity=0,
    min_detection_confidence=0.7,
    min_tracking_confidence=0.7
)

mp_draw = mp.solutions.drawing_utils

# =========================
# KEYBOARD LAYOUT
# =========================
keys = [
    ["Q","W","E","R","T","Y","U","I","O","P"],
    ["A","S","D","F","G","H","J","K","L"],
    ["Z","X","C","V","B","N","M"]
]

key_width = 50
key_height = 50

last_click_time = 0
click_delay = 0.5

# =========================
# CAMERA
# =========================
cap = cv2.VideoCapture(0)

# Balanced resolution for laptop webcam
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1024)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 576)


# =========================
# MAIN LOOP
# =========================
while True:

    success, frame = cap.read()

    if not success:
        break

    frame = cv2.flip(frame, 1)

    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    results = hands.process(rgb)

    finger_x, finger_y = 0, 0

    clicking = False

    # =========================
    # HAND DETECTION
    # =========================
    if results.multi_hand_landmarks:

        for hand_landmarks in results.multi_hand_landmarks:

            mp_draw.draw_landmarks(
                frame,
                hand_landmarks,
                mp_hands.HAND_CONNECTIONS
            )

            h, w, c = frame.shape

            # INDEX FINGER
            index_tip = hand_landmarks.landmark[8]

            finger_x = int(index_tip.x * w)
            finger_y = int(index_tip.y * h)

            # THUMB
            thumb_tip = hand_landmarks.landmark[4]

            thumb_x = int(thumb_tip.x * w)
            thumb_y = int(thumb_tip.y * h)

            # DRAW CURSORS
            cv2.circle(frame, (finger_x, finger_y), 15, (0,255,255), -1)
            cv2.circle(frame, (thumb_x, thumb_y), 15, (255,0,255), -1)

            # DISTANCE BETWEEN FINGERS
            distance = math.hypot(
                thumb_x - finger_x,
                thumb_y - finger_y
            )

            # PINCH DETECTION
            if distance < 30:
                clicking = True

    # =========================
    # DRAW KEYBOARD
    # =========================
    from theengineerszone_projects.projects.python.air_keyboard.draw_keyboard import draw_keyboard
    pressed_key = draw_keyboard(
        frame,
        finger_x,
        finger_y,
        clicking
    )

    # =========================
    # TYPE KEY
    # =========================
    current_time = time.time()

    if clicking and pressed_key:

        if current_time - last_click_time > click_delay:

            pyautogui.press(pressed_key.lower())

            print("Pressed:", pressed_key)

            last_click_time = current_time

    # =========================
    # SHOW WINDOW
    # =========================
    cv2.namedWindow("Air Keyboard", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("Air Keyboard", 1024, 576)
    cv2.imshow("Air Keyboard", frame)

    # ESC TO EXIT
    if cv2.waitKey(1) & 0xFF == 27:
        break
cap.release()
cv2.destroyAllWindows()