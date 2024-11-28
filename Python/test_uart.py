import serial
import time

PORT = "COM7"
BAUD_RATE = 115200
TIMEOUT_S = 0.2

if __name__ == "__main__" :

    ser = serial.Serial(PORT, BAUD_RATE, timeout=TIMEOUT_S)

    while (1) :

        command = input("Commande : ")
        command += '\n'

        ser.write(command.encode('utf-8'))

        response = b""
        start_time = time.time()

        while time.time() - start_time < (TIMEOUT_S):
            if ser.in_waiting > 0:
                response += ser.read(ser.in_waiting)
                # Optionally break if you detect a complete message, e.g., a newline

        if response:
            decoded_response = response.decode('utf-8')
            print(f"Received: [{decoded_response}]\n")
        else:
            print("No answer\n")

        