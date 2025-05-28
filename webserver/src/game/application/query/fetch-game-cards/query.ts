import { Card } from "@/game/domain/model/card";

export class FetchGameCardsQuery {
    readonly __returnType: Card[];

    public constructor(
        public readonly gameId: string
    ) { }
} 
