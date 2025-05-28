import { Prop, Schema, SchemaFactory } from "@nestjs/mongoose";
import { HydratedDocument } from "mongoose";

export type PlayerDocument = HydratedDocument<PlayerDefinition>;

@Schema({
    collection: 'players',

})
export class PlayerDefinition {
    @Prop({ required: true })
    nickname: string;

    @Prop({ required: true })
    time: number;

    @Prop({ required: true })
    attempts: number;

    @Prop({ required: true, default: Date.now })
    date: Date;
}

export const PlayerSchema = SchemaFactory.createForClass(PlayerDefinition);
