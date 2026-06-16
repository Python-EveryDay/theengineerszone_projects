import serial
import re
import time

# ==========================
# Configuration
# ==========================
PORT = "COM7"
BAUD_RATE = 9600

LYRICS_FILE = r"D:\Arduino&gestureProject\githubrepo\theengineerszone_projects\projects\Arduino\lcd_song_sync_arduino\love_me_not_lyrics.txt"

# ==========================
# Connect to Arduino
# ==========================
ser = serial.Serial(PORT, BAUD_RATE)
time.sleep(2)

# ==========================
# Parse Lyrics
# ==========================
lyrics = []

with open(LYRICS_FILE, "r", encoding="utf-8") as f:

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

print(f"Loaded {len(lyrics)} lyric lines")

# ==========================
# Start Synchronization
# ==========================
start_time = time.time()

for i in range(len(lyrics)):

    current_timestamp, text = lyrics[i]

    # Skip blank lyric lines
    if not text:
        continue

    # Remove punctuation
    text = re.sub(r"[^\w\s']", "", text)

    words = text.split()

    if not words:
        continue

    # Calculate duration until next lyric line
    if i < len(lyrics) - 1:
        next_timestamp = lyrics[i + 1][0]
        duration = next_timestamp - current_timestamp
    else:
        duration = len(words) * 0.5

    # Prevent division by zero
    duration = max(duration, 0.2)

    delay_per_word = duration / len(words)

    # Wait until lyric timestamp arrives
    while time.time() - start_time < current_timestamp:
        time.sleep(0.005)

    # Display each word
    for word in words:

        # Send word to Arduino
        message = f"{word}|\n"

        print(
            f"{time.time()-start_time:.2f}s -> {word}"
        )

        ser.write(message.encode())

        time.sleep(delay_per_word)

print("Lyrics Finished!")

ser.close()