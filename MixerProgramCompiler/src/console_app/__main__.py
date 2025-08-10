import argparse
import logging

from program_loader import run


def read_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument('port', type=str, help='Имя COM порта')
    parser.add_argument('baudrate', type=int)
    parser.add_argument('program_file_path', type=str)
    return parser.parse_args()

def main():
    logging.basicConfig(level=logging.INFO)
    args = read_arguments()
    run(args.port, args.baudrate, args.program_file_path, logging.getLogger())

if __name__ == "__main__":
    main()