import { CloneCardsCommand } from './command';
import { CardCloner } from '@/game/domain/cloner/card-cloner';
import { Injectable } from '@nestjs/common';
import { Card } from '@/game/domain/model/card';
import { GameStateService } from '@/game/domain/service/game-state.service';
import { UuidGeneratorInterface } from '@/shared/domain/generator/uuid/uuid.generator.interface';
import { CommandHandler } from '@/shared/infrastructure/command-handler/command-handler';

@Injectable()
@CommandHandler(CloneCardsCommand)
export class CloneCardsHandler {
    constructor(
        private readonly cardCloner: CardCloner,
        private readonly gameStateService: GameStateService,
        private readonly uuidGenerator: UuidGeneratorInterface
    ) { }

    async execute({ cards, gameId }: CloneCardsCommand): Promise<void> {
        const clonedCards: Card[] = [];
        for (let i = cards.length, j = 0; j < cards.length; i++, j++) {
            const card = this.cardCloner.clone(cards[j], this.uuidGenerator.generate());
            clonedCards.push(card);
        }

        this.gameStateService.getGameState(gameId).addCards(clonedCards);
    }
} 
