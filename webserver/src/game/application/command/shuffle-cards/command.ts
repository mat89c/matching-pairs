import { Card } from "@/game/domain/model/card";

export class ShuffleCardsCommand {
    public constructor(
        public readonly cards: Card[],
        public readonly gameId: string
    ) { }
} 
