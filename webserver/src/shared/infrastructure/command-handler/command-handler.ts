import { CommandHandler as NestJsCommandHandler, ICommand } from "@nestjs/cqrs";

export function CommandHandler(command: ICommand): ClassDecorator {
    return NestJsCommandHandler(command);
}
