import { Injectable } from "@nestjs/common";
import { SubmitNicknameCommand } from "./command";
import { CommandHandler } from "@/shared/infrastructure/command-handler/command-handler";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { PlayerRepositoryInterface } from "@/game/domain/repository/player.repository.interface";
import { Player } from "@/game/domain/model/player";

@Injectable()
@CommandHandler(SubmitNicknameCommand)
export class SubmitNicknameCommandHandler {
    public constructor(
        private readonly queryBus: QueryBusInterface,
        private readonly playerRepository: PlayerRepositoryInterface
    ) { }

    async execute({ gameId, nickname }: SubmitNicknameCommand): Promise<void> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));
        const player = new Player(gameId, nickname, gameState.getFinalTimer(), gameState.getNumberOfAttempts());
        await this.playerRepository.create(player);
    }
}
