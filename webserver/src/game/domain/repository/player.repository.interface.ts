import { Player } from "../model/player";
import { PlayerCollectionInterface } from "./player.collection.interface";

export abstract class PlayerRepositoryInterface {
    abstract create(player: Player): Promise<void>;
    abstract findAll(page?: number, limit?: number, sortBy?: string, sortDir?: 'asc' | 'desc'): Promise<PlayerCollectionInterface>;
}
