import { Card } from "./card";

export class GameState {
    private cards: Card[] = [];
    private firstFlippedCard: Card | null = null;
    private secondFlippedCard: Card | null = null;
    private currentFlippedCard: Card | null = null;
    private numberOfAttempts: number = 0;
    private numberOfMatches: number = 0;
    private isFinished: boolean;
    private isEnabled: boolean;
    private nickname: string | null = null;
    private timer: bigint = BigInt(0);
    private finalTimer: number = 0;

    constructor(cards: Card[]) {
        this.cards = cards;
        this.firstFlippedCard = null;
        this.secondFlippedCard = null;
        this.currentFlippedCard = null;
        this.numberOfAttempts = 0;
        this.numberOfMatches = 0;
        this.isFinished = false;
        this.isEnabled = true;
        this.nickname = null;
        this.timer = BigInt(0);
        this.finalTimer = 0;
    }

    public flipCard(cardIndex: string): void {
        if (!this.isEnabled) {
            return;
        }

        this.currentFlippedCard = this.cards.find(card => card.getIndex() === cardIndex);

        if (this.firstFlippedCard === null) {
            this.firstFlippedCard = this.currentFlippedCard;
            return;
        }

        if (this.secondFlippedCard === null) {
            this.secondFlippedCard = this.currentFlippedCard;
        }

        if (this.firstFlippedCard.getId() !== this.secondFlippedCard.getId()) {
            this.numberOfAttempts++;
            return;
        }

        this.numberOfMatches++;

        if (this.numberOfMatches === this.cards.length / 2) {
            this.isFinished = true;
        }
    }

    public getCurrentFlippedCard(): Card | null {
        return this.currentFlippedCard;
    }

    public isGameFinished(): boolean {
        return this.isFinished;
    }

    public increaseAttempt(): void {
        this.numberOfAttempts++;
    }

    public getNumberOfAttempts(): number {
        return this.numberOfAttempts;
    }

    public getCards(): Card[] {
        return this.cards;
    }

    public setCards(cards: Card[]): void {
        this.cards = cards;
    }

    public addCards(cards: Card[]): void {
        this.cards.push(...cards);
    }

    public isCardsMatch(): boolean {
        return this.firstFlippedCard !== null && this.secondFlippedCard !== null && this.firstFlippedCard.getId() === this.secondFlippedCard.getId();
    }

    public hasTwoFlippedCards(): boolean {
        return this.firstFlippedCard !== null && this.secondFlippedCard !== null && this.firstFlippedCard.getId() !== this.secondFlippedCard.getId();
    }

    public getFirstFlippedCard(): Card | null {
        return this.firstFlippedCard;
    }

    public getSecondFlippedCard(): Card | null {
        return this.secondFlippedCard;
    }

    public hideCards(): void {
        this.firstFlippedCard = null;
        this.secondFlippedCard = null;
        this.currentFlippedCard = null;
    }

    public disable(): void {
        this.isEnabled = false;
    }

    public enable(): void {
        this.isEnabled = true;
    }

    public setNickname(nickname: string): void {
        this.nickname = nickname;
    }

    public getNickname(): string | null {
        return this.nickname;
    }

    public setTimer(timer: bigint): void {
        this.timer = timer;
    }

    public getTimer(): bigint {
        return this.timer;
    }

    public setFinalTimer(finalTimer: number): void {
        this.finalTimer = finalTimer;
    }

    public getFinalTimer(): number {
        return this.finalTimer;
    }
}
