
# Checkers-online
Downloadable binaries are located as artifacts at according builds:

[![Ubuntu Release](https://github.com/dimaglushkov/checkers-online/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/dimaglushkov/checkers-online/actions/workflows/ubuntu.yml)  [![MacOS Release](https://github.com/dimaglushkov/checkers-online/actions/workflows/macos.yml/badge.svg)](https://github.com/dimaglushkov/checkers-online/actions/workflows/macos.yml) [![Windows Release](https://github.com/dimaglushkov/checkers-online/actions/workflows/windows.yml/badge.svg)](https://github.com/dimaglushkov/checkers-online/actions/workflows/windows.yml)

Checkers-online is a simple client-server desktop checkers written on C with SDL2.0

|                                                     Loading screen                                                     |                                                   Game screen                                                    |
|:----------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------:|
| ![loading sample](https://github.com/dimaglushkov/checkers-online/blob/master/Client/img/samples/loading.png?raw=true) | ![game sample](https://github.com/dimaglushkov/checkers-online/blob/master/Client/img/samples/game.png?raw=true) |

## Installation
You can try to download and launch build for your OS from the latest release.

In order to compile and build from sources, you need to install [SDL2.0](https://www.libsdl.org/download-2.0.php) and [CMake](https://cmake.org/).
Client and Server compiles separately:

```
cmake -S Client -B Client/build -DCMAKE_BUILD_TYPE=Release
cmake --build Client/build

cmake -S Server -B Server/build -DCMAKE_BUILD_TYPE=Release
cmake --build Server/build
```

On Windows you need to put SDL2 sources to `Client/extern/SDL2-2.0.20`

## Launching

1. In order to launch server you need to specify port for it to run on by adding `-p PORT_NUMBER` command line arguments. Default value are: port - 2510. You can run Server in debug mode, to do so add `--info`.
<br>Example: `$ ./Server --info -p 2510`
3. In order to launch client app you need to specify host ip (`-h` key) and port (`-p` key). Default values are: host - 127.0.0.1, port - 2510. 
<br>Example: `$ ./Client -h 127.0.0.1 -p 2510`
