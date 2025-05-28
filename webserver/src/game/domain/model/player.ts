export class Player {
    public constructor(
        public readonly id: string,
        public readonly nickname: string,
        public readonly time: number,
        public readonly attempts: number,
    ) { }
}
