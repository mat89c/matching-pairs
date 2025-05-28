export class HideCardsDto {
    public event: string = "hide-cards";

    public data: {
        numberOfAttempts: number;
    }

    public constructor(numberOfAttempts: number) {
        this.data = {
            numberOfAttempts,
        }
    }
}
