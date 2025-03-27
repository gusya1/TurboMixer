from unittest import TestCase

from compiler.compiler import compile_program, CompilerError
from program.command import Command, CommandType


class TestCompileProgram(TestCase):
    def test_correct(self):
        program = [
            Command(CommandType.Mix, [3, 60]),
            Command(CommandType.Gap, [300]),
            Command(CommandType.Mix, [6, 120]),
            Command(CommandType.Alarm, [])
        ]
        result = compile_program(program)
        expect = b'\x02\x04\x00\x02\x03\x00\x00\x00<\x00\x00\x00\x01\x01,\x01\x00\x00\x00\x02\x06\x00\x00\x00x\x00\x00\x00\x02\x00\x03' # noqa: E501
        self.assertEqual(expect, result)

    def test_too_many_command(self):
        program = [Command(CommandType.Mix, [3, 60])] * 1024
        with self.assertRaises(CompilerError):
            compile_program(program)

