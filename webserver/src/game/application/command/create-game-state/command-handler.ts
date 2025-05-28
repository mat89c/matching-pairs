import { CardFactory } from '@/game/domain/factory/card.factory';
import { Injectable } from '@nestjs/common';
import { Card } from '@/game/domain/model/card';
import { GameStateService } from '@/game/domain/service/game-state.service';
import { UuidGeneratorInterface } from '@/shared/domain/generator/uuid/uuid.generator.interface';
import { GameStateFactory } from '@/game/domain/factory/game-state.factory';
import { GameState } from '@/game/domain/model/game-state';
import { CreateGameStateCommand } from '@/game/application/command/create-game-state/command';
import { CommandHandler } from '@/shared/infrastructure/command-handler/command-handler';

@Injectable()
@CommandHandler(CreateGameStateCommand)
export class CreateGameStateHandler {
    constructor(
        private readonly gameStateFactory: GameStateFactory,
        private readonly cardFactory: CardFactory,
        private readonly gameStateService: GameStateService,
        private readonly uuidGenerator: UuidGeneratorInterface
    ) { }

    async execute({ gameId, cardTextureIndices }: CreateGameStateCommand): Promise<void> {
        const cards: Card[] = [];
        for (let i = 0; i < cardTextureIndices.length; i++) {
            const card = this.cardFactory.create(
                this.uuidGenerator.generate(),
                this.uuidGenerator.generate(),
                cardTextureIndices[i]
            );
            cards.push(card);
        }

        const gameState: GameState = this.gameStateFactory.create(cards);

        this.gameStateService.addGameState(gameId, gameState);
    }
} 
