import { Injectable } from "@nestjs/common";
import { Card } from "@/game/domain/model/card";

@Injectable()
export class CardShuffler {
    public shuffle(cards: Card[]): Card[] {
        return cards.sort(() => Math.random() - 0.5);
    }
}
