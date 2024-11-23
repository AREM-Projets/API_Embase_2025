import serial
from genericCommand import genericCommand

class robotDestination :

    def __init__(self, port : str = "", baud_rate : int = 115200) -> None:
        """
        Initialize a destination with no command types and no commands
        """
        self.destination_name = "robot"
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

    def set(self, coord : str, args : list) -> str :
        """
        Set the robot's position (one coordinate at a time)
        """
        set_command = genericCommand(coord, 1, [int], [0])

        if (coord not in ['x', 'y', 'theta']) :
            print(f"Invalid command [set {coord}]")
            return ""

        if not set_command.isValid(args) :
            print(f"[set {coord}] - Invalid args {str(args)}")
            return ""
        
        res = self.commandString("set", coord, args)
        if self.port != "" :
            self.Serial.write(f"{res}\n")
        return res
        
        
    def get(self, coord : str) -> str :
        """
        Get the robot's position (one coordinate at a time)
        """

        if (coord not in ['x', 'y', 'theta']) :
            print(f"Invalid command [get {coord}]")
            return ""
        
        res = self.commandString("get", coord, [])
        if self.port != "" :
            self.Serial.write(f"{res}\n")
        return res
    
# Test 

if __name__ == "__main__" :
    robot = robotDestination()
    print(robot.set("x", [100]))
    print(robot.get("x"))


        

        


        
    