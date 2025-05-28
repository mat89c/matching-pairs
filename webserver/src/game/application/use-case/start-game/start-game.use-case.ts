import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { CommandBusInterface } from "@/shared/application/command-bus/command-bus.interface";
import { FetchCardTextureIndicesQuery } from "@/game/application/query/fetch-card-texture-indices/query";
import { FetchGameCardsQuery } from "@/game/application/query/fetch-game-cards/query";
import { Injectable } from "@nestjs/common";
import { CreateGameStateCommand } from "@/game/application/command/create-game-state/command";
import { CloneCardsCommand } from "@/game/application/command/clone-cards/command";
import { ShuffleCardsCommand } from "@/game/application/command/shuffle-cards/command";
import { GameStartedDto } from "@/game/application/use-case/start-game/game-started.dto";
import { StartTimerCommand } from "@/game/application/command/start-timer/command";
@Injectable()
export class StartGameUseCase {
    public constructor(
        private readonly queryBus: QueryBusInterface,
        private readonly commandBus: CommandBusInterface,
    ) { }

    public async execute(gameId: string): Promise<GameStartedDto> {
        const cardTextureIndices = await this.queryBus.execute(new FetchCardTextureIndicesQuery());

        await this.commandBus.execute(new CreateGameStateCommand(gameId, cardTextureIndices));

        const cards = await this.queryBus.execute(new FetchGameCardsQuery(gameId));

        await this.commandBus.execute(new CloneCardsCommand(cards, gameId));

        const gameCards = await this.queryBus.execute(new FetchGameCardsQuery(gameId));

        await this.commandBus.execute(new ShuffleCardsCommand(gameCards, gameId));

        const cardIndices = gameCards.map((card) => card.getIndex());

        await this.commandBus.execute(new StartTimerCommand(gameId));

        return new GameStartedDto(cardIndices);
    }
}
