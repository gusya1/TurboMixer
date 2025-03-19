from unittest import TestCase

from parser.code_parser import parse_code
from program.command import Command
from parser.code_specification import CommandType
from parser.exceptions import ParseError


def get_alarm_command():
    return Command(CommandType.Alarm, [])

class ParseCodeTest(TestCase):
    def test_parse_empty(self):
        commands = parse_code("")
        self.assertEqual([], commands)

    def test_parse_one_correct_command(self):
        commands = parse_code("alarm")
        self.assertEqual([get_alarm_command()], commands)

    def test_parse_one_incorrect_command(self):
        with self.assertRaises(ParseError):
            parse_code("incorrect_command")

    def test_parse_many_correct_command(self):
        code_string = """alarm
alarm
alarm
alarm
alarm"""
        commands = parse_code(code_string)
        self.assertEqual([get_alarm_command()] * 5, commands)

    def test_parse_code_width_space(self):
        code_string = """alarm
alarm

alarm
alarm"""
        with self.assertRaises(ParseError):
            parse_code(code_string)

    def test_parse_code_width_invalid_argument(self):
        code_string = """alarm
alarm
mix wrong_argument 1
alarm"""
        with self.assertRaises(ParseError):
            parse_code(code_string)

