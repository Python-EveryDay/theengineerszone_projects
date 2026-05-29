import serial
import pyvjoy
import time

# CHANGE THIS
PORT = 'COM3'

arduino = serial.Serial(PORT, 115200)
time.sleep(2)

j = pyvjoy.VJoyDevice(1)

while True:
    try:
        data = arduino.readline().decode().strip()

        if data:
            val = int(data)

            # Map 0-1023 to joystick range
            axis = int((val / 1023) * 32767)

            j.set_axis(pyvjoy.HID_USAGE_X, axis)

            print(val)

    except:
        pass