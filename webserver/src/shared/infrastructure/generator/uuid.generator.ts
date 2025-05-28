import { Injectable } from "@nestjs/common";
import { UuidGeneratorInterface } from "@/shared/domain/generator/uuid/uuid.generator.interface";
import { v4 as uuidv4 } from 'uuid';

@Injectable()
export class UuidGenerator implements UuidGeneratorInterface {
    generate(): string {
        return uuidv4();
    }
}
