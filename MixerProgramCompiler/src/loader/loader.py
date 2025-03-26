import serial
import asyncio
import enum
import logging


class SerialLoaderError(Exception):
    pass

class OpenPortError(Exception):
    pass

class UnknownStatusError(SerialLoaderError):
    pass


class LoadStatus(enum.StrEnum):
    Ready = "READY",
    Loading = "LOADING"
    Success = "SUCCESS",
    Fail = "FAIL"


def get_status(message):
    try:
        return LoadStatus(message)
    except ValueError:
        return None


class SerialLoader:
    def __init__(self):
        self.socket: None | serial.Serial = None

    def connect(self, port, baudrate):
        """Подключение к последовательному порту."""
        try:
            self.socket = serial.Serial(port, baudrate=baudrate)
            logging.debug(f"Connected to {port} at {baudrate} baud.")
        except serial.SerialException as e:
            raise OpenPortError(str(e))

    def disconnect(self):
        """Отключение от последовательного порта."""
        self.socket = None
        logging.debug(f"Disconnected from port")

    def read_status(self) -> LoadStatus:
        """Чтение данных и определение статуса."""
        assert self.socket
        try:
            message = self.socket.readline().decode().rstrip()
            status = get_status(message)
            if status is None:
                raise UnknownStatusError(message)
            return status
        except serial.SerialException as e:
            raise SerialLoaderError("Ошибка чтения: {}".format(str(e)))

    def load(self, data: bytes):
        """Запись данных в порт."""
        assert self.socket
        try:
            self.socket.write(data)
        except serial.SerialException as e:
            raise SerialLoaderError("Ошибка записи: {}".format(str(e)))