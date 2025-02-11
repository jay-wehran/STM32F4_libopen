import os
import time

BOOTLOADER_SIZE = 0x8000  # 32 KB
BOOTLOADER_FILE = os.path.join(os.getcwd(), ".pio", "build", "bootloader", "firmware.bin")

# Wait for firmware.bin to be generated (up to 5 seconds)
timeout = 5
while not os.path.exists(BOOTLOADER_FILE) and timeout > 0:
    time.sleep(1)
    timeout -= 1

# If the file still doesn't exist, exit with an error
if not os.path.exists(BOOTLOADER_FILE):
    print(f"Error: {BOOTLOADER_FILE} not found after waiting!")
    exit(1)

# Read and pad the firmware
with open(BOOTLOADER_FILE, "rb") as f:
    raw_file = f.read()

bytes_to_pad = BOOTLOADER_SIZE - len(raw_file)
padding = bytes([0xFF] * bytes_to_pad)  # Fill with 0xFF

with open(BOOTLOADER_FILE, "wb") as f:
    f.write(raw_file + padding)

print(f"Padded {BOOTLOADER_FILE} to {BOOTLOADER_SIZE} bytes.")
