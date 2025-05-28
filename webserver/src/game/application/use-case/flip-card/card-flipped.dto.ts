export class CardFlippedDto {
    public event: string = "card-flipped";
    public data: {
        cardIndex: string,
        cardTextureIndex: number,
    }

    constructor(
        cardIndex: string,
        cardTextureIndex: number,
    ) {
        this.data = {
            cardIndex: cardIndex,
            cardTextureIndex: cardTextureIndex,
        }
    }
}
