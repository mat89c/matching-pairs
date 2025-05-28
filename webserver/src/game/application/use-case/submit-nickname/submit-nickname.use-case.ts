import { Injectable } from "@nestjs/common";
import { CommandBusInterface } from "@/shared/application/command-bus/command-bus.interface";
import { SubmitNicknameCommand } from "@/game/application/command/submit-nickname/command";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { NicknameSubmittedDto } from "./nickname-submitted.dto";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";

@Injectable()
export class SubmitNicknameUseCase {
    public constructor(
        private readonly queryBus: QueryBusInterface,
        private readonly commandBus: CommandBusInterface,
    ) { }

    public async execute(gameId: string, nickname: string): Promise<NicknameSubmittedDto | null> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));

        if (!gameState.isGameFinished()) {
            return null;
        }

        await this.commandBus.execute(new SubmitNicknameCommand(gameId, nickname));

        return new NicknameSubmittedDto();
    }
}
