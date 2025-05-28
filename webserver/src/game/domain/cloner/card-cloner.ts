import { Card } from "../model/card";
import { Injectable } from "@nestjs/common";
@Injectable()
export class CardCloner {
    public clone(card: Card, index: string): Card {
        return new Card(card.getId(), index, card.getTextureIndex());
    }
}
