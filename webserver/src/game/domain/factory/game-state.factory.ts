import { GameState } from "@/game/domain/model/game-state";
import { Card } from "@/game/domain/model/card";
import { Injectable } from "@nestjs/common";

@Injectable()
export class GameStateFactory {
    public create(cards: Card[]): GameState {
        return new GameState(cards);
    }
}
