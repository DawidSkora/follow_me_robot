import serial

DEFAULT_PORT = "/dev/ttyAMA0"
BAUDRATE = 115200


class CarLink:
    """UART link to the ESP32. Sends one ASCII byte per command (see docs/protocol.md).

    The ESP32 stops when it gets no command for ~500 ms, so the current
    command has to be re-sent continuously.
    """

    def __init__(self, port=DEFAULT_PORT, baudrate=BAUDRATE):
        self._ser = serial.Serial(port=port, baudrate=baudrate, timeout=0)

    def send(self, command: str):
        self._ser.write(command.encode("ascii"))

    def close(self):
        try:
            self.send("s")
        finally:
            self._ser.close()
