import { FlipCardCommand } from "@/game/application/command/flip-card/command";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { Injectable } from "@nestjs/common";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";

@Injectable()
@CommandHandler(FlipCardCommand)
export class FlipCardCommandHandler {
    constructor(private readonly queryBus: QueryBusInterface) { }

    async execute({ gameId, cardIndex }: FlipCardCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        gameState.flipCard(cardIndex);
    }
}
