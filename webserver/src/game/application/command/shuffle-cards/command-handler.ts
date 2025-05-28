import { ShuffleCardsCommand } from './command';
import { CardShuffler } from '@/game/domain/shuffler/card-shuffler';
import { Injectable } from '@nestjs/common';
import { GameStateService } from '@/game/domain/service/game-state.service';
import { CommandHandler } from '@/shared/infrastructure/command-handler/command-handler';

@Injectable()
@CommandHandler(ShuffleCardsCommand)
export class ShuffleCardsHandler {
    constructor(
        private readonly cardShuffler: CardShuffler,
        private readonly gameStateService: GameStateService
    ) { }

    async execute({ cards, gameId }: ShuffleCardsCommand): Promise<void> {
        const shuffledCards = this.cardShuffler.shuffle(cards);
        this.gameStateService.getGameState(gameId).setCards(shuffledCards);
    }
} 
