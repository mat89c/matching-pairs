import { Card } from "@/game/domain/model/card";
import { Injectable } from "@nestjs/common";

@Injectable()
export class CardFactory {
    public create(id: string, index: string, textureIndex: number): Card {
        return new Card(id, index, textureIndex);
    }
}
