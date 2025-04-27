include ./app/.env

SRC = $(shell cd ./app && find src/ -type f -name '*.c')
DOCKER-COMPOSE := docker compose --env-file ./app/.env --file docker/docker-compose.yaml

APP_DIR := ./app
WEB_DIR := ./web
DIST_DIR := ./dist

up:
	@${DOCKER-COMPOSE} up --detach --build --remove-orphans

down:
	@${DOCKER-COMPOSE} down --volumes --remove-orphans

start:
	@${DOCKER-COMPOSE} start

stop:
	@${DOCKER-COMPOSE} stop

build:
	@${DOCKER-COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER-COMPOSE} exec ubuntu mkdir -p /app/libs
	@${DOCKER-COMPOSE} exec ubuntu cp /raylib/src/raylib.h /app/libs/raylib.h
	@${DOCKER-COMPOSE} exec ubuntu cp /raygui/src/raygui.h /app/libs/raygui.h
	@${DOCKER-COMPOSE} exec ubuntu make all PROJECT_NAME=${PROJECT_NAME} PLATFORM=${PLATFORM}

build-web:
	@${DOCKER-COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER-COMPOSE} exec ubuntu mkdir -p /app/libs
	@${DOCKER-COMPOSE} exec ubuntu cp /raylib/src/raylib.h /app/libs/raylib.h
	@${DOCKER-COMPOSE} exec ubuntu cp /raygui/src/raygui.h /app/libs/raygui.h
	@${DOCKER-COMPOSE} exec ubuntu bash -c "source /emsdk/emsdk_env.sh && make all PROJECT_NAME=${PROJECT_NAME} PLATFORM=WEB"

shell:
	@${DOCKER-COMPOSE} exec ubuntu bash

clean:
	@${DOCKER-COMPOSE} exec ubuntu rm -rf obj
