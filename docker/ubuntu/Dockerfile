FROM ubuntu:25.04

RUN apt-get update && apt-get install -y \
    git \
    # build-essential \
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
    npm \
    wget \
    tar \
    unzip \
    openjdk-17-jdk

RUN git clone https://github.com/raysan5/raygui.git raygui

RUN git clone https://github.com/warmcat/libwebsockets.git && \
    cd libwebsockets && \
    git checkout v4.3-stable && \
    mkdir build && cd build && \
    cmake .. -DLWS_WITH_STATIC=ON -DLWS_WITH_SEQUENCER=ON -DLWS_WITH_SHARED=OFF -DCMAKE_BUILD_TYPE=Release && \
    make && \
    make install

RUN git clone https://github.com/DaveGamble/cJSON.git && \
    cd cJSON && \
    mkdir build && cd build && \
    cmake -DENABLE_CJSON_TEST=Off \
      -DENABLE_CJSON_UTILS=Off \
      -DENABLE_TARGET_EXPORT=Off \
      -DENABLE_CUSTOM_COMPILER_FLAGS=Off \
      -DENABLE_VALGRIND=Off \
      -DENABLE_SANITIZERS=Off \
      -DENABLE_SAFE_STACK=Off \
      -DENABLE_LOCALES=Off \
      -DBUILD_SHARED_LIBS=Off \
      -DENABLE_CJSON_VERSION_SO=Off \
      -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc) && \
    make install

RUN git clone https://github.com/emscripten-core/emsdk.git /emsdk
RUN cd /emsdk && ./emsdk install latest && ./emsdk activate latest
ENV PATH="/emsdk:/emsdk/upstream/emscripten:/emsdk/node/14.18.2_64bit/bin:${PATH}"
ENV EMSDK="/emsdk"
ENV EMSCRIPTEN="/emsdk/upstream/emscripten"

RUN git clone https://github.com/raysan5/raylib.git raylib
RUN cd raylib && \
    mkdir build && cd build && \
    cmake -DBUILD_EXAMPLES=FALSE -DBUILD_SHARED_LIBS=OFF .. && \
    make && \
    make install && \
    ldconfig

# ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
# ENV PATH=$PATH:$JAVA_HOME/bin

# ENV NDK_VERSION r25b
# RUN wget https://dl.google.com/android/repository/android-ndk-${NDK_VERSION}-linux.zip && \
#     unzip android-ndk-${NDK_VERSION}-linux.zip -d /opt && \
#     rm android-ndk-${NDK_VERSION}-linux.zip && \
#     ln -s /opt/android-ndk-${NDK_VERSION} /opt/android-ndk

# RUN wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip && \
#     unzip commandlinetools-linux-9477386_latest.zip -d /opt && \
#     rm commandlinetools-linux-9477386_latest.zip && \
#     mkdir -p /opt/android-sdk/cmdline-tools && \
#     mv /opt/cmdline-tools /opt/android-sdk/cmdline-tools/latest

# ENV ANDROID_NDK_HOME=/opt/android-ndk
# ENV ANDROID_HOME=/opt/android-sdk
# ENV PATH=$PATH:$ANDROID_NDK_HOME:$ANDROID_HOME/cmdline-tools/latest/bin

# RUN yes | sdkmanager --sdk_root=${ANDROID_HOME} "platform-tools" "platforms;android-29" "build-tools;29.0.3"

# Compile raylib for WebAssembly
RUN cd raylib && \
    rm -rf build && \
    mkdir build && cd build && \
    emcmake cmake -DBUILD_EXAMPLES=FALSE -DBUILD_SHARED_LIBS=OFF \
          -DPLATFORM=Web \
          -DUSE_EXTERNAL_GLFW=OFF \
          -DUSE_WAYLAND=OFF \
          -DUSE_X11=OFF \
          -DUSE_DRM=OFF \
          -DUSE_EGL=ON \
          -DUSE_GLES=ON \
          -DCMAKE_BUILD_TYPE=Release .. && \
    emmake make && \
    cp raylib/libraylib.a /usr/local/lib/libraylib_web.a

# Compile raylib for Android ARM64
# RUN cd raylib && \
#     rm -rf build && \
#     mkdir build && cd build && \
#     cmake -DBUILD_EXAMPLES=FALSE -DBUILD_SHARED_LIBS=OFF \
#           -DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk/build/cmake/android.toolchain.cmake \
#           -DANDROID_ABI=arm64-v8a \
#           -DANDROID_PLATFORM=android-29 \
#           -DANDROID_STL=c++_shared \
#           -DCMAKE_BUILD_TYPE=Release \
#           -DPLATFORM=Android \
#           -DUSE_EXTERNAL_GLFW=OFF \
#           -DUSE_WAYLAND=OFF \
#           -DUSE_X11=OFF \
#           -DUSE_DRM=OFF \
#           -DUSE_EGL=ON \
#           -DUSE_GLES=ON .. && \
#     make && \
#     cp raylib/libraylib.a /usr/local/lib/libraylib_android.a

