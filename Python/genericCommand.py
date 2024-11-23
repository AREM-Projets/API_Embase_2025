class genericCommand : 
    def __init__(self, name : str, argc : int, argtypes : list, default_args : list) -> None:
        self.name = name
        self.argc = argc
        self.argtypes = argtypes 
        self.default_args = default_args
        self.args = default_args

    def isValid(self, args : list) -> bool:
        """
        Returns true if the given argument list can be used with the command
        """
        
        # Checking arg count
        if len(args) > self.argc :
            return False

        # Checking arg types
        for i in range(len(args)) :
            if not isinstance(args[i], self.argtypes[i]) :
                return False
            
        return True
    
    def __str__(self) -> str:
        return self.name

# Test
if __name__ == "__main__" :
    
    test = genericCommand("test", 3, [str, str, int], ["", "", 0])
    args = ['hello', 'there']

    print(f"Command name : {str(test)}")
    print(f"Args {str(args)} valid : {str(test.isValid(args))}")
    

