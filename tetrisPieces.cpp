#include "raylib.h"
#include "tetrisPieces.h"
#include <array>
#include <iostream>

#define ny 20
#define nx 10
using BoardType = std::array<std::array<Color, ny>, nx>;

Piece::Piece(){
    this->Initialized = false;
}

void Piece::deletePiece(){
    this->Initialized = false;
}

void Piece::init(OutPiece& p){
    this->color = p.color;
    this->posX = p.posX;
    this->posY = p.posY;
    this->rotation = p.rotation;
    this->pieceShape = p.Shape;
    this->Initialized = p.Initialized;
    this->fastFalling = p.fastFalling;

    p.Initialized = false;
}

void Piece::rotate(BoardType& Board){
    int coordX, coordYu, coordYd;
    if(IsKeyPressed(KEY_E)){
        this->rotation--;
        for (int k = 0; k<5; k++){
            if (shapes[pieceShape][rotation%4][k][0] + posX < 0 || shapes[pieceShape][rotation%4][k][0] + posX >= nx || shapes[pieceShape][rotation%4][k][1] + posY < 0){
                this->rotation++;
                break;
            }
            coordX = shapes[pieceShape][rotation%4][k][0] + posX;
            coordYu = shapes[pieceShape][rotation%4][k][1] + posY + 0.9999999f;
            coordYd = shapes[pieceShape][rotation%4][k][1] + posY + 0.2f;
            if (Board[coordX][coordYu].a != 0 || Board[coordX][coordYd].a != 0){
                this->rotation++;
                break;
            }
        }
    }
    if(IsKeyPressed(KEY_Q)){
        this->rotation++;
        for (int k = 0; k<5; k++){
            if (shapes[pieceShape][rotation%4][k][0] + posX < 0 || shapes[pieceShape][rotation%4][k][0] + posX >= nx || shapes[pieceShape][rotation%4][k][1] + posY < 0){
                this->rotation--;
                break;
            }
            coordX = shapes[pieceShape][rotation%4][k][0] + posX;
            coordYu = shapes[pieceShape][rotation%4][k][1] + posY + 0.9999999f;
            coordYd = shapes[pieceShape][rotation%4][k][1] + posY + 0.2f;
            if (Board[coordX][coordYu].a != 0 || Board[coordX][coordYd].a != 0){
                this->rotation--;
                break;
            }
        }
    }
}

void Piece::fall(int time0){
    // if(fastFalling){
    //     posY += 0.30 + GetTime()/30000;
    // } else {
    //     posY += 0.01 + GetTime()/30000;
    // }

    if(fastFalling){
        posY += (300 + GetTime() - time0)/1000;
    } else {
        posY += (300 + GetTime() - time0)/30000;
    }
}

void Piece::move(BoardType& Board){
    int coordX, coordYu, coordYd;
    if(IsKeyPressed(KEY_A)){
        this->posX--;
        for (int k = 0; k<5; k++){
            if (shapes[pieceShape][rotation%4][k][0] + posX < 0){
                this->posX++;
                break;
            }
            coordX = shapes[pieceShape][rotation%4][k][0] + posX;
            coordYu = shapes[pieceShape][rotation%4][k][1] + posY + 0.9999999f;
            coordYd = shapes[pieceShape][rotation%4][k][1] + posY + 0.2f;
            if (Board[coordX][coordYu].a != 0 || Board[coordX][coordYd].a != 0){
                this->posX++;
                break;
            }
        }
    }
    if(IsKeyPressed(KEY_D)){
        this->posX++;
        for (int k = 0; k<5; k++){
            if (shapes[pieceShape][rotation%4][k][0] + posX >= nx){
                this->posX--;
                break;
            }
            coordX = shapes[pieceShape][rotation%4][k][0] + posX;
            coordYu = shapes[pieceShape][rotation%4][k][1] + posY + 0.9999999f;
            coordYd = shapes[pieceShape][rotation%4][k][1] + posY + 0.2f;
            if (Board[coordX][coordYu].a != 0 || Board[coordX][coordYd].a != 0){
                this->posX--;
                break;
            }
        }
    }
}

void Piece::SetFastFalling(){
    this->fastFalling = true;
}

bool Piece::shouldFreeze(BoardType& Board){
    int coordX, coordY;
    for (int k = 0; k<5; k++){
        coordX = shapes[pieceShape][rotation%4][k][0] + posX;
        coordY = shapes[pieceShape][rotation%4][k][1] + posY + 0.9999999999999999;
        if (shapes[pieceShape][rotation%4][k][1] + posY + 1 > ny){
            return true;
        }
        if (Board[coordX][coordY].a != 0){
            return true;
        }
    }
    return false;
}