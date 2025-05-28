export interface QueryHandlerInterface<TQuery, TResult> {
    execute(query: TQuery): Promise<TResult>;
  }
  