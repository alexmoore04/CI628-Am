#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "SDL.h"

static struct GameData {
    int player1Y = 0;
    int player1X = 0;
    int player2Y = 0;
    int player2X = 0;
    int player1HP = 100;
    int player2HP = 100;
    int winner = 0;
    int player1Wins = 0;
    int player2Wins = 0;
} game_data;

static struct AnimData {
    int collum;
    int row;
    int frameAmmount;
    int xtra;
    int displace;
} frame1, frame2, frame3, frame4, frame5, frame6, frame7, frame8, frame9, frame10, frame11;


static struct PlayerFrames {
    int height = 128;
    int width = 64;
    int player1Xtra = 0;
    int player2Xtra = 0;
    int player1Anim = 0;
    int player2Anim = 0;
    int player1frame = 0;
    int player2frame = 0;
    int player1FrameAmmount = 0;
    int player2FrameAmmount = 0;
    int player1Row = 0;
    int player1Collum = 0;
    int player2Row = 0;
    int player2Collum = 0;
    int player1displace = 0;
    int player2displace = 0;
} player_frames;

static struct BackgroundFrames {
    int height = 272;
    int width = 688;
    int collumn = 0;
    int row = 0;
} background_frames; 


class MyGame {

    private:
        int fps = 0;
        int backgroundFps = 0;
        int playSound1 = 0;
        int playSound2 = 0;
        
        SDL_Rect backgroundDimensions = { 0, 0, 800, 600 };

        SDL_Rect player1 = { 200, 0, 64, 128 };
        SDL_Rect player2 = { 580, 0, 64, 128 };

        SDL_Rect p1HP = { 20, 50 , 300 , 30 };
        SDL_Rect p2HP = { 780, 50 , 300 , 30 };

        SDL_Rect player1src = { 64, 128, 64, 128 };
        SDL_Rect player2src = { 0, 0, 64, 128 };
        SDL_Rect foregroundsrc = { 0, 0, 688, 272 };

        //background textures
        SDL_Texture* background;
        SDL_Texture* middleground;
        SDL_Texture* foreground;

        //player textures
        SDL_Texture* player1Texture;
        SDL_Texture* player2Texture;

        //health textures
        SDL_Texture* player1HealthTex;
        SDL_Texture* player2HealthTex;

        //fonts
        TTF_Font* font = TTF_OpenFont("fonts/Bugis.ttf", 20);

        //music
        Mix_Music* gMusic = NULL;

        //sound effects
        Mix_Chunk* gPunch = NULL;
        Mix_Chunk* gKick = NULL;
        Mix_Chunk* gHurt = NULL;
        Mix_Chunk* gGrab = NULL;
        Mix_Chunk* gThrowing = NULL;
        Mix_Chunk* gThrown = NULL;
        Mix_Chunk* gDeath = NULL;

    public:


        std::vector<std::string> messages;

        void MyGame::Init(SDL_Renderer* renderer);
        bool MyGame::loadAudio();
        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void fixed_update();
        void frames();
        void sfx();
        void render(SDL_Renderer* renderer);

        
};

#endif