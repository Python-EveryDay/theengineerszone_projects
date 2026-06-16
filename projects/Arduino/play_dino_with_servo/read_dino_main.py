import cv2
import numpy as np
import pyautogui
import mss
import time

# Start the game manually and place the browser in front
print("Starting in 3 seconds...")
time.sleep(3)

# Detection area in front of dino
# Adjust according to your screen resolution
monitor = {
        "top": 440,
        "left": 300,
        "width": 300,
        "height": 300
    }

with mss.mss() as sct:

    while True:
        img = np.array(sct.grab(monitor))

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Binary threshold
        _, thresh = cv2.threshold(
                gray,
                100,    # adjust if needed
                255,
                cv2.THRESH_BINARY
            )
        obstacle_pixels = cv2.countNonZero(thresh) 
        background_pixel = gray[5, 5]
        print(f"Background pixel value: {background_pixel}, gray[5, 5]: {gray[5, 5]}")
        if background_pixel < 100:
            #print("Dark mode")
               
            #print(f"Dark mode:Light Obstacle pixels: {obstacle_pixels}")            
            if obstacle_pixels > 1500:   
                pyautogui.press("space")
                print("Jumped! Dark", "obstacle_pixels :", obstacle_pixels)
                time.sleep(0.05)
        else:                            
            print(f"Light mode: Dark Obstacle pixels: {obstacle_pixels}")            
            if obstacle_pixels > 50000:
                pyautogui.press("space")
                print("Jumped! Light", "obstacle_pixels :", obstacle_pixels    )
                time.sleep(0.05)
            
        #cv2.imshow("Detection Area", thresh)

        if cv2.waitKey(1) == ord('q'):
            break

cv2.destroyAllWindows()

