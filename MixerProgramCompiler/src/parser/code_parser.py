from .code_specification import CommandType, CommandArgument, get_command_argument_types
from .argument_parser import parse_argument
from .exceptions import LineError, ParseError, LexemeError
from program.command import Command

def get_command_type(lexeme):
    try:
        return {
            "mix": CommandType.Mix,
            "gap": CommandType.Gap,
            "alarm": CommandType.Alarm,
        }[lexeme]
    except KeyError:
        raise LineError("Неизвестная команда: {}".format(lexeme))


def make_command_template(command_type: CommandType, argument_types: list[CommandArgument]) -> str:
    arguments_string = " ".join("<{}>".format(argument_type.to_str()) for argument_type in argument_types)
    return "{} {}".format(command_type.to_str(), arguments_string)


def parse_arguments(command_type, lexemes):
    argument_types = get_command_argument_types(command_type)
    if len(argument_types) != (len(lexemes) - 1):
        raise LineError("Недопустимое число аргументов. Ожидаются аргументы: {}".format(
            make_command_template(command_type, argument_types)))
    for arg_num, argument_type, lexeme in zip(range(len(argument_types)), argument_types, lexemes[1:]):
        try:
            yield parse_argument(argument_type, lexeme)
        except LexemeError as e:
            raise LineError("Ошибка аргумента {}: {}".format(arg_num + 1, str(e)))


def parse_line(line: str):
    if not line:
        raise LineError("Пустая строка")
    lexemes = line.split(" ")
    if not lexemes[0]:
        raise LineError("Лишние пробелы в начале строки")
    command_type = get_command_type(lexemes[0])
    return Command(command_type, list(parse_arguments(command_type, lexemes)))


def parse_code(code: str) -> list[Command]:
    lines = code.splitlines()
    commands = []
    for line_num, line in enumerate(lines):
        try:
            commands.append(parse_line(line))
        except LineError as e:
            raise ParseError("Ошибка строки {}: {}".format(line_num + 1, str(e)))
    return commands