FROM node:20-alpine AS base

WORKDIR /app

COPY webserver/package*.json .
COPY webserver/tsconfig.json .

#------------------------------------------#

FROM base AS development

ENV NODE_ENV=development

COPY webserver/ .

COPY docker/node/start.sh /start.sh
RUN chmod +x /start.sh

EXPOSE 3000

CMD ["/start.sh"]

#------------------------------------------#

FROM base AS production

ENV NODE_ENV=production

RUN npm install --only=production

COPY webserver/ .

EXPOSE 3000

CMD ["node", "dist/main"]
