import { Injectable } from "@nestjs/common";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { IncreaseAttemptCommand } from "@/game/application/command/increase-attempt/command";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";

@Injectable()
@CommandHandler(IncreaseAttemptCommand)
export class IncreaseAttemptCommandHandler {
    constructor(
        private readonly queryBus: QueryBusInterface
    ) { }

    public async execute({ gameId }: IncreaseAttemptCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        gameState.increaseAttempt();
    }
}
