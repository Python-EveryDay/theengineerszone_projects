import serial
import pyautogui
import time

arduino = serial.Serial('COM3', 9600, timeout=1)

print("Waiting for Arduino...")
time.sleep(2)

last_jump = 0
#cooldown = 0.1

while True:
    try:
        data = arduino.readline().decode().strip()

        if not data:
            continue

        #print("Raw:", data)

        if "Distance:" in data:
            distance = float(
                data.replace("Distance:", "")
                    .replace("cm", "")
                    .strip()
            )

            #print(f"Distance = {distance:.2f} cm")

            current_time = time.time()

            if distance < 20:
                pyautogui.press('space')
                #print("JUMP!")
                last_jump = current_time

    except Exception as e:
        print("Error:", e)

    except KeyboardInterrupt:
        break

arduino.close()               