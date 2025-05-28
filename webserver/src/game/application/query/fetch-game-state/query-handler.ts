import { GameState } from "@/game/domain/model/game-state";
import { GameStateService } from "@/game/domain/service/game-state.service";
import { Injectable } from "@nestjs/common";
import { FetchGameStateQuery } from "./query";
import { QueryHandlerInterface } from "@/shared/application/query-handler/query-handler.interface";
import { QueryHandler } from "@/shared/infrastructure/query-handler/query-handler";

@Injectable()
@QueryHandler(FetchGameStateQuery)
export class FetchGameStateQueryHandler implements QueryHandlerInterface<FetchGameStateQuery, GameState> {
    constructor(
        private readonly gameStateService: GameStateService,
    ) { }

    public async execute({ gameId }: FetchGameStateQuery): Promise<GameState> {
        return this.gameStateService.getGameState(gameId);
    }
}
