import os
import time
import shutil
from SCons.Script import AlwaysBuild, Builder, DefaultEnvironment

env = DefaultEnvironment()

# Path to the firmware binary inside the .pio/build folder
firmware_bin = os.path.join(env.subst("$BUILD_DIR"), "firmware.bin")
padding_script = os.path.join(env.subst("$PROJECT_DIR"), "src", "bootloader", "src", "pad-bootloader.py")

def pad_bootloader(source, target, env):
    """Runs the padding script after the build completes."""
    print(f"Checking for {firmware_bin}...")
    
    # Wait for firmware.bin to be generated (max 5 seconds)
    timeout = 5
    while not os.path.exists(firmware_bin) and timeout > 0:
        time.sleep(1)
        timeout -= 1

    if not os.path.exists(firmware_bin):
        print(f"Error: {firmware_bin} not found after waiting!")
        return

    print(f"Padding bootloader binary using {padding_script}...")
    
    # Run the Python padding script
    result = os.system(f"python {padding_script}")

    if result != 0:
        print("Error: Padding script failed!")
        env.Exit(1)

    print("Bootloader padding completed successfully!")

# Register post-build action to execute after compiling
env.AddPostAction("$BUILD_DIR/firmware.bin", pad_bootloader)
