FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    xorg-dev \
    mesa-common-dev \
    libglu1-mesa-dev \
    libglfw3-dev \
    libglew-dev \
    libx11-dev \
    libxrandr-dev \
    libxi-dev \
    freeglut3-dev \
    libglew2.2 \
    libglu1-mesa \
    libgl1-mesa-dev \
    libxcursor-dev \
    libxinerama-dev \
    libglfw3 \
    libasound2-dev \
    libglx-mesa0 \
    clang \
    python3 \
    nodejs \
    npm

# Install emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    ./emsdk install latest && \
    ./emsdk activate latest

RUN git clone https://github.com/raysan5/raylib.git raylib
RUN cd raylib && \
    mkdir build && cd build && \
    cmake -DBUILD_EXAMPLES=FALSE -DBUILD_SHARED_LIBS=OFF .. && \
    make && \
    make install && \
    ldconfig

RUN git clone https://github.com/raysan5/raygui.git raygui

WORKDIR /app
