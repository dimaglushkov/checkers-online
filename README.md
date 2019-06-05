# Checkers-online
Checkers-online is a simple client-server desktop checkers written on C with SDL2.0

Loading screen | Game screen
:-------------:|:-------------:
![loading sample](https://github.com/allacee/checkers-online/blob/master/Client/img/samples/loading.png) |![game sample](https://github.com/allacee/checkers-online/blob/master/Client/img/samples/game.png)

### Installation
_To compile client part it is necessary to install SDL2.0 on your machine.
Check [SDL home page](https://www.libsdl.org/download-2.0.php) for more information.__
0. Install SDL2.0 (for example `$ sudo apt-get install libsdl2-dev` or `# pacman -S sdl2`)
1. `$ git clone https://github.com/allacee/checkers-online.git`
2. `$ cd checkers-online`
4. `$ ./install`

### Launching
1. In the `launch_server` file (which is generated after installation) you can specify port for a server by adding `-p PORT_NUMBER` line. Default value: port - 2510. Also you can run Server in debug mode. To do so add `--info`.
<br>Example: `$ ./Server --info -p 2510`
2. In the `launch_client` file (which is also generated after installation) you can specify host (-h key) and port (-p key). Default values: host - 127.0.0.1, port - 2510. Running both Server and Client (twice) allows you to play with two windows opened on your local machine.
<br>Example: `$ ./Client -h 127.0.0.1 -p 2510`. 
