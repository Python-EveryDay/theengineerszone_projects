import cv2
import mediapipe as mp
import vgamepad as vg

gamepad = vg.VX360Gamepad()

cap = cv2.VideoCapture(1)

# Reduce latency
cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

# Lower resolution
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)

mp_hands = mp.solutions.hands

hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=1,
    model_complexity=0,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)

prev_steering = 0
frame_count = 0

while True:

    success, img = cap.read()

    if not success:
        break

    # Skip alternate frames
    frame_count += 1
    if frame_count % 2 != 0:
        continue

    img = cv2.flip(img, 1)
    
    # Smaller processing frame
    small = cv2.resize(img, (240, 180))
    #       cv2.imshow("Hand Tracking", small)
    imgRGB = cv2.cvtColor(small, cv2.COLOR_BGR2RGB)

    results = hands.process(imgRGB)

    h, w, c = small.shape

    if results.multi_hand_landmarks:

        handLms = results.multi_hand_landmarks[0]

        lm = handLms.landmark[9]

        cx = int(lm.x * w)

        raw_steering = int(
            ((cx - w//2) / (w//2)) * 32767
        )

        # Lower sensitivity
        raw_steering = int(raw_steering * 0.6)

        # Faster smoother steering
        steering = int(
            prev_steering * 0.7 +
            raw_steering * 0.3
        )
        #print(f"Steering: {steering}")
        # Deadzone
        if abs(steering) < 3000:
            steering = 0

        prev_steering = steering

        gamepad.left_joystick(
            x_value=steering,
            y_value=0
        )

        gamepad.update()

    # Remove imshow for max FPS
        
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()