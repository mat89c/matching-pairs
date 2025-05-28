import { QueryHandler } from '@/shared/infrastructure/query-handler/query-handler';
import { FetchCardTextureIndicesQuery } from './query';
import { QueryHandlerInterface } from '@/shared/application/query-handler/query-handler.interface';
import { CardTextureIndices } from '@/game/domain/constants/card-texture-indices';

@QueryHandler(FetchCardTextureIndicesQuery)
export class FetchCardTextureIndicesQueryHandler implements QueryHandlerInterface<FetchCardTextureIndicesQuery, number[]> {
  constructor(
  ) { }

  async execute(query: FetchCardTextureIndicesQuery): Promise<number[]> {
    return Object.values(CardTextureIndices)
      .filter(value => typeof value === 'number')
      .map(index => Number(index));
  }
}
