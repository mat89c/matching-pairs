export class GameStartedDto {
    public event: string = "game-started";
    public data: {
        cardIndices: string[],
    }

    constructor(
        cardIndices: string[],
    ) {
        this.data = {
            cardIndices: cardIndices,
        }
    }
}
