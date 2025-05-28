import { Card } from "@/game/domain/model/card";

export class FetchGameCardByIndexQuery {
    readonly __returnType: Card;

    constructor(
        public readonly gameId: string,
        public readonly cardIndex: string,
    ) { }
}
