import { QueryHandler } from '@nestjs/cqrs';
import { FetchGameCardsQuery } from './query';
import { Injectable } from '@nestjs/common';
import { Card } from '@/game/domain/model/card';
import { GameStateService } from '@/game/domain/service/game-state.service';

@Injectable()
@QueryHandler(FetchGameCardsQuery)
export class FetchGameCardsHandler {
    constructor(private readonly gameStateService: GameStateService) { }

    async execute({ gameId }: FetchGameCardsQuery): Promise<Card[]> {
        return this.gameStateService.getGameState(gameId).getCards();
    }
} 
