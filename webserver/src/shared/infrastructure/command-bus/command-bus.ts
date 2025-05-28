import { CommandBusInterface } from '@/shared/application/command-bus/command-bus.interface';
import { Injectable } from '@nestjs/common';
import { CommandBus as NestJsCommandBus } from '@nestjs/cqrs';

@Injectable()
export class CommandBus implements CommandBusInterface {
    public constructor(private readonly commandBus: NestJsCommandBus) { }

    public async execute<T, R = any>(command: T): Promise<R> {
        return this.commandBus.execute(command);
    }
} 
