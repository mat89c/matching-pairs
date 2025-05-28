export class Card {
    public constructor(
        private readonly id: string,
        private readonly index: string,
        private readonly textureIndex: number
    ) { }

    public getId(): string {
        return this.id;
    }

    public getIndex(): string {
        return this.index;
    }

    public getTextureIndex(): number {
        return this.textureIndex;
    }
}
