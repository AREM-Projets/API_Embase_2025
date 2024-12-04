import serial
from genericDestination import genericDestination

class configDestination(genericDestination) :

    def __init__(self, ser : serial.Serial, ser_timeout_s : float = 1):
        super().__init__("config", ser, ser_timeout_s)

    def get(self, variable : str) -> str:
        return self.sendCommand("get", [variable])

    def set(self, variable : str, value) -> str:
        return self.sendCommand("set", [variable, value])
    
if __name__=="__main__" :
    # ser = serial.Serial("COM7", 115200, timeout=1)
    config = configDestination(None, 1)

    print(config.get("team"))