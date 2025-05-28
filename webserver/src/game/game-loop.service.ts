import { Injectable } from "@nestjs/common";
import { WebSocket } from 'ws';
import { CommandBusInterface } from "@/shared/application/command-bus/command-bus.interface";

const TICK_RATE = 50;

@Injectable()
export class GameLoopService {
    private clients: Map<string, WebSocket> = new Map();
    private actionQueues: Map<string, string[]> = new Map();

    initGameLoop(client: WebSocket, gameId: string) {
        this.clients.set(gameId, client);
        this.actionQueues.set(gameId, []);
        this.startGameLoop(gameId);
    }

    enqueueAction(gameId: string, action: string) {
        const queue = this.actionQueues.get(gameId) || [];
        queue.push(action);
        this.actionQueues.set(gameId, queue);
    }

    private startGameLoop(gameId: string) {
        setInterval(() => {
            this.processActions(gameId);
        }, TICK_RATE);
    }

    private processActions(gameId: string) {
        const queue = this.actionQueues.get(gameId) || [];
        const actionsToProcess = [...queue];
        this.actionQueues.set(gameId, []);

        const client = this.clients.get(gameId);
        if (!client) return;

        for (const action of actionsToProcess) {
            this.handleAction(client, action);
        }
    }

    private handleAction(client: WebSocket, action: string) {
        client.send(action);
    }

    removeClient(gameId: string) {
        this.clients.delete(gameId);
        this.actionQueues.delete(gameId);
    }
}

