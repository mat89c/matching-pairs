import { PlayerCollectionInterface } from "@/game/domain/repository/player.collection.interface";

export class FetchBestPlayersQuery {
    readonly __returnType: PlayerCollectionInterface;

    public constructor(
        public readonly page: number,
    ) { }
}
