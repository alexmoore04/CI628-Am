#include "MyGame.h"

void MyGame::frames() {

    frame1 = { 2,  1,  3,  0,  0 }; //idle
    frame2 = { 6, 1, 3, 0, 0 };  //walk
    frame3 = { 7,  2,  1,  0,  0 }; //punch
    frame4 = { 6,  5,  1,  0,  0 }; //kick
    frame5 = { 11,  4,  1,  40,  0 }; //hurt
    frame6 = { 14,  4,  1,  65,  0 }; //grab
    frame7 = { 8,  5,  1,  0,  0 }; //block
    frame8 = { 13,  6,  1,  30,  0 }; //block kick
    frame9 = { 11,  6,  1,  0,  40 }; //throw
    frame10 = { 11,  5,  1,  20,  40 }; //thrown
    frame11 = { 14,  5,  1,  60,  30 }; //death

    AnimData array[11] = { frame1, frame2, frame3, frame4, frame5, frame6, frame7, frame8, frame9, frame10, frame11 };

    player_frames.player1Collum = array[player_frames.player1Anim].collum;
    player_frames.player1Row = array[player_frames.player1Anim].row;
    player_frames.player1FrameAmmount = array[player_frames.player1Anim].frameAmmount;
    player_frames.player1Xtra = array[player_frames.player1Anim].xtra;
    player_frames.player1displace = array[player_frames.player1Anim].displace;

    player_frames.player2Collum = array[player_frames.player2Anim].collum;
    player_frames.player2Row = array[player_frames.player2Anim].row;
    player_frames.player2FrameAmmount = array[player_frames.player2Anim].frameAmmount;
    player_frames.player2Xtra = array[player_frames.player2Anim].xtra;
    player_frames.player2displace = array[player_frames.player2Anim].displace;
}