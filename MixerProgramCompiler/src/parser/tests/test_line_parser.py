from unittest import TestCase

from parser.code_parser import parse_line
from parser.exceptions import LineError
from program.command import Command
from parser.code_specification import CommandType

class ParseLineTest(TestCase):
    def test_parse_empty_line(self):
        with self.assertRaises(LineError):
            parse_line("")

    def test_parse_unknown_command(self):
        with self.assertRaises(LineError):
            parse_line("some_text")

    def test_parse_command_start_with_space(self):
        command = parse_line(" alarm")
        expect = Command(CommandType.Alarm, [])
        self.assertEqual(expect, command)

    def test_parse_mix_without_arguments(self):
        with self.assertRaises(LineError):
            parse_line("mix")

    def test_parse_mix_with_less_arguments(self):
        with self.assertRaises(LineError):
            parse_line("mix 1")

    def test_parse_mix_with_more_arguments(self):
        with self.assertRaises(LineError):
            parse_line("mix 1 1 1")

    def test_parse_mix_power1_1sec(self):
        command = parse_line("mix 1 1")

        expect = Command(CommandType.Mix, [1, 1])
        self.assertEqual(expect, command)

    def test_parse_gap_without_arguments(self):
        with self.assertRaises(LineError):
            parse_line("gap")

    def test_parse_gap_with_more_arguments(self):
        with self.assertRaises(LineError):
            parse_line("gap 1 1")

    def test_parse_gap_power1_1sec(self):
        command = parse_line("gap 1")

        expect = Command(CommandType.Gap, [1])
        self.assertEqual(expect, command)

    def test_parse_alarm_with_more_arguments(self):
        with self.assertRaises(LineError):
            parse_line("alarm 1")

    def test_parse_alarm(self):
        command = parse_line("alarm")
        expect = Command(CommandType.Alarm, [])
        self.assertEqual(expect, command)

    def test_parse_gap_command_with_comment(self):
        command = parse_line("gap 1 // комментарий")
        expect = Command(CommandType.Gap, [1])
        self.assertEqual(expect, command)

    def test_parse_gap_command_with_invalid_comment(self):
        with self.assertRaises(LineError):
            parse_line("gap 1 / комментарий")
