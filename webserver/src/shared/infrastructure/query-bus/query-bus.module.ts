import { Module } from '@nestjs/common';
import { QueryBus } from './query-bus';
import { CqrsModule } from '@nestjs/cqrs';

@Module({
  imports: [CqrsModule],
  providers: [QueryBus],
  exports: [QueryBus],
})
export class QueryBusModule {}
