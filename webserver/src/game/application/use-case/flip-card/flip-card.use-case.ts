import { Injectable } from "@nestjs/common";
import { CommandBusInterface } from "@/shared/application/command-bus/command-bus.interface";
import { FlipCardCommand } from "@/game/application/command/flip-card/command";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { FetchGameCardByIndexQuery } from "@/game/application/query/fetch-game-card-by-index/query";
import { CardFlippedDto } from "./card-flipped.dto";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";

@Injectable()
export class FlipCardUseCase {
    constructor(
        private readonly commandBus: CommandBusInterface,
        private readonly queryBus: QueryBusInterface,
    ) { }

    public async execute(gameId: string, cardIndex: string): Promise<CardFlippedDto | null> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));

        if (gameState.getFirstFlippedCard() !== null && gameState.getFirstFlippedCard().getIndex() === cardIndex) {
            return null;
        }

        await this.commandBus.execute(new FlipCardCommand(gameId, cardIndex));
        const card = await this.queryBus.execute(new FetchGameCardByIndexQuery(gameId, cardIndex));

        return new CardFlippedDto(cardIndex, card.getTextureIndex());
    }
}
