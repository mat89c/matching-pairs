# Matching Pairs Game

Play the game online: [https://matching-pairs.czajka-mateusz.pl/](https://matching-pairs.czajka-mateusz.pl/)

A cross-platform memory card matching game built with C, Raylib, and Nest.js, MongoDB, WebSockets.

## Technical Overview

This project is a multiplayer memory card matching game that supports multiple platforms:
- Linux (Desktop)
- Web (WebAssembly)
- Android

The game uses:
- Raylib for graphics and input handling
- RayGUI for UI components
- libwebsockets for multiplayer functionality
- cJSON for JSON parsing
- Docker for development environment

## WebSocket Server

The game uses a WebSocket server built with:
- NestJS (Node.js framework)
- TypeScript
- WebSocket (ws) library
- MongoDB (via Mongoose) for list of the best players

### Server Features
- Real-time game state synchronization
- Game state persistence

### Server Requirements
- Node.js >= 20.0.0
- npm >= 10.0.0
- MongoDB

### Server Configuration
The WebSocket server runs on port 3000 by default. For Android clients, make sure to:
1. Update the WebSocket server address in `app/src/include/utils/websocket-manager/global-websocket-manager.h`
2. Use your computer's local IP address instead of `localhost`
3. Ensure both the server and Android device are on the same network

## Prerequisites

- Docker and Docker Compose
- Make
- Git

## Project Structure

```
matching-pairs/
├── app/                    # Main application code
│   ├── src/               # Source files
│   ├── assets/            # Game assets (images, sounds)
│   ├── libs/              # External libraries (created dynamically by Docker)
│   ├── html/              # Web build output (created dynamically by Docker)
│   ├── obj/               # Build artifacts (created dynamically by Docker)
│   ├── android.matching_pairs/  # Android build files (created dynamically by Docker)
│   ├── Makefile           # Main build configuration
│   └── Makefile.Android   # Android-specific build configuration
├── webserver/             # WebSocket server implementation
│   ├── src/              # Server source code
│   │   ├── main.ts       # Application entry point
│   │   ├── app.module.ts # Main application module
│   │   ├── app.controller.ts # HTTP controller
│   │   ├── app-cluster.service.ts # Cluster service for scaling
│   │   ├── game/         # Game-related modules and services
│   │   └── shared/       # Shared utilities and types
│   ├── dist/             # Compiled JavaScript files (created dynamically)
│   ├── node_modules/     # Node.js dependencies (created dynamically)
│   ├── package.json      # Node.js dependencies and scripts
│   ├── package-lock.json # Locked dependencies versions
│   ├── tsconfig.json     # TypeScript configuration
│   ├── tsconfig.build.json # TypeScript build configuration
│   ├── nest-cli.json     # NestJS CLI configuration
│   ├── eslint.config.mjs # ESLint configuration
│   └── .prettierrc       # Prettier code formatter configuration
├── docker/               # Docker configuration
│   └── docker-compose.yaml
├── .vscode/             # VS Code configuration
├── Makefile             # Root Makefile for Docker commands
└── README.md            # This file
```

## Development Setup

1. Clone the repository:
```bash
git clone https://github.com/your-username/matching-pairs.git
cd matching-pairs
```

2. Start the development environment:
```bash
make up
```

This will:
- Build and start the Docker containers
- Set up the development environment
- Copy necessary library headers to the app directory

## Building the Project

### For Linux
```bash
make build-for-linux
```

### For Web (WebAssembly)
```bash
make build-for-web
```

### For Android
```bash
make build-for-android
```

> **Important Note for Android Builds**: When building for Android and running on the same network as the WebSocket server, you need to modify the WebSocket server address in `app/src/include/utils/websocket-manager/global-websocket-manager.h`. Replace `localhost` with your computer's local IP address (e.g., "192.168.1.100"). You can find your IP address using:
> - On Linux: `ip addr show`
> - On Windows: `ipconfig`
> - On Mac: `ifconfig`

## Development Commands

- `make up` - Start the development environment
- `make down` - Stop and remove containers
- `make start` - Start existing containers
- `make stop` - Stop running containers
- `make shell-ubuntu` - Open shell in Ubuntu container
- `make shell-node` - Open shell in Node.js container
- `make shell-ubuntu-android` - Open shell in Android build container
- `make logs-node` - View Node.js container logs

## Environment Configuration

The project uses environment variables defined in `app/.env`. To set up your environment:

1. Copy the example configuration file:
```bash
cp app/.env.dist app/.env
```
## Dependencies

The project automatically handles dependencies through Docker, including:
- Raylib
- RayGUI
- libwebsockets
- cJSON
- Emscripten (for WebAssembly builds)
- Android NDK (for Android builds)

## License

This project is licensed under the MIT License - see the LICENSE file for details.
