# Checkers-online
Checkers-online is a simple client-server desktop checkers written on C with SDL2.0

![loading_sample](https://github.com/allacee/checkers-online/blob/master/Client/img/samples/loading.png)
![game sample](https://github.com/allacee/checkers-online/blob/master/Client/img/samples/game.png)

### Instalation
To compile client part it's necessary to install SDL2.0 lib to your pc.
Check [SDL home page](https://www.libsdl.org/download-2.0.php) for more information.
On Ubuntu you can do it simply by running

`$ apt-cache search libsdl2`

`$ sudo apt-get install libsdl2-dev `

On your terminal.

### Execution
To start playing do next things:
1. Open [Server-release directory](https://github.com/allacee/checkers-online/tree/master/Server/cmake-build-release) and run 'Server' (`$ ./Server` on terminal). You can specify port for the server part by adding `-p PORT_NUMBER` line. Default value: port - 2510;
2. Open [Client-release directory](https://github.com/allacee/checkers-online/tree/master/Client/cmake-build-release) and run 'Client' with specified host (-h key) and port (-p key). <br> Example: `$ ./Client -h HOST -p PORT_NUMBER`. Default values: host - 127.0.0.1, port - 2510. Running both Server and Client (twice) allows you to play with two windows opened on your local machine.
