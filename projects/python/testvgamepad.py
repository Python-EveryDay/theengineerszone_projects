import time
import vgamepad as vg

gamepad = vg.VX360Gamepad()

print("Connected")

while True:

    gamepad.left_joystick(
        x_value=32767,
        y_value=0
    )

    gamepad.update()

    time.sleep(1)

    gamepad.left_joystick(
        x_value=-32768,
        y_value=0
    )

    gamepad.update()

    time.sleep(1)