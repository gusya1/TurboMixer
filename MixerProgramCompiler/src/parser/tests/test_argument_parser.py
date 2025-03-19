from unittest import TestCase

from parser.exceptions import LexemeError
from parser.argument_parser import parse_argument
from parser.code_specification import CommandArgument


def parse_power(lexeme):
    return parse_argument(CommandArgument.Power, lexeme)


def parse_duration(lexeme):
    return parse_argument(CommandArgument.Duration, lexeme)


class ParsePowerArgumentTest(TestCase):
    def test_parse_not_a_number_power(self):
        with self.assertRaises(LexemeError):
            parse_power("some_text")

    def test_parse_zero_power(self):
        with self.assertRaises(LexemeError):
            parse_power("0")

    def test_parse_more_power(self):
        with self.assertRaises(LexemeError):
            parse_power("7")

    def test_parse_min_power(self):
        power = parse_power("1")
        self.assertEqual(1, power)

    def test_parse_max_power(self):
        power = parse_power("6")
        self.assertEqual(6, power)


class ParseDurationArgumentTest(TestCase):
    def test_parse_not_a_number_power(self):
        with self.assertRaises(LexemeError):
            parse_duration("some_text")

    def test_parse_zero_duration(self):
        with self.assertRaises(LexemeError):
            parse_duration("0")

    def test_parse_negative_duration(self):
        with self.assertRaises(LexemeError):
            parse_duration("-1")

    def test_parse_more_duration(self):
        with self.assertRaises(LexemeError):
            parse_duration("3601")

    def test_parse_min_duration(self):
        power = parse_duration("1")
        self.assertEqual(1, power)

    def test_parse_max_duration(self):
        power = parse_duration("3600")
        self.assertEqual(3600, power)