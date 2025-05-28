export class CreateGameStateCommand {
    public constructor(
        public readonly gameId: string,
        public readonly cardTextureIndices: number[]
    ) { }
} 
