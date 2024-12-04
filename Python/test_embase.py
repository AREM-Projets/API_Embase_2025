import serial
import time
from embase import embaseClass

PORT = "COM7"
BAUD_RATE = 115200
TIMEOUT_S = 0.5

if __name__ == "__main__" :
    
    # Create serial object and pass it to command handlers
    ser = serial.Serial(PORT, BAUD_RATE, timeout=TIMEOUT_S)
    embase = embaseClass(ser, TIMEOUT_S)

    # Example usage
    embase.config.set("team", 2)
    time.sleep(1)
    team = embase.config.get("team")
