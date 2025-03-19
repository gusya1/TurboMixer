import enum


class CommandType(enum.IntEnum):
    Mix = 0,
    Gap = 1,
    Alarm = 2

    def to_str(self):
        return {
            CommandType.Mix: "mix",
            CommandType.Gap: "gap",
            CommandType.Alarm: "alarm"
        }[self]

class CommandArgument(enum.IntEnum):
    Integer = 0,
    Duration = 1,
    Power = 2

    def to_str(self):
        return {
            CommandArgument.Integer: "int",
            CommandArgument.Duration: "duration",
            CommandArgument.Power: "power"
        }[self]


class Command:
    def __init__(self, command_type: CommandType, args: list):
        self.type = command_type
        self.args = args

    def __eq__(self, other):
        """Сравнение команд. Нужно для тестирования."""
        return self.type == other.type and self.args == other.args