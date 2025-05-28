import { Injectable } from "@nestjs/common";
import { Model } from "mongoose";
import { PlayerDefinition, PlayerDocument } from "@/game/infrastructure/persistence/mongoose/schema/player.schema";
import { InjectModel } from "@nestjs/mongoose";
import { Player } from "@/game/domain/model/player";
import { PlayerRepositoryInterface } from "@/game/domain/repository/player.repository.interface";
import { PlayerDocumentInterface } from "@/game/domain/repository/player.document.interface";
import { PlayerCollectionInterface } from "@/game/domain/repository/player.collection.interface";

@Injectable()
export class PlayerRepository implements PlayerRepositoryInterface {
    public constructor(
        @InjectModel(PlayerDefinition.name) private readonly playerModel: Model<PlayerDocument>
    ) { }

    public async create(player: Player): Promise<void> {
        await this.playerModel.create(player);
    }

    public async findAll(
        page = 1,
        limit = 10,
        sortBy = 'attempts',
        sortDir: 'asc' | 'desc' = 'asc'
    ): Promise<PlayerCollectionInterface> {
        const total = await this.playerModel.countDocuments();

        const sortCriteria: Record<string, 1 | -1> = {
            [sortBy]: sortDir === 'asc' ? 1 : -1,
        };

        if (sortBy === 'attempts') {
            sortCriteria['time'] = 1;
        }

        const players = await this.playerModel.find()
            .sort(sortCriteria)
            .skip((page - 1) * limit)
            .limit(limit)
            .exec();

        return this.mapToCollection(players, total, page, limit);
    }

    private mapToCollection(players: PlayerDocument[], total: number, page: number, limit: number): PlayerCollectionInterface {
        return {
            elements: players.map((player, index) => this.mapToDomain(player, index + (page - 1) * limit)),
            total: total,
            page: page
        };
    }

    private mapToDomain(player: PlayerDocument, position: number): PlayerDocumentInterface {
        const date = player.date;
        const pad = (n: number) => n.toString().padStart(2, '0');

        const hours = pad(date.getHours());
        const minutes = pad(date.getMinutes());
        const day = pad(date.getDate());
        const month = pad(date.getMonth() + 1);
        const year = date.getFullYear();

        return {
            id: player._id.toString(),
            position: position + 1,
            nickname: player.nickname,
            time: player.time,
            attempts: player.attempts,
            date: `${hours}:${minutes} ${day}-${month}-${year}`
        };
    }
}
