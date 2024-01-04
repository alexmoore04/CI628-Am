#include "MyGame.h"

void MyGame::sfx() {
	Mix_Chunk* sound[7] = {  gPunch, gKick, gHurt, gGrab, gThrowing, gThrown, gDeath};

	if (player_frames.player1Anim != 0 && player_frames.player1Anim != 1 && player_frames.player1Anim != 2 && player_frames.player1Anim != 3) {
		if(playSound1 == 1){
			Mix_PlayChannel(1, sound[player_frames.player1Anim - 4], 0);
			playSound1 = 0;
		}
	}
	if (player_frames.player2Anim != 0 && player_frames.player2Anim != 1 && player_frames.player2Anim != 2 && player_frames.player2Anim != 3) {
		if(playSound2 == 1){
			Mix_PlayChannel(2, sound[player_frames.player2Anim - 4], 0);
			playSound2 = 0;
		}
	}
}
