import serial

class configDestination :

    def __init__(self, serial_instance : serial.Serial) -> None:
        """
        Initialize a destination with no command types and no commands
        """
        self.destination_name = "config"
        self.serial_inst = serial_instance

    def commandString(self, command_name : str, command_args : list) -> str :
        """
        returns the string associated with a specific command, used by other methods
        """
        args_string = " ".join(map(str, command_args))
        res = f"{self.destination_name} {command_name} {args_string}"
        return res

    def set(self, variable : str, value) -> str :
        """
        Set a variable to a value
        """
        res : str = self.commandString("set", [variable, value])
        ## Send via UART
        ## Wait for answer
        ## 
        
        
        
    def get(self, variable : str) -> str :
        """
        Get a variable's value
        """
        
    
# Test

if __name__ == "__main__" :
    config = configDestination()
    print(config.set("team", [1]))
    print(config.set("team", ["blue"]))
    print(config.get("base_speed"))

    