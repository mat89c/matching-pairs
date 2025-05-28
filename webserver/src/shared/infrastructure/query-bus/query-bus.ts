import { QueryBusInterface } from '@/shared/application/query-bus/query-bus.interface';
import { Injectable } from '@nestjs/common';
import { QueryBus as NestJsQueryBus } from '@nestjs/cqrs';

@Injectable()
export class QueryBus implements QueryBusInterface {
  public constructor(private readonly queryBus: NestJsQueryBus) {}

  public async execute<T, R = any>(query: T): Promise<R> {
    return this.queryBus.execute(query);
  }
}
