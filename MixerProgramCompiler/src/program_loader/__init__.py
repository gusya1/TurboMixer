import logging

from compiler.compiler import compile_program, CompilerError
from parser.code_parser import parse_code
from serial_loader.loader import SerialLoader, OpenPortError, LoadStatus


def read_program_file(program_file_path):
    file = open(program_file_path, 'r')
    return file.read()

def run(port: str, baudrate: int, program_file_path: str, logger: logging.Logger):
    try:
        loader = SerialLoader()
        try:
            loader.connect(port, baudrate)
        except OpenPortError as e:
            logging.error("Ошибка открытия порта: {}".format(str(e)))
            return

        program_text = read_program_file(program_file_path)
        program = parse_code(program_text)
        program_bin = compile_program(program)
        # Ожидание готовности
        status = LoadStatus.Fail
        while status is not LoadStatus.Ready:
            logging.info("Ожидание готовности устройства. Войдите в режим загрузки.")
            status = loader.read_status()
            if status is not LoadStatus.Ready:
                logging.error("Неожиданный статус устройства: {}".format(str(status)))

        logging.info("Устройство готово. Инициализируется запись.")
        # Загрузка программы
        loader.load(program_bin)

        # Ожидание окончания загрузки
        while True:
            status = loader.read_status()
            if status is LoadStatus.Loading:
                logging.info("Осуществляется запись. Подождите.")
            if status is LoadStatus.Fail:
                logging.info("Запись завершена с ошибкой.")
                return
            if status is LoadStatus.Success:
                logging.info("Запись завершена успешно.")
                return
            if status is LoadStatus.Ready:
                logging.error("Неожиданный статус устройства: {}".format(str(status)))
                return

    except CompilerError as e:
        logging.error("Ошибка компиляции: {}".format(e))