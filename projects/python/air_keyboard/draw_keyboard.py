import cv2
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
# =========================
# DRAW KEYBOARD
# =========================
def draw_keyboard(frame, finger_x, finger_y, clicking):

    pressed_key = None

    for row_index, row in enumerate(keys):

        for col_index, key in enumerate(row):

            x = 20 + col_index * 55
            y = 100 + row_index * 65

            hovered = (
                x < finger_x < x + key_width and
                y < finger_y < y + key_height
            )

            # COLORS
            if clicking and hovered:
                color = (0,255,255)  # Yellow while clicking
            elif hovered:
                color = (0,255,0)    # Green hover
            else:
                color = (0,0,180)    # Red normal
            '''
            # Filled rectangle
            cv2.rectangle(
                frame,
                (x, y),
                (x + key_width, y + key_height),
                color,
                -1
            )
        '''
            # Border
            cv2.rectangle(
                frame,
                (x, y),
                (x + key_width, y + key_height),
                (180,5,5),
                2
            )

            cv2.putText(
                frame,
                key,
                (x+12, y+35),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (0,0,0),
                2
            )

            if hovered:
                pressed_key = key

    return pressed_key