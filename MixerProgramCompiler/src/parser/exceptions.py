
class ParseError(Exception):
    pass

class LineError(Exception):
    pass

class InvalidArgumentsCountError(LineError):
    pass

class LexemeError(Exception):
    pass
