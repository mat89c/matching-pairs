include ./app/.env

DOCKER_COMPOSE := docker compose -p ${PROJECT_NAME} --env-file ./app/.env --file docker/docker-compose.yaml

up:
	@${DOCKER_COMPOSE} up --detach --build --remove-orphans
	@${DOCKER_COMPOSE} exec ubuntu mkdir -p /app/libs
	@${DOCKER_COMPOSE} exec ubuntu cp /raylib/src/raylib.h /app/libs/raylib.h
	@${DOCKER_COMPOSE} exec ubuntu cp /raygui/src/raygui.h /app/libs/raygui.h
	@${DOCKER_COMPOSE} exec ubuntu cp /usr/local/include/libwebsockets.h /app/libs/libwebsockets.h
	@${DOCKER_COMPOSE} exec ubuntu cp /usr/local/include/lws_config.h /app/libs/lws_config.h
	@${DOCKER_COMPOSE} exec ubuntu mkdir -p /app/libs/libwebsockets
	@${DOCKER_COMPOSE} exec ubuntu cp -r /usr/local/include/libwebsockets /app/libs
	@${DOCKER_COMPOSE} exec ubuntu mkdir -p /app/libs/cjson
	@${DOCKER_COMPOSE} exec ubuntu cp /usr/local/include/cjson/cJSON.h /app/libs/cjson/cJSON.h
	@${DOCKER_COMPOSE} exec ubuntu cp /emsdk/upstream/emscripten/system/include/emscripten.h /app/libs/emscripten.h
	@${DOCKER_COMPOSE} exec ubuntu mkdir -p /app/libs/emscripten
	@${DOCKER_COMPOSE} exec ubuntu cp -r /emsdk/upstream/emscripten/system/include/emscripten /app/libs
	@${DOCKER_COMPOSE} exec ubuntu-android cp /opt/android-ndk/sources/android/native_app_glue/android_native_app_glue.h /app/libs/android_native_app_glue.h
down:
	@${DOCKER_COMPOSE} down --volumes --remove-orphans

start:
	@${DOCKER_COMPOSE} start

stop:
	@${DOCKER_COMPOSE} stop

build-for-linux:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu make clean PLATFORM=PLATFORM_DESKTOP
	@${DOCKER_COMPOSE} exec ubuntu make all PROJECT_NAME=${PROJECT_NAME} PLATFORM=PLATFORM_DESKTOP

build-for-web:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu rm -rf html
	@${DOCKER_COMPOSE} exec ubuntu bash -c "source /emsdk/emsdk_env.sh && make clean PLATFORM=PLATFORM_WEB"
	@${DOCKER_COMPOSE} exec ubuntu bash -c "source /emsdk/emsdk_env.sh && make PROJECT_NAME=${PROJECT_NAME} PLATFORM=PLATFORM_WEB -B"

build-for-android:
	@${DOCKER_COMPOSE} exec ubuntu-android rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu-android rm -rf android.matching_pairs
	@${DOCKER_COMPOSE} exec ubuntu-android rm -rf matching_pairs.apk
	@${DOCKER_COMPOSE} exec ubuntu-android bash -c "make PROJECT_NAME=${PROJECT_NAME} PLATFORM=PLATFORM_ANDROID -f Makefile.Android"

logs-node:
	@${DOCKER_COMPOSE} logs -f node

shell-ubuntu:
	@${DOCKER_COMPOSE} exec ubuntu bash

shell-node:
	@${DOCKER_COMPOSE} exec node sh

shell-ubuntu-android:
	@${DOCKER_COMPOSE} exec ubuntu-android bash
