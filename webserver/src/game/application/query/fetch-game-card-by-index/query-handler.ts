import { GameStateService } from "@/game/domain/service/game-state.service";
import { Injectable } from "@nestjs/common";
import { FetchGameCardByIndexQuery } from "./query";
import { Card } from "@/game/domain/model/card";
import { QueryHandlerInterface } from "@/shared/application/query-handler/query-handler.interface";
import { QueryHandler } from "@/shared/infrastructure/query-handler/query-handler";

@Injectable()
@QueryHandler(FetchGameCardByIndexQuery)
export class FetchGameCardByIndexQueryHandler implements QueryHandlerInterface<FetchGameCardByIndexQuery, Card> {
    constructor(
        private readonly gameStateService: GameStateService,
    ) { }

    public async execute({ gameId, cardIndex }: FetchGameCardByIndexQuery): Promise<Card> {
        const gameState = this.gameStateService.getGameState(gameId);
        const cards = gameState.getCards();
        const card = cards.find(card => card.getIndex() === cardIndex);

        if (!card) {
            throw new Error("Card not found");
        }

        return card;
    }
}
