from .code_specification import CommandArgument
from .exceptions import LexemeError

def parse_integer_argument(lexeme: str):
    try:
        duration = int(lexeme)
        return duration
    except ValueError:
        raise LexemeError("Аргумент должен быть целочисленным")

def parse_power_argument(lexeme: str):
    power = parse_integer_argument(lexeme)
    if power not in range(1, 7):
        raise LexemeError("Мощность должна быть указана от 1 до 6")
    return power

def parse_duration_argument(lexeme: str):
    duration = parse_integer_argument(lexeme)
    if duration <= 0:
        raise LexemeError("Продолжительность должна быть больше нуля")
    if duration > 3600:
        raise LexemeError("Продолжительность не может быть больше 3600 секунд")
    return duration

def get_argument_parser(argument: CommandArgument):
    return {
        CommandArgument.Integer: parse_integer_argument,
        CommandArgument.Duration: parse_duration_argument,
        CommandArgument.Power: parse_power_argument
    }[argument]


def parse_argument(argument: CommandArgument, lexeme: str):
    return get_argument_parser(argument)(lexeme)