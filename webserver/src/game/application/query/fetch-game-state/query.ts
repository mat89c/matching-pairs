import { GameState } from "@/game/domain/model/game-state";

export class FetchGameStateQuery {
    readonly __returnType: GameState;

    constructor(
        public readonly gameId: string,
    ) { }
}
