#include "SDL_net.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>
#include <sstream>

#include "MyGame.h"

using namespace std;

const char* IP_NAME = "localhost";
const Uint16 PORT = 55555;

bool is_running = true;

MyGame* game = new MyGame();

static int on_receive(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    const int message_length = 1024;

    char message[message_length];
    int received;

    // TODO: while(), rather than do
    do {
        received = SDLNet_TCP_Recv(socket, message, message_length);
        message[received] = '\0';

        std::cout << message << "\n";

        stringstream test(message);
        string segment;
        vector<string> seglist;

        while (getline(test, segment, '|'))
        {
            seglist.push_back(segment);
        }

        //cout << "seglist length " << seglist.size() << "\n";
        //cout << "seglist " << seglist[0] << "\n";

        if (seglist.size() > 1) {
            // get the command, which is the first string in the message
            for (int i = 0; i < seglist.size(); i++)
            {
                int len = seglist[i].length();

                //cout << "seglist length " << seglist[i].length() << "\n";

                char* nChar = new char[len + 1];
                strcpy(nChar, seglist[i].c_str());
                char* pch = strtok(nChar, ",");

                //cout << "nChar " << nChar << "\n";

                string cmd(pch);

                // then get the arguments to the command
                vector<string> args;

                while (pch != NULL) {
                    pch = strtok(NULL, ",");


                    if (pch != NULL) {
                        args.push_back(string(pch));
                    }
                }

                delete[] nChar;

                game->on_receive(cmd, args);

                if (cmd == "exit") {
                    break;
                }

            }
        }
        else 
        {
            char* pch = strtok(message, ",");
            string cmd(pch);

            // then get the arguments to the command
            vector<string> args;

            while (pch != NULL) {
                pch = strtok(NULL, ",");


                if (pch != NULL) {
                    args.push_back(string(pch));
                }
            }

            game->on_receive(cmd, args);

            if (cmd == "exit") {
                break;
            }
        }

    } while (received > 0 && is_running);

    return 0;
}


static int on_send(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    while (is_running) {
        if (game->messages.size() > 0) {
            string message = "CLIENT_DATA";

            for (auto m : game->messages) {
                message += "," + m;
            }

            game->messages.clear();

            cout << "Sending_TCP: " << message << endl;

            SDLNet_TCP_Send(socket, message.c_str(), message.length());
        }
        
        SDL_Delay(1);
    }
    SDLNet_TCP_Send(socket, "player disconnected", 19);

    return 0;
}

void loop(SDL_Renderer* renderer) {
    SDL_Event event;

    while (is_running) {
        // input
        while (SDL_PollEvent(&event)) {
            if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
                game->input(event);

                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:               
                        is_running = false;
                        break;

                    default:
                        break;
                }
            }

            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        game->sfx();

        game->update();

        game->render(renderer);

        game->frames();

        game->fixed_update();

        SDL_RenderPresent(renderer);

        SDL_Delay(17);
    }
}

int run_game() {
    SDL_Window* window = SDL_CreateWindow(
        "Multiplayer Fighting Game Client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (nullptr == window) {
        std::cout << "Failed to create window" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (nullptr == renderer) {
        std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
        return -1;
    }

    game->Init(renderer);

    game->loadAudio();

    loop(renderer);

    return 0;
}

int main(int argc, char** argv) {

    // Initialize SDL
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    //initialise audio
    if (SDL_INIT_AUDIO == -1) {
        printf("SDL_INIT_AUDIO: %s\n", SDL_GetError());
        exit(3);
    }

    //initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 2048) == -1)
    {
        printf("SDL_mixer: %s\n", Mix_GetError());
        exit(4);
    }


    //initialise images
    if (SDL_Init(IMG_INIT_PNG) == -1) {
        printf("SDL_Init(IMG_INIT_PNG): %s\n", SDLNet_GetError());
        exit(5);
    }

    //initialise text 
    if (TTF_Init() == -1) {
        printf("TTF_Init(): %s\n", SDLNet_GetError());
        exit(6);
    }



    IPaddress ip;

    // Resolve host (ip name + port) into an IPaddress type
    if (SDLNet_ResolveHost(&ip, IP_NAME, PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(3);
    }

    // Open the connection to the server
    TCPsocket socket = SDLNet_TCP_Open(&ip);

    if (!socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(4);
    }



    SDL_CreateThread(on_receive, "ConnectionReceiveThread", (void*)socket);
    SDL_CreateThread(on_send, "ConnectionSendThread", (void*)socket);

    run_game();

    delete game;

    // Close connection to the server
    SDLNet_TCP_Close(socket);

    // Shutdown SDL_net
    SDLNet_Quit();

    // Shutdown Mixer
    Mix_Quit();

    // Shutdown SDL
    SDL_Quit();

    return 0;
}