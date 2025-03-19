from program.command import CommandType, CommandArgument


def get_code_specification():
    return {
        CommandType.Mix: [CommandArgument.Power, CommandArgument.Duration, ],
        CommandType.Gap: [CommandArgument.Duration, ],
        CommandType.Alarm: [],
    }


def get_command_argument_types(command_type: CommandType) -> list[CommandArgument]:
    return get_code_specification()[command_type]
