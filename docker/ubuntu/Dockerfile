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
    npm \
    wget \
    tar \
    unzip \
    openjdk-17-jdk

RUN git clone https://github.com/raysan5/raygui.git raygui

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

# Set JAVA_HOME
ENV JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64
ENV PATH=$PATH:$JAVA_HOME/bin

# Install Android NDK
ENV NDK_VERSION r25b
RUN wget https://dl.google.com/android/repository/android-ndk-${NDK_VERSION}-linux.zip && \
    unzip android-ndk-${NDK_VERSION}-linux.zip -d /opt && \
    rm android-ndk-${NDK_VERSION}-linux.zip && \
    ln -s /opt/android-ndk-${NDK_VERSION} /opt/android-ndk
    # # Create missing directories and symlinks
    # mkdir -p /opt/android-ndk/sysroot/usr/include/asm && \
    # ln -s /opt/android-ndk/sysroot/usr/include/arm-linux-androideabi/asm /opt/android-ndk/sysroot/usr/include/asm/arm && \
    # ln -s /opt/android-ndk/sysroot/usr/include/aarch64-linux-android/asm /opt/android-ndk/sysroot/usr/include/asm/aarch64 && \
    # # Add missing sys/poll.h symlink
    # mkdir -p /opt/android-ndk/sysroot/usr/include/sys && \
    # ln -s /opt/android-ndk/sysroot/usr/include/poll.h /opt/android-ndk/sysroot/usr/include/sys/poll.h && \
    # # Add missing stddef.h symlink
    # ln -s /opt/android-ndk/sysroot/usr/include/linux/stddef.h /opt/android-ndk/sysroot/usr/include/stddef.h && \
    # # Add system-wide symlinks
    # mkdir -p /usr/include/sys && \
    # ln -s /opt/android-ndk/sysroot/usr/include/poll.h /usr/include/sys/poll.h && \
    # ln -s /opt/android-ndk/sysroot/usr/include/linux/stddef.h /usr/include/stddef.h

# Install Android SDK Command-line Tools
RUN wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip && \
    unzip commandlinetools-linux-9477386_latest.zip -d /opt && \
    rm commandlinetools-linux-9477386_latest.zip && \
    mkdir -p /opt/android-sdk/cmdline-tools && \
    mv /opt/cmdline-tools /opt/android-sdk/cmdline-tools/latest

# Set environment variables
ENV ANDROID_NDK_HOME=/opt/android-ndk
ENV ANDROID_HOME=/opt/android-sdk
ENV PATH=$PATH:$ANDROID_NDK_HOME:$ANDROID_HOME/cmdline-tools/latest/bin

# Install Android SDK components
RUN yes | sdkmanager --sdk_root=${ANDROID_HOME} "platform-tools" "platforms;android-29" "build-tools;29.0.3"

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
RUN cd raylib && \
    rm -rf build && \
    mkdir build && cd build && \
    cmake -DBUILD_EXAMPLES=FALSE -DBUILD_SHARED_LIBS=OFF \
          -DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk/build/cmake/android.toolchain.cmake \
          -DANDROID_ABI=arm64-v8a \
          -DANDROID_PLATFORM=android-29 \
          -DANDROID_STL=c++_shared \
          -DCMAKE_BUILD_TYPE=Release \
          -DPLATFORM=Android \
          -DUSE_EXTERNAL_GLFW=OFF \
          -DUSE_WAYLAND=OFF \
          -DUSE_X11=OFF \
          -DUSE_DRM=OFF \
          -DUSE_EGL=ON \
          -DUSE_GLES=ON .. && \
    make && \
    cp raylib/libraylib.a /usr/local/lib/libraylib_android.a

# Install Gradle
RUN wget https://services.gradle.org/distributions/gradle-8.5-bin.zip && \
    unzip gradle-8.5-bin.zip -d /opt && \
    rm gradle-8.5-bin.zip && \
    ln -s /opt/gradle-8.5/bin/gradle /usr/bin/gradle

# Set Gradle environment variable
ENV GRADLE_HOME=/opt/gradle-8.5
ENV PATH=$PATH:$GRADLE_HOME/bin

#RUN cp /usr/include/stddef.h /opt/android-ndk/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/stddef.h

WORKDIR /app
