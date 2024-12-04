import serial
import time

class genericDestination :

    def __init__(self, name : str, ser : serial.Serial, ser_timeout_s : float = 1):
        self.destination_name = name
        self.await_answer = True
        self.ser = ser
        self.ser_timeout_s = ser_timeout_s

        if self.ser == None :
            self.ser_name = "None"
        else :
            self.ser_name = self.ser.name


    def getCommandString(self, command_name : str, args : list) -> str :
        """
        returns the string associated with a specific command, used by other methods
        """
        args_string = " ".join(map(str, args))
        res = f"{self.destination_name} {command_name} {args_string}"
        return res
    
    def sendCommand(self, command_name : str, args : list) -> str :
        """
        Execute a command (aka send it via UART). Waits for an answer (ends with '\n') with default but can be deactivated by setting 
        """
        # Send command #

        command_string = self.getCommandString(command_name, args)
        command_string += '\n'

        if self.ser != None :
            self.ser.write(command_string.encode("utf-8"))
           
        print(f"[{self.ser_name}/{self.destination_name}] Sent : [{command_string[:-1]}]")

        if (self.await_answer == False) or (self.ser == None) :
            return ""
        
        # Await answer #

        response = b""
        start_time = time.time()

        while (time.time() - start_time) < (self.ser_timeout_s):
            if self.ser.in_waiting > 0:
                response += self.ser.read(self.ser.in_waiting)
                if b'\n' in response:
                    break
        
        if response:
            decoded_response = response.decode('utf-8')
            print(f"[{self.ser_name}/{self.destination_name}] Received : [{decoded_response[:-1]}]")
            if b'\n' not in response :
                print("Warning : no '\\n' in response (could be incomplete)")
        else:
            print(f"[{self.ser_name}/{self.destination_name}] No answer\n")

        return response.decode("utf-8")
        
    def setAwaitMode(self, await_answer : bool) :
        self.await_answer = await_answer

    def setSerial(self, ser : serial.Serial, timeout_s : float = 1) :
        self.ser = ser
        self.ser_timeout_s = timeout_s

        if self.ser == None :
            self.ser_name = ""
        else :
            self.ser_name = self.ser.name