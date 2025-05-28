export abstract class QueryBusInterface {
    abstract execute<T extends { __returnType: any }>(query: T): Promise<T['__returnType']>;

}
