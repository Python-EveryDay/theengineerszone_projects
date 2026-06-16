import cv2
import serial
import time

ser = serial.Serial("COM7", 921600)
time.sleep(2)

# Try 0,1,2 if needed
cap = cv2.VideoCapture(1)

while True:

    ret, frame = cap.read()

    if not ret:
        continue

    frame = cv2.resize(frame, (128, 64))

    gray = cv2.cvtColor(
        frame,
        cv2.COLOR_BGR2GRAY
    )

    # Better visibility on OLED
    gray = cv2.equalizeHist(gray)

    _, bw = cv2.threshold(
        gray,
        110,
        255,
        cv2.THRESH_BINARY
    )

    buffer = bytearray(1024)

    for y in range(64):
        for x in range(128):

            if bw[y, x]:

                idx = x + (y // 8) * 128
                buffer[idx] |= (
                    1 << (y % 8)
                )

    ser.write(buffer)

    preview = cv2.resize(
        bw,
        (640, 320)
    )

    cv2.imshow(
        "OLED Preview",
        preview
    )

    if cv2.waitKey(1) == 27:
        break

cap.release()
ser.close()
cv2.destroyAllWindows()