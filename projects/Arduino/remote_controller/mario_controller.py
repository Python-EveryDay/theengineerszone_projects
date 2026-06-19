import serial
import pydirectinput
import time
  
ser = serial.Serial("COM8", 9600)
time.sleep(2)

while True:
    if ser.in_waiting:
        cmd = ser.readline().decode().strip()

        print(cmd)

        if cmd == "RIGHT_DOWN":
            pydirectinput.keyDown("right")

        elif cmd == "RIGHT_UP":
            pydirectinput.keyUp("right")

        elif cmd == "JUMP_DOWN":
            pydirectinput.keyDown("space")

        elif cmd == "JUMP_UP":
            pydirectinput.keyUp("space")