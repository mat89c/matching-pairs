import { PlayerCollectionInterface } from "@/game/domain/repository/player.collection.interface";

export class FetchBestPlayersDto {
    public event: string = "fetch-best-players";

    public data: {
        collection: PlayerCollectionInterface;
    }

    public constructor(collection: PlayerCollectionInterface) {
        this.data = {
            collection,
        }
    }
}
