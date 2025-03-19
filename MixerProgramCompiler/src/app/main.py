from loader.loader import SerialLoader, LoadStatus, OpenPortError
from parser.code_parser import parse_code
from compiler.compiler import compile_program
import argparse
import logging

def read_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('port', type=str, help='Имя COM порта')
    parser.add_argument('baudrate', type=int)
    parser.add_argument('program_file_path', type=str)
    return parser.parse_args()

def read_program_file(program_file_path):
    file = open(program_file_path, 'r')
    return file.read()


def main():
    logging.basicConfig(level=logging.INFO)

    args = read_arguments()

    loader = SerialLoader()
    try:
        loader.connect(args.port, args.baudrate)
    except OpenPortError as e:
        logging.error("Ошибка открытия порта: {}".format(str(e)))
        return

    program_text = read_program_file(args.program_file_path)
    program = parse_code(program_text)
    program_bin = compile_program(program)

    # Ожидание готовности
    status = LoadStatus.Fail
    while status is not LoadStatus.Ready:
        logging.info("Ожидание готовности устройства. Перезагрузите устройство.")
        status = loader.read_status()
        if loader.read_status() is not LoadStatus.Ready:
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


if __name__ == "__main__":
    main()