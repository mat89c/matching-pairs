import { Module } from '@nestjs/common';
import { GameModule } from './game/game.module';
import { ConfigModule } from '@nestjs/config';
import { MongooseModule } from '@nestjs/mongoose';
import { AppController } from './app.controller';

@Module({
  imports: [GameModule, ConfigModule.forRoot(), MongooseModule.forRoot(process.env.MONGODB_URI)],
  controllers: [AppController],
  providers: [],
})
export class AppModule { }
