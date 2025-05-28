import { Injectable } from "@nestjs/common";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { DisableCardsCommand } from "./command";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";

@Injectable()
@CommandHandler(DisableCardsCommand)
export class DisableCardsCommandHandler {
    constructor(private readonly queryBus: QueryBusInterface) { }

    async execute({ gameId }: DisableCardsCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        gameState.disable();
    }
}
