from program.command import Command

class CompilerError(Exception):
    pass

STX = b'\x02'
ETX = b'\x03'

def to_bytes(number: int, length:int , **kwargs):
    return number.to_bytes(length, byteorder="little", **kwargs)

def compile_argument(argument) -> bytes:
    if type(argument) is int:
        return to_bytes(argument, 4, signed=True)
    assert False, "unsupported argument type"


def compile_command(command: Command) -> bytes:
    result = b''
    result += to_bytes(int(command.type), 1, signed=False)
    result += to_bytes(len(command.args), 1, signed=False)
    for arg in command.args:
        result += compile_argument(arg)
    return result


def compile_program(program: list[Command]) -> bytes:
    result = b''
    result += STX
    try:
        result += to_bytes(len(program), 1, signed=False)
    except OverflowError:
        raise CompilerError("Слишком много команд")
    for command in program:
        result +=  compile_command(command)
    result += ETX
    return result


def main():
    pass


if __name__ == "__main__":
    main()