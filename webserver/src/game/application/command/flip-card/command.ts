export class FlipCardCommand {
    constructor(
        public readonly gameId: string,
        public readonly cardIndex: string
    ) { }
}
