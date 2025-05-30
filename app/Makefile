.PHONY: all clean run

PLATFORM              ?= $(PLATFORM)

PROJECT_NAME          ?= matching_pairs
PROJECT_VERSION       ?= $(PROJECT_VERSION)
PROJECT_BUILD_PATH    ?= .

ifeq ($(PLATFORM),PLATFORM_WEB)
    OUTPUT_DIR           ?= html
else
    OUTPUT_DIR           ?= .
endif

RAYLIB_PATH           ?= /raylib

RAYLIB_INCLUDE_PATH   ?= /usr/local/include
RAYLIB_LIB_PATH       ?= /usr/local/lib

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    RAYLIB_LIB = $(RAYLIB_LIB_PATH)/libraylib.a
else ifeq ($(PLATFORM),PLATFORM_WEB)
    RAYLIB_LIB = $(RAYLIB_LIB_PATH)/libraylib_web.a
endif

RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= DEBUG

USE_WAYLAND_DISPLAY   ?= FALSE

BUILD_WEB_ASYNCIFY    ?= FALSE
BUILD_WEB_SHELL       ?= /app/template.html
BUILD_WEB_HEAP_SIZE   ?= 134217728
BUILD_WEB_RESOURCES   ?= TRUE
BUILD_WEB_RESOURCES_PATH  ?= /app/assets

PLATFORM_OS = LINUX
PLATFORM_SHELL = sh

RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src

CC = gcc

ifeq ($(PLATFORM),PLATFORM_WEB)
    CC = emcc
    RAYLIB_PATH = /raylib
    RAYLIB_LIB_PATH = /usr/local/lib
endif

MAKE ?= make

CFLAGS = -std=c11 -Wall -Wno-missing-braces -Wno-unused-but-set-variable -Wunused-result -D_DEFAULT_SOURCE

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
    ifeq ($(PLATFORM),PLATFORM_DESKTOP)
        CFLAGS += -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope
    endif
else
    ifeq ($(PLATFORM),PLATFORM_WEB)
        ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
            CFLAGS += -O3
        else
            CFLAGS += -Os
        endif
    else
        CFLAGS += -s -O2
    endif
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),STATIC)
            CFLAGS += -D_DEFAULT_SOURCE
        endif
    endif
endif

INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I$(RAYLIB_PATH)/src/extras

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        INCLUDE_PATHS += -I$(RAYLIB_INCLUDE_PATH)
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    INCLUDE_PATHS += -I$(EMSCRIPTEN_PATH)/cache/sysroot/include -I/usr/local/include
endif

LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        LDFLAGS += -L$(RAYLIB_LIB_PATH)
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    LDFLAGS += -s USE_GLFW=3 -s TOTAL_MEMORY=$(BUILD_WEB_HEAP_SIZE) -s FORCE_FILESYSTEM=1 -s STACK_SIZE=131072 -L$(RAYLIB_LIB_PATH) /usr/local/lib/libcjson_web.a
    
    ifeq ($(BUILD_WEB_ASYNCIFY),TRUE)
        LDFLAGS += -s ASYNCIFY
    endif
    
    ifeq ($(BUILD_WEB_RESOURCES),TRUE)
        LDFLAGS += --preload-file $(BUILD_WEB_RESOURCES_PATH)
    endif
    
    ifeq ($(BUILD_MODE),DEBUG)
        LDFLAGS += -s ASSERTIONS=1 --profiling
    endif

    LDFLAGS += -s EXPORTED_FUNCTIONS=['_main','_malloc','_free']
    LDFLAGS += -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','HEAPF32']

    LDFLAGS += --shell-file $(BUILD_WEB_SHELL)
    EXT = .html
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
        # NOTE: Required packages: libegl1-mesa-dev
        LDLIBS = -lraylib /usr/local/lib/libwebsockets.a -lz -lssl -lcrypto -lGL -lm -lpthread -ldl -lrt /usr/local/lib/libcjson.a

        LDLIBS += -lX11

        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            LDLIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
        endif
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            LDLIBS += -lc
        endif
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    LDLIBS = $(RAYLIB_LIB) /usr/local/lib/libcjson_web.a -lwebsocket.js
endif

SRC_DIR = src
OBJ_DIR = obj

ifeq ($(PLATFORM),PLATFORM_WEB)
    SRC := $(shell find $(SRC_DIR) -name "*.c" \
    ! -path "$(SRC_DIR)/*/utils/websocket-manager/libwebsockets/*" \
    ! -path "$(SRC_DIR)/*/utils/android/*")
endif

ifeq ($(PLATFORM),PLATFORM_ANDROID)
        SRC := $(shell find $(SRC_DIR) -name "*.c" ! -path "${SRC_DIR}/*/utils/websocket-manager/emscripten/*")
endif

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    SRC := $(shell find $(SRC_DIR) -name "*.c" \
    ! -path "$(SRC_DIR)/*/utils/websocket-manager/emscripten/*" \
    ! -path "$(SRC_DIR)/*/utils/android/*")
endif

OBJS = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(shell mkdir -p $(dir $(OBJS)))

MAKEFILE_PARAMS = $(PROJECT_NAME)

all:
	$(MAKE) $(MAKEFILE_PARAMS)

$(PROJECT_NAME): $(OBJS)
	@mkdir -p $(OUTPUT_DIR)
	$(CC) -o $(OUTPUT_DIR)/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

run:
	$(MAKE) $(MAKEFILE_PARAMS)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
		./$(PROJECT_NAME)
    endif
endif

.PHONY: clean_shell_cmd clean_shell_sh

clean:	clean_shell_$(PLATFORM_SHELL)
	@echo Cleaning done

clean_shell_sh:
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),LINUX)
		find . -type f -executable -delete
		rm -fv *.o
    endif
endif
ifeq ($(PLATFORM),PLATFORM_WEB)
    ifeq ($(PLATFORM_OS),LINUX)
		rm -fv *.o $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm
    endif
endif

clean_shell_cmd: SHELL=cmd
clean_shell_cmd:
	del *.o *.exe $(PROJECT_NAME).data $(PROJECT_NAME).html $(PROJECT_NAME).js $(PROJECT_NAME).wasm /s

lint:
	cppcheck --enable=all --suppress=missingIncludeSystem src/**
