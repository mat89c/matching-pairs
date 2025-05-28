import { Injectable } from "@nestjs/common";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { StartTimerCommand } from "./command";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";

@Injectable()
@CommandHandler(StartTimerCommand)
export class StartTimerCommandHandler {
    constructor(
        private readonly queryBus: QueryBusInterface
    ) { }

    public async execute({ gameId }: StartTimerCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        gameState.setTimer(process.hrtime.bigint());
    }
} 
