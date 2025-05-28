import { PlayerDocumentInterface } from "./player.document.interface";

export interface PlayerCollectionInterface {
    elements: PlayerDocumentInterface[];
    total: number;
    page: number;
}
