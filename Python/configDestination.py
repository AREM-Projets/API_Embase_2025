import serial
from genericCommand import genericCommand

class configDestination :

    def __init__(self, port : str = "", baud_rate : int = 115200) -> None:
        """
        Initialize a destination with no command types and no commands
        """
        self.destination_name = "config"
        self.port = port
        if port != "" :
            self.Serial = serial.Serial(port, baud_rate)
        

    def commandString(self, command_type : str, command_name : str, command_args : list) -> str :
        """
        returns the string associated with a specific command, used by other methods
        """
        args_string = " ".join(map(str, command_args))
        res = f"{self.destination_name} {command_type} {command_name} {args_string}"
        return res

    def set(self, variable : str, args : list) -> str :
        """
        Set the robot's position (one coordinate at a time)
        """
        set_command = genericCommand(variable, 1, [int | str], [0])

        if not set_command.isValid(args) :
            print(f"[set {variable}] - Invalid args {str(args)}")
            return ""
        
        res = self.commandString("set", variable, args)
        if self.port != "" :
            self.Serial.write(f"{res}\n")
        return res
        
        
    def get(self, variable : str) -> str :
        """
        Get the robot's position (one coordinate at a time)
        """
        
        res = self.commandString("get", variable, [])
        if self.port != "" :
            self.Serial.write(f"{res}\n")
        return res
    
# Test

if __name__ == "__main__" :
    config = configDestination()
    print(config.set("team", [1]))
    print(config.set("team", ["blue"]))
    print(config.get("base_speed"))

    