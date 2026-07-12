import serial
import pydirectinput
import time

ser = serial.Serial("COM7", 9600)
time.sleep(2)

while True:
    if ser.in_waiting:
        cmd = ser.readline().decode().strip()

        print(cmd)

        if cmd == "LEFT_DOWN":
            pydirectinput.keyDown("left")

        elif cmd == "LEFT_UP":
            pydirectinput.keyUp("left")

        elif cmd == "RIGHT_DOWN":
            pydirectinput.keyDown("right")

        elif cmd == "RIGHT_UP":
            pydirectinput.keyUp("right")

        elif cmd == "UP_DOWN":
            pydirectinput.keyDown("up")

        elif cmd == "UP_UP":
            pydirectinput.keyUp("up")

        elif cmd == "DOWN_DOWN":
            pydirectinput.keyDown("down")

        elif cmd == "DOWN_UP":
            pydirectinput.keyUp("down")