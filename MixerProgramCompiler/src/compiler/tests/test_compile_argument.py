from unittest import TestCase

from compiler.compiler import compile_argument

class TestCompileArgument(TestCase):
    def test_int(self):
        result = compile_argument(64)
        self.assertEqual(b'\x40\x00\x00\x00', result)

    def test_string(self):
        with self.assertRaises(AssertionError):
            compile_argument("some string")