# Compile cJSON for WebAssembly
RUN cd cJSON && \
    rm -rf build && \
    mkdir build && cd build && \
    emcmake cmake -DENABLE_CJSON_TEST=Off \
      -DENABLE_CJSON_UTILS=Off \
      -DENABLE_TARGET_EXPORT=Off \
      -DENABLE_CUSTOM_COMPILER_FLAGS=Off \
      -DENABLE_VALGRIND=Off \
      -DENABLE_SANITIZERS=Off \
      -DENABLE_SAFE_STACK=Off \
      -DENABLE_LOCALES=Off \
      -DBUILD_SHARED_LIBS=Off \
      -DENABLE_CJSON_VERSION_SO=Off \
      -DCMAKE_BUILD_TYPE=Release .. && \
    emmake make && \
    cp libcjson.a /usr/local/lib/libcjson_web.a

# Compile cJSON for Android ARM64
# RUN cd cJSON && \
#     rm -rf build && mkdir build && cd build && \
#     cmake .. \
#         -DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk/build/cmake/android.toolchain.cmake \
#         -DANDROID_ABI=arm64-v8a \
#         -DANDROID_PLATFORM=android-29 \
#         -DANDROID_STL=c++_shared \
#         -DCMAKE_BUILD_TYPE=Release \
#         -DPLATFORM=Android \
#         -DBUILD_SHARED_LIBS=OFF \
#         -DENABLE_CJSON_TEST=OFF \
#         -DENABLE_CJSON_UTILS=OFF \
#         -DENABLE_TARGET_EXPORT=OFF \
#         -DENABLE_CUSTOM_COMPILER_FLAGS=OFF \
#         -DENABLE_VALGRIND=OFF \
#         -DENABLE_SANITIZERS=OFF \
#         -DENABLE_SAFE_STACK=OFF \
#         -DENABLE_LOCALES=OFF \
#         -DENABLE_CJSON_VERSION_SO=OFF && \
#     make -j$(nproc) && \
#     cp libcjson.a /usr/local/lib/libcjson_android.a

# RUN git clone https://github.com/openssl/openssl.git && \
#     cd openssl && \
#     git checkout OpenSSL_1_1_1u && \
#     export TOOLCHAIN=$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64 && \
#     export PATH=$TOOLCHAIN/bin:$PATH && \
#     ./Configure android-arm64 no-shared no-unit-test no-asm no-engine no-hw \
#       --prefix=/usr/local/openssl-android-arm64 \
#       --openssldir=/usr/local/openssl-android-arm64/ssl && \
#     make -j$(nproc) && \
#     make install_sw

# Compile libwebsockets for Android ARM64
# RUN git clone https://github.com/warmcat/libwebsockets.git libwebsockets_android && \
#     cd libwebsockets_android && \
# git checkout v4.3-stable && \
#     rm -rf build && \
#     mkdir build && cd build && \
#     cmake .. \
#         -DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk/build/cmake/android.toolchain.cmake \
#         -DANDROID_ABI=arm64-v8a \
#         -DANDROID_PLATFORM=android-29 \
#         -DLWS_WITH_STATIC=ON \
#         -DLWS_WITH_SSL=OFF \
#         -DOPENSSL_ROOT_DIR=/usr/local/openssl-android-arm64 \
#         -DOPENSSL_INCLUDE_DIR=/usr/local/openssl-android-arm64/include \
#         -DOPENSSL_CRYPTO_LIBRARY=/usr/local/openssl-android-arm64/lib/libcrypto.a \
#         -DOPENSSL_SSL_LIBRARY=/usr/local/openssl-android-arm64/lib/libssl.a \
#         -DCMAKE_LIBRARY_PATH=/usr/local/openssl-android-arm64/lib \
#         -DLWS_WITH_SEQUENCER=ON \
#         -DLWS_WITH_SHARED=OFF \
#         -DOPENSSL_USE_STATIC_LIBS=TRUE \
#         -DCMAKE_BUILD_TYPE=Release \
#         -DCMAKE_C_FLAGS="-I/usr/local/openssl-android-arm64/include -isystem /usr/local/openssl-android-arm64/include" && \
#     make && \
#     make install && \
#     cp lib/libwebsockets.a /usr/local/lib/libwebsockets_android.a

# RUN wget https://services.gradle.org/distributions/gradle-8.5-bin.zip && \
#     unzip gradle-8.5-bin.zip -d /opt && \
#     rm gradle-8.5-bin.zip && \
#     ln -s /opt/gradle-8.5/bin/gradle /usr/bin/gradle

# ENV GRADLE_HOME=/opt/gradle-8.5
# ENV PATH=$PATH:$GRADLE_HOME/bin

WORKDIR /app
