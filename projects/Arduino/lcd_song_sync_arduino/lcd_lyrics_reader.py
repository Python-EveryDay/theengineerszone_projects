import serial
import re
import time

# Change COM port
PORT = "COM3"

ser = serial.Serial(PORT, 9600)

# Wait for Arduino reset
time.sleep(2)
def format_for_lcd(text):

    words = text.split()

    line1 = ""
    line2 = ""

    for word in words:

        if len(line1) == 0:
            candidate = word
        else:
            candidate = line1 + " " + word

        if len(candidate) <= 16:
            line1 = candidate
        else:

            if len(line2) == 0:
                line2 = word
            else:
                line2 += " " + word

    return line1, line2

lyrics = []

with open("projects/Arduino/lcd_song_sync_arduino/lyrics.txt", "r", encoding="utf-8") as f:

    for line in f:

        line = line.strip()

        match = re.match(
            r"\[(\d+):(\d+\.\d+)\](.*)",
            line
        )

        if match:

            minutes = int(match.group(1))
            seconds = float(match.group(2))
            text = match.group(3).strip()

            timestamp = minutes * 60 + seconds

            lyrics.append(
                (timestamp, text)
            )

print("Lyrics loaded:")
#print(lyrics)

start_time = time.time()

current_index = 0

while current_index < len(lyrics):

    elapsed = time.time() - start_time

    timestamp, text = lyrics[current_index]

    if elapsed >= timestamp:

        line1, line2 = format_for_lcd(text)

        message = f"{line1}|{line2}\n"
        print(f"At {elapsed:.2f}s: {message.strip()}")
        ser.write(message.encode())

        current_index += 1

    time.sleep(0.05)

print("Finished")