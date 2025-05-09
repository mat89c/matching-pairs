include ./app/.env

DOCKER_COMPOSE := docker compose --env-file ./app/.env --file docker/docker-compose.yaml

up:
	@${DOCKER_COMPOSE} up --detach --build --remove-orphans
	@${DOCKER_COMPOSE} exec ubuntu mkdir -p /app/libs
	@${DOCKER_COMPOSE} exec ubuntu cp /raylib/src/raylib.h /app/libs/raylib.h
	@${DOCKER_COMPOSE} exec ubuntu cp /raygui/src/raygui.h /app/libs/raygui.h
	@${DOCKER_COMPOSE} exec ubuntu cp /opt/android-ndk/sources/android/native_app_glue/android_native_app_glue.h /app/libs/android_native_app_glue.h

down:
	@${DOCKER_COMPOSE} down --volumes --remove-orphans

start:
	@${DOCKER_COMPOSE} start

stop:
	@${DOCKER_COMPOSE} stop

build:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu make clean PLATFORM=PLATFORM_DESKTOP
	@${DOCKER_COMPOSE} exec ubuntu make all PROJECT_NAME=${PROJECT_NAME} PLATFORM=PLATFORM_DESKTOP

build-html:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu rm -rf html
	@${DOCKER_COMPOSE} exec ubuntu bash -c "source /emsdk/emsdk_env.sh && make clean PLATFORM=PLATFORM_WEB"
	@${DOCKER_COMPOSE} exec ubuntu bash -c "source /emsdk/emsdk_env.sh && make PROJECT_NAME=${PROJECT_NAME} PLATFORM=PLATFORM_WEB -B"

build-android:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu rm -rf android.matching_pairs
	@${DOCKER_COMPOSE} exec ubuntu rm -rf matching_pairs.apk
	@${DOCKER_COMPOSE} exec ubuntu bash -c "make PROJECT_NAME=${PROJECT_NAME} PLATFORM=PLATFORM_ANDROID -f Makefile.Android"

build-android-sh:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
	@${DOCKER_COMPOSE} exec ubuntu rm -rf matching_pairs.apk
	@${DOCKER_COMPOSE} exec ubuntu bash -c "./build.sh"

shell:
	@${DOCKER_COMPOSE} exec ubuntu bash

clean:
	@${DOCKER_COMPOSE} exec ubuntu rm -rf obj
