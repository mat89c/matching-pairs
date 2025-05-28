import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { Injectable } from "@nestjs/common";
import { FetchBestPlayersQuery } from "@/game/application/query/fetch-best-players/query";
import { FetchBestPlayersDto } from "./fetch-best-players.dto";

@Injectable()
export class FetchBestPlayersUseCase {
    public constructor(
        private readonly queryBus: QueryBusInterface
    ) { }

    public async execute(page: number): Promise<FetchBestPlayersDto> {
        const bestPlayerCollection = await this.queryBus.execute(new FetchBestPlayersQuery(page));

        return new FetchBestPlayersDto(bestPlayerCollection);
    }
}
