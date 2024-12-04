import time
import serial
from configDestination import configDestination

class embaseClass :

    def __init__(self, ser : serial.Serial, timeout_s : float = 1):
        # Init destinations
        self.config = configDestination(ser, timeout_s)
        
        # Manually updated list of all destinations
        self.destinations = [self.config]

    def setAwaitMode(self, await_answer : bool) :
        for dest in self.destinations :
            dest.setAwaitMode(await_answer)

    def setSerial(self, ser : serial.Serial, timeout_s : float = 1) :
        for dest in self.destinations :
            dest.setSerial(ser, timeout_s)
        

if __name__ == "__main__" :
    # main function
    embase = embaseClass(None, 1)

    embase.config.set("team", "blue")
    time.sleep(1)
    embase.config.get("team")