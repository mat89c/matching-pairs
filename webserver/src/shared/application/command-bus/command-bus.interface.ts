export abstract class CommandBusInterface {
    abstract execute<T, R = any>(command: T): Promise<R>;
} 
