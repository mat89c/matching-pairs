import { Injectable } from "@nestjs/common";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { StopTimerCommand } from "./command";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";

@Injectable()
@CommandHandler(StopTimerCommand)
export class StopTimerCommandHandler {
    constructor(private readonly queryBus: QueryBusInterface
    ) { }

    public async execute({ gameId }: StopTimerCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        const startTime = gameState.getTimer();
        const finalTimer = Math.round(Number(process.hrtime.bigint() - startTime) / 1e7) / 100;
        gameState.setFinalTimer(finalTimer);
    }
}
