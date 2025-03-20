from unittest import TestCase

from compiler.compiler import compile_command
from program.command import Command, CommandType

class TestCompileCommand(TestCase):
    def test_mix(self):
        command = Command(CommandType.Mix, [3, 50])
        result = compile_command(command)
        self.assertEqual(b'\x00\x02\x03\x00\x00\x002\x00\x00\x00', result)

