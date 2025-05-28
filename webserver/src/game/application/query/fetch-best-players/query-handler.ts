import { PlayerRepositoryInterface } from "@/game/domain/repository/player.repository.interface";
import { FetchBestPlayersQuery } from "./query";
import { Injectable } from "@nestjs/common";
import { PlayerCollectionInterface } from "@/game/domain/repository/player.collection.interface";
import { QueryHandler } from "@/shared/infrastructure/query-handler/query-handler";

@Injectable()
@QueryHandler(FetchBestPlayersQuery)
export class FetchBestPlayersQueryHandler {
    public constructor(
        private readonly playerRepository: PlayerRepositoryInterface
    ) { }

    public async execute({ page }: FetchBestPlayersQuery): Promise<PlayerCollectionInterface> {
        return this.playerRepository.findAll(page);
    }
}   
