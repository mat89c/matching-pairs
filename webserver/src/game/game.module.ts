import { UuidGenerator } from "@/shared/infrastructure/generator/uuid.generator";
import { UuidGeneratorInterface } from "@/shared/domain/generator/uuid/uuid.generator.interface";
import { GameGateway } from "./ui/websocket/game.gateway";
import { Module } from "@nestjs/common";
import { FetchCardTextureIndicesQueryHandler } from "./application/query/fetch-card-texture-indices/query-handler";
import { FetchGameCardsHandler } from "./application/query/fetch-game-cards/query-handler";
import { CqrsModule } from "@nestjs/cqrs";
import { QueryBus } from "@/shared/infrastructure/query-bus/query-bus";
import { CommandBus } from "@/shared/infrastructure/command-bus/command-bus";
import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { CommandBusInterface } from "@/shared/application/command-bus/command-bus.interface";
import { GameLoopService } from "./game-loop.service";
import { CardCloner } from "./domain/cloner/card-cloner";
import { StartGameUseCase } from "./application/use-case/start-game/start-game.use-case";
import { CardFactory } from "./domain/factory/card.factory";
import { CardShuffler } from "./domain/shuffler/card-shuffler";
import { CloneCardsHandler } from "./application/command/clone-cards/command-handler";
import { ShuffleCardsHandler } from "./application/command/shuffle-cards/command-handler";
import { CreateGameStateHandler } from "./application/command/create-game-state/command-handler";
import { GameStateService } from "./domain/service/game-state.service";
import { GameStateFactory } from "./domain/factory/game-state.factory";
import { FetchGameCardByIndexQueryHandler } from "./application/query/fetch-game-card-by-index/query-handler";
import { FlipCardCommandHandler } from "./application/command/flip-card/command-handler";
import { FetchGameStateQueryHandler } from "./application/query/fetch-game-state/query-handler";
import { HideCardsCommandHandler } from "./application/command/hide-cards/command-handler";
import { CheckGameStateUseCase } from "./application/use-case/check-game-state/check-game-state.use-case";
import { IncreaseAttemptCommandHandler } from "./application/command/increase-attempt/command-handler";
import { FlipCardUseCase } from "./application/use-case/flip-card/flip-card.use-case";
import { DisableCardsCommandHandler } from "./application/command/disable-cards/command-handler";
import { EnableCardsCommandHandler } from "./application/command/enable-cards/command-handler";
import { SubmitNicknameCommandHandler } from "./application/command/submit-nickname/command-handler";
import { SubmitNicknameUseCase } from "./application/use-case/submit-nickname/submit-nickname.use-case";
import { StartTimerCommandHandler } from "./application/command/start-timer/command-handler";
import { StopTimerCommandHandler } from "./application/command/stop-timer/command-handler";
import { PlayerRepository } from "./infrastructure/repository/player/player.repository";
import { PlayerRepositoryInterface } from "./domain/repository/player.repository.interface";
import { PlayerDefinition, PlayerSchema } from "./infrastructure/persistence/mongoose/schema/player.schema";
import { MongooseModule } from "@nestjs/mongoose";
import { FetchBestPlayersQueryHandler } from "./application/query/fetch-best-players/query-handler";
import { FetchBestPlayersUseCase } from "./application/use-case/fetch-best-players/fetch-best-players.use-case";

const QueryHandlers = [
    FetchCardTextureIndicesQueryHandler,
    FetchGameCardsHandler,
    FetchGameCardByIndexQueryHandler,
    FetchGameStateQueryHandler,
    FetchBestPlayersQueryHandler,
];

const CommandHandlers = [
    CreateGameStateHandler,
    CloneCardsHandler,
    ShuffleCardsHandler,
    FlipCardCommandHandler,
    HideCardsCommandHandler,
    IncreaseAttemptCommandHandler,
    DisableCardsCommandHandler,
    EnableCardsCommandHandler,
    SubmitNicknameCommandHandler,
    StartTimerCommandHandler,
    StopTimerCommandHandler,
];

@Module({
    imports: [CqrsModule, MongooseModule.forFeature([{ name: PlayerDefinition.name, schema: PlayerSchema }])],
    providers: [
        GameGateway,
        GameLoopService,
        CardCloner,
        CardFactory,
        CardShuffler,
        GameStateService,
        GameStateFactory,
        StartGameUseCase,
        FlipCardUseCase,
        FetchGameCardByIndexQueryHandler,
        CheckGameStateUseCase,
        SubmitNicknameUseCase,
        FetchBestPlayersUseCase,
        ...QueryHandlers,
        ...CommandHandlers,
        { provide: UuidGeneratorInterface, useClass: UuidGenerator },
        { provide: QueryBusInterface, useClass: QueryBus },
        { provide: CommandBusInterface, useClass: CommandBus },
        { provide: PlayerRepositoryInterface, useClass: PlayerRepository },
    ],
})
export class GameModule { }
