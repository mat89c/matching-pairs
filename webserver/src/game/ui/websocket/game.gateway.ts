import { WebSocketGateway, OnGatewayConnection, OnGatewayDisconnect, SubscribeMessage, } from '@nestjs/websockets';
import { WebSocket } from 'ws';
import { GameLoopService } from '@/game/game-loop.service';
import { v4 as uuidv4 } from 'uuid';
import { StartGameUseCase } from '@/game/application/use-case/start-game/start-game.use-case';
import { GameStateService } from '@/game/domain/service/game-state.service';
import { FlipCardUseCase } from '@/game/application/use-case/flip-card/flip-card.use-case';
import { CheckGameStateUseCase } from '@/game/application/use-case/check-game-state/check-game-state.use-case';
import { SubmitNicknameUseCase } from '@/game/application/use-case/submit-nickname/submit-nickname.use-case';
import { FetchBestPlayersUseCase } from '@/game/application/use-case/fetch-best-players/fetch-best-players.use-case';

@WebSocketGateway({ path: '/game', transport: ['websocket'] })
export class GameGateway implements OnGatewayConnection, OnGatewayDisconnect {
    public constructor(
        private readonly gameLoopService: GameLoopService,
        private readonly startGameUseCase: StartGameUseCase,
        private readonly gameStateService: GameStateService,
        private readonly flipCardUseCase: FlipCardUseCase,
        private readonly checkGameStateUseCase: CheckGameStateUseCase,
        private readonly submitNicknameUseCase: SubmitNicknameUseCase,
        private readonly fetchBestPlayersUseCase: FetchBestPlayersUseCase
    ) { }

    public async handleConnection(client: WebSocket) {
        console.log('Client connected');
        const gameId = uuidv4();
        (client as any).gameId = gameId;
        this.gameLoopService.initGameLoop(client, gameId);
    }

    @SubscribeMessage('game-start')
    public async gameStarted(client: WebSocket) {
        const gameId = (client as any).gameId;
        const response = await this.startGameUseCase.execute(gameId);

        this.gameLoopService.enqueueAction(
            gameId,
            JSON.stringify(response)
        );
    }

    @SubscribeMessage('card-flip')
    public async cardFlipped(client: WebSocket, message: { cardIndex: string }) {
        const gameId = (client as any).gameId;
        const response = await this.flipCardUseCase.execute(gameId, message.cardIndex);

        this.gameLoopService.enqueueAction(
            gameId,
            JSON.stringify(response)
        );

        const checkGameStateResponse = await this.checkGameStateUseCase.execute(gameId);

        this.gameLoopService.enqueueAction(
            gameId,
            JSON.stringify(checkGameStateResponse)
        );
    }

    @SubscribeMessage('nickname-submit')
    public async submitNickname(client: WebSocket, message: { nickname: string }) {
        const gameId = (client as any).gameId;
        const response = await this.submitNicknameUseCase.execute(gameId, message.nickname);

        if (null === response) {
            return;
        }

        this.gameLoopService.enqueueAction(
            gameId,
            JSON.stringify(response)
        );
    }

    @SubscribeMessage('fetch-best-players')
    public async fetchBestPlayers(client: WebSocket, message: { page: number }) {
        const gameId = (client as any).gameId;

        const response = await this.fetchBestPlayersUseCase.execute(message.page);

        this.gameLoopService.enqueueAction(
            gameId,
            JSON.stringify(response)
        );
    }

    public handleDisconnect(client: WebSocket) {
        const gameId = (client as any).gameId;
        this.gameStateService.removeGameState(gameId);
        this.gameLoopService.removeClient(gameId);
        console.log('Client disconnected');
    }
}
