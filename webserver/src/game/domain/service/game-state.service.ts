import { Injectable } from '@nestjs/common';
import { GameState } from '@/game/domain/model/game-state';

@Injectable()
export class GameStateService {
    private gameStates: Map<string, GameState> = new Map();

    public addGameState(gameId: string, gameState: GameState): void {
        this.gameStates.set(gameId, gameState);
    }

    public getGameState(gameId: string): GameState {
        return this.gameStates.get(gameId);
    }

    public removeGameState(gameId: string): void {
        this.gameStates.delete(gameId);
    }
} 
