#include "MyGame.h"
#include "AnimManager.cpp"
#include "SFXManager.cpp"

SDL_Texture* IMG_LoadTexture(SDL_Renderer* renderer, const char* img) {
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(img);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    return texture;
}


void MyGame::Init(SDL_Renderer* renderer) {
    //background
    background = IMG_LoadTexture(renderer, "assets/back.png");
    middleground = IMG_LoadTexture(renderer, "assets/middle.png");
    foreground = IMG_LoadTexture(renderer, "assets/fore.png");
    //players
    player1Texture = IMG_LoadTexture(renderer, "assets/Player1Sprites.png");
    player2Texture = IMG_LoadTexture(renderer, "assets/Player2Sprites.png");
    //health
    player1HealthTex = IMG_LoadTexture(renderer, "assets/Health.png");
    player2HealthTex = IMG_LoadTexture(renderer, "assets/Health.png");
}

bool MyGame::loadAudio() {
    bool success = true;

    //Load music
    gMusic = Mix_LoadMUS("sounds/gameMusic.wav");
    if (gMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load punch
    gPunch = Mix_LoadWAV("sounds/punch.wav");
    if (gPunch == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load kick
    gKick = Mix_LoadWAV("sounds/kick.wav");
    if (gKick == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load hurt
    gHurt = Mix_LoadWAV("sounds/punched.wav");
    if (gHurt == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load grab
    gGrab = Mix_LoadWAV("sounds/grab.wav");
    if (gGrab == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load throw
    gThrowing = Mix_LoadWAV("sounds/throw.wav");
    if (gThrowing == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load thrown
    gThrown = Mix_LoadWAV("sounds/thrown.wav");
    if (gThrown == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load death
    gDeath = Mix_LoadWAV("sounds/death.wav");
    if (gDeath == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}


void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player1X = stoi(args.at(1));
            game_data.player2Y = stoi(args.at(2));
            game_data.player2X = stoi(args.at(3));
        }
    } else if (cmd == "Anim1") {
        if (args.size() == 1) {
            player_frames.player1Anim = stoi(args.at(0));
            playSound1 = 1;
        }
    } else if (cmd == "Anim2") {
        if (args.size() == 1) {
            player_frames.player2Anim = stoi(args.at(0));
            playSound2 = 1;
        }
    } else if(cmd == "SCORES") {
        if (args.size() == 2) {
            game_data.player1HP = stoi(args.at(0));
            game_data.player2HP = stoi(args.at(1));
        }
    } else if (cmd == "Win") {
        if (args.size() == 2) {
            game_data.player1Wins = stoi(args.at(0));
            game_data.player2Wins = stoi(args.at(1));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_a:
            send(event.type == SDL_KEYDOWN ? "A_DOWN" : "A_UP");
            break;
        case SDLK_d:
            send(event.type == SDL_KEYDOWN ? "D_DOWN" : "D_UP");
            break;
        case SDLK_e:
            send(event.type == SDL_KEYDOWN ? "E_DOWN" : "E_UP");
            break;
        case SDLK_q:
            send(event.type == SDL_KEYDOWN ? "Q_DOWN" : "Q_UP");
            break;
        case SDLK_r:
            send(event.type == SDL_KEYDOWN ? "R_DOWN" : "R_UP");
            break;
    }
}

void MyGame::update() {
        player1.y = game_data.player1Y;
        player1.x = game_data.player1X;
        player2.y = game_data.player2Y;
        player2.x = game_data.player2X - player_frames.player2Xtra / 2;
        p1HP = { 20, 50 , 3 * game_data.player1HP , 30 };
        p2HP = { 480 + (100 - game_data.player2HP) * 3, 50 , 3 * game_data.player2HP , 30 };
        if (Mix_PlayingMusic() == 0)
        {
            Mix_VolumeMusic(40);
            //Play the music
            Mix_PlayMusic(gMusic, 1);
        }
}


void MyGame::fixed_update() {
    if (fps < SDL_GetTicks()) {
        player_frames.player1frame++;
        if (player_frames.player1FrameAmmount <= player_frames.player1frame) {
            player_frames.player1frame = 0;
        }
        player_frames.player2frame++;
        if (player_frames.player2FrameAmmount <= player_frames.player2frame) {
            player_frames.player2frame = 0;
        }
        player1src = { player_frames.width * (player_frames.player1Collum + player_frames.player1frame) + player_frames.player1displace , player_frames.height * player_frames.player1Row ,  player_frames.width + player_frames.player1Xtra, player_frames.height};
        player1 = { player_frames.width , player_frames.height , player_frames.width + player_frames.player1Xtra , player_frames.height };
        player2src = { player_frames.width * (player_frames.player2Collum + player_frames.player2frame) + player_frames.player2displace, player_frames.height * player_frames.player2Row ,  player_frames.width + player_frames.player2Xtra, player_frames.height };
        player2 = { player_frames.width , player_frames.height , player_frames.width + player_frames.player2Xtra , player_frames.height };
        if (backgroundFps < SDL_GetTicks()) {
            if (background_frames.collumn == 3) {
                if (background_frames.row == 6) {
                    background_frames.collumn = 0;
                    background_frames.row = 0;
                }
                else {
                    background_frames.row++;
                    background_frames.collumn = 0;
                }
            }
            foregroundsrc = { background_frames.width * background_frames.collumn , background_frames.height * background_frames.row , background_frames.width , background_frames.height };
            background_frames.collumn++;
            backgroundFps = SDL_GetTicks() + 400;
        }
        fps = SDL_GetTicks() + 100;
    }
}


SDL_Texture* createTextureFromString(SDL_Renderer* renderer, const std::string& text, TTF_Font* _font, SDL_Color color) {
    SDL_Texture* textTexture = nullptr;
    SDL_Surface* textSurface = TTF_RenderText_Blended(_font, text.c_str(), color);
    if (textSurface != nullptr) {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
    }
    else {
        std::cout << "failed to create texture from string " << text << "\n";
        std::cout << TTF_GetError() << "\n";
    }
    return textTexture;
}


TTF_Font* font;
void drawText(SDL_Renderer* renderer, const std::string& text, const int& x, const int& y) {
    if (font == nullptr) {
        font = TTF_OpenFont("fonts/Bugis.ttf", 100);
    }
    SDL_Texture* textTexture = createTextureFromString(renderer, text, font, { 0xFF, 0xFF, 0xFF });
    int w, h;
    SDL_QueryTexture(textTexture, 0, 0, &w, &h);
    SDL_Rect dst = { x, y, w, h };
    SDL_RenderCopyEx(renderer, textTexture, 0, &dst, 0.0, 0, SDL_FLIP_NONE);

    SDL_DestroyTexture(textTexture);
}


void MyGame::render(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderCopy(renderer, background, NULL, &backgroundDimensions);
    SDL_RenderCopy(renderer, middleground, NULL, &backgroundDimensions);
    SDL_RenderCopy(renderer, foreground, &foregroundsrc, &backgroundDimensions);
    SDL_RenderCopy(renderer, player1Texture, &player1src, &player1);
    SDL_RenderCopyEx(renderer, player2Texture, &player2src, &player2, 0, NULL, SDL_FLIP_HORIZONTAL);
    SDL_RenderCopy(renderer, player1HealthTex, &p1HP, &p1HP);
    SDL_RenderCopy(renderer, player2HealthTex, &p2HP, &p2HP);

    drawText(renderer, std::to_string(game_data.player1Wins), 330, 7);
    drawText(renderer, std::to_string(game_data.player2Wins), 450, 7);
}