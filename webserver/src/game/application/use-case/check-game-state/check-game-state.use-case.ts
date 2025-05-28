import { QueryBusInterface } from "@/shared/application/query-bus/query-bus.interface";
import { Injectable } from "@nestjs/common";
import { FetchGameStateQuery } from "@/game/application/query/fetch-game-state/query";
import { GameFinishedDto } from "@/game/application/use-case/check-game-state/game-finished.dto";
import { CardsMatchDto } from "@/game/application/use-case/check-game-state/cards-match.dto";
import { CommandBusInterface } from "@/shared/application/command-bus/command-bus.interface";
import { HideCardsCommand } from "@/game/application/command/hide-cards/command";
import { HideCardsDto } from "./hide-cards.dto";
import { DisableCardsCommand } from "../../command/disable-cards/command";
import { EnableCardsCommand } from "../../command/enable-cards/command";
import { StopTimerCommand } from "../../command/stop-timer/command";

@Injectable()
export class CheckGameStateUseCase {
    constructor(
        private readonly queryBus: QueryBusInterface,
        private readonly commandBus: CommandBusInterface
    ) { }

    public async execute(gameId: string): Promise<GameFinishedDto | CardsMatchDto | HideCardsDto> {
        const gameState = await this.queryBus.execute(new FetchGameStateQuery(gameId));

        if (gameState.isGameFinished()) {
            await this.commandBus.execute(new StopTimerCommand(gameId));
            return new GameFinishedDto(gameState.getFinalTimer());
        }

        if (gameState.isCardsMatch()) {
            const firstCardIndex = gameState.getFirstFlippedCard()?.getIndex() ?? "";
            const secondCardIndex = gameState.getSecondFlippedCard()?.getIndex() ?? "";
            await this.commandBus.execute(new HideCardsCommand(gameId));

            return new CardsMatchDto(firstCardIndex, secondCardIndex);
        }

        if (gameState.hasTwoFlippedCards()) {
            await this.commandBus.execute(new DisableCardsCommand(gameId));
            await new Promise(resolve => setTimeout(resolve, 200));
            await this.commandBus.execute(new HideCardsCommand(gameId));
            await this.commandBus.execute(new EnableCardsCommand(gameId));

            return new HideCardsDto(gameState.getNumberOfAttempts());
        }
    }
}
