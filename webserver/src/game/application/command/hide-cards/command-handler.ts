import { Injectable } from "@nestjs/common";
import { HideCardsCommand } from "@/game/application/command/hide-cards/command";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";

@Injectable()
@CommandHandler(HideCardsCommand)
export class HideCardsCommandHandler {
    constructor(
        private readonly queryBus: QueryBusInterface
    ) {
    }

    public async execute({ gameId }: HideCardsCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        gameState.hideCards();
    }
}
