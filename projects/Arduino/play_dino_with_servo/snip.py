from matplotlib.pyplot import gray
import mss
import numpy as np
import cv2
import time
time.sleep(3)  # Wait for 3 seconds before starting
with mss.mss() as sct:
    monitor = {
        "top": 440,
        "left": 350,
        "width": 300,
        "height": 300
    }
    
    screenshot = sct.grab(monitor)

    img = np.array(screenshot)

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    background_pixel = gray[5, 5]
    print(f"Background pixel value: {background_pixel}, gray[5, 5]: {gray[5, 5]}")
    if background_pixel < 100:
        print("Dark mode")
    else:
        print("Light mode")
        cv2.imshow("Screen", img)
    cv2.waitKey(0)
#light -255
#dark - 33

Background pixel value: 26, gray[5, 5]: 26
Dark mode, Obstacle pixels: 2265


