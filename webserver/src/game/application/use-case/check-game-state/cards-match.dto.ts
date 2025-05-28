export class CardsMatchDto {
    public event: string = "cards-match";

    public data: {
        firstCardIndex: string;
        secondCardIndex: string;
    }

    public constructor(firstCardIndex: string, secondCardIndex: string) {
        this.data = {
            firstCardIndex,
            secondCardIndex,
        }
    }
}
