export class GameFinishedDto {
    public event: string = "game-finished";

    public data: {
        time: number
    }

    constructor(time: number) {
        this.data = {
            time: time
        }
    }
}
