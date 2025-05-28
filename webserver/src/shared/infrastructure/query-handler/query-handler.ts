import { IQuery, QueryHandler as NestJsQueryHandler } from '@nestjs/cqrs';

export function QueryHandler(query: IQuery): ClassDecorator {
  return NestJsQueryHandler(query);
}
