export class SubmitNicknameCommand {
    public constructor(
        public readonly gameId: string,
        public readonly nickname: string
    ) { }
}
