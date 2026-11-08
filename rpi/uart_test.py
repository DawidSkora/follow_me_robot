import serial, time

MESSAGE = "HELLO WORLD UART!"
DELAY_S = 2

def main():
    ser = serial.Serial(port='/dev/ttyAMA0', baudrate=115200, timeout=1)  # open serial port
    print(ser.name)         # check which port was really used
    cnt = 0
    try:
        while True:
            message = f"{MESSAGE}{cnt}\n"          # dodaj \n na końcu (ułatwia parsowanie linii)
            print(f"Sending message: {message.strip()}")
            ser.write(message.encode('utf-8'))     # zamień string na bytes i wyślij
            res = ser.readline().decode("utf-8")
            print(f"received message: {res}")
            time.sleep(DELAY_S)
            cnt = cnt + 1
    finally:
        ser.close()             # close port

if __name__ == "__main__":
    main()