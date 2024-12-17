#include "raylib.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <array>
#include <utility>

#include "tetrisPieces.h"

#define BACKGROUND (Color){50,50,50,255}

#define ny 20
#define nx 10
using BoardType = std::array<std::array<Color, ny>, nx>;

const int weights[15] = {100, 50, 100, 20, 30, 100, 50, 50, 35, 35, 50, 50, 35, 35, 30};
const int shapes[15][4][5][2] = {
        {{{-1, 0}, { 0, 0}, { 1, 0}, { 2, 0}, { 0, 0}}, {{ 0,-1}, { 0, 0}, { 0, 1}, { 0, 2}, { 0, 0}}, {{-1, 0}, { 0, 0}, { 1, 0}, { 2, 0}, { 0, 0}}, {{ 0,-1}, { 0, 0}, { 0, 1}, { 0, 2}, { 0, 0}}}, //  0 shape: I 
        {{{ 0, 0}, { 1, 0}, { 0, 0}, { 0, 0}, { 0, 0}}, {{ 0, 0}, { 0, 1}, { 0, 0}, { 0, 0}, { 0, 0}}, {{ 0, 0}, { 1, 0}, { 0, 0}, { 0, 0}, { 0, 0}}, {{ 0, 0}, { 0, 1}, { 0, 0}, { 0, 0}, { 0, 0}}}, //  1 shape: i
        {{{ 0, 0}, { 1, 0}, { 0, 1}, { 1, 1}, { 0, 0}}, {{ 0, 0}, { 1, 0}, { 0, 1}, { 1, 1}, { 0, 0}}, {{ 0, 0}, { 1, 0}, { 0, 1}, { 1, 1}, { 0, 0}}, {{ 0, 0}, { 1, 0}, { 0, 1}, { 1, 1}, { 0, 0}}}, //  2 shape: sq
        {{{ 0, 0}, { 0, 1}, { 0, 2}, { 1, 1}, {-1, 1}}, {{ 0, 0}, { 0, 1}, { 0, 2}, { 1, 1}, {-1, 1}}, {{ 0, 0}, { 0, 1}, { 0, 2}, { 1, 1}, {-1, 1}}, {{ 0, 0}, { 0, 1}, { 0, 2}, { 1, 1}, {-1, 1}}}, //  3 shape: plus
        {{{ 0, 0}, {-1, 0}, { 1, 0}, {-1, 1}, { 1, 1}}, {{ 0, 0}, { 0,-1}, { 0, 1}, {-1,-1}, {-1, 1}}, {{ 0, 0}, { 1, 0}, {-1, 0}, { 1,-1}, {-1,-1}}, {{ 0, 0}, { 0, 1}, { 0,-1}, { 1, 1}, { 1,-1}}}, //  4 shape: U
        {{{-1, 0}, { 0, 0}, { 1, 0}, { 0, 1}, { 0, 0}}, {{ 0,-1}, { 0, 0}, { 0, 1}, {-1, 0}, { 0, 0}}, {{ 1, 0}, { 0, 0}, {-1, 0}, { 0,-1}, { 0, 0}}, {{ 0, 1}, { 0, 0}, { 0,-1}, { 1, 0}, { 0, 0}}}, //  5 shape: T
        {{{-1, 0}, { 0, 0}, { 1, 0}, {-1, 1}, { 0, 0}}, {{ 0,-1}, { 0, 0}, { 0, 1}, {-1,-1}, { 0, 0}}, {{ 1, 0}, { 0, 0}, {-1, 0}, { 1,-1}, { 0, 0}}, {{ 0, 1}, { 0, 0}, { 0,-1}, { 1, 1}, { 0, 0}}}, //  6 shape: L1
        {{{-1, 0}, { 0, 0}, { 1, 0}, { 1, 1}, { 0, 0}}, {{ 0,-1}, { 0, 0}, { 0, 1}, {-1, 1}, { 0, 0}}, {{ 1, 0}, { 0, 0}, {-1, 0}, {-1,-1}, { 0, 0}}, {{ 0, 1}, { 0, 0}, { 0,-1}, { 1,-1}, { 0, 0}}}, //  7 shape: L2
        {{{-1, 0}, {-1, 1}, { 0, 0}, { 1, 0}, { 2, 0}}, {{ 0,-1}, {-1,-1}, { 0, 0}, { 0, 1}, { 0, 2}}, {{ 1, 0}, { 1,-1}, { 0, 0}, {-1, 0}, {-2, 0}}, {{ 0, 1}, { 1, 1}, { 0, 0}, { 0,-1}, { 0,-2}}}, //  8 shape: LL1
        {{{-2, 0}, {-1, 0}, { 0, 0}, { 1, 0}, { 1, 1}}, {{ 0,-2}, { 0,-1}, { 0, 0}, { 0, 1}, {-1, 1}}, {{ 2, 0}, { 1, 0}, { 0, 0}, {-1, 0}, {-1,-1}}, {{ 0, 2}, { 0, 1}, { 0, 0}, { 0,-1}, { 1,-1}}}, //  9 shape: LL2
        {{{-1, 0}, { 0, 0}, { 0, 1}, { 1, 1}, { 0, 0}}, {{ 0,-1}, { 0, 0}, {-1, 0}, {-1, 1}, { 0, 0}}, {{ 1, 0}, { 0, 0}, { 0,-1}, {-1,-1}, { 0, 0}}, {{ 0, 1}, { 0, 0}, { 1, 0}, { 1,-1}, { 0, 0}}}, // 10 shape: S1
        {{{-1, 1}, { 0, 1}, { 0, 0}, { 1, 0}, { 0, 0}}, {{-1,-1}, {-1, 0}, { 0, 0}, { 0, 1}, { 0, 0}}, {{ 1,-1}, { 0,-1}, { 0, 0}, {-1, 0}, { 0, 0}}, {{ 1, 1}, { 1, 0}, { 0, 0}, { 0,-1}, { 0, 0}}}, // 11 shape: S2
        {{{-1, 0}, { 0, 0}, { 1, 0}, { 0, 1}, { 1, 1}}, {{ 0,-1}, { 0, 0}, { 0, 1}, {-1, 0}, {-1, 1}}, {{ 1, 0}, { 0, 0}, {-1, 0}, { 0,-1}, {-1,-1}}, {{ 0, 1}, { 0, 0}, { 0,-1}, { 1, 0}, { 1,-1}}}, // 12 shape: P1
        {{{-1, 0}, { 0, 0}, { 1, 0}, { 0, 1}, {-1, 1}}, {{ 0,-1}, { 0, 0}, { 0, 1}, {-1, 0}, {-1,-1}}, {{ 1, 0}, { 0, 0}, {-1, 0}, { 0,-1}, { 1,-1}}, {{ 0, 1}, { 0, 0}, { 0,-1}, { 1, 0}, { 1, 1}}}, // 13 shape: P2
        {{{ 0, 0}, {-1 ,0}, { 1, 0}, { 0, 1}, { 0, 2}}, {{-1, 1}, { 0, 1}, { 1, 0}, { 1, 1}, { 1, 2}}, {{ 0, 0}, { 0, 1}, { 0, 2}, {-1, 2}, { 1, 2}}, {{-1, 0}, {-1, 1}, {-1, 2}, { 0, 1}, { 1, 1}}}  // 14 shape: TT
    };
const int shapeDims[15][2] = {{ 4, 1}, { 2, 1}, { 2, 2}, { 3, 3}, { 3, 2}, { 3, 2}, { 3, 2}, { 3, 2}, { 4, 2}, { 4, 2}, { 3, 2}, { 3, 2}, { 3, 2}, { 3, 2}, { 3, 3}};

int strToInt(std::string str){
    int num = 0;

    for(int i = 0; i<(int)str.length(); i++){
        num += (str[i] - '0')*pow(10, str.length() - i - 1);
    }

    return num;
}

char* strToChar(std::string& str){
    char* cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    return cstr;
}

std::string intToStr(int num){
    std::string str = std::to_string(num);
    return str;
}

std::string cronometro(int time_s){
    std::string cronometro = "00:00:00";

    int hours   = time_s/3600;
    int minutes = (time_s/60)%60;
    int seconds = time_s%60;

    cronometro[0] = hours / 10 + 48;
    cronometro[1] = hours % 10 + 48;
    cronometro[3] = minutes / 10 + 48;
    cronometro[4] = minutes % 10 + 48;
    cronometro[6] = seconds / 10 + 48;
    cronometro[7] = seconds % 10 + 48;

    return cronometro;
}

bool mouseInRect(int mx, int my, int posx, int posy, int width, int height){
    return (mx > posx && mx < posx+width) && (my > posy && my < posy+height);
}

void DrawTetrisSq(float posX, float posY, int side, Color color){
    DrawTriangle((Vector2){posX       , posY       }, 
                 (Vector2){posX       , posY + side},
                 (Vector2){posX + side, posY       }, 
                 (Color){(unsigned char)(color.r + 30), 
                         (unsigned char)(color.g + 30), 
                         (unsigned char)(color.b + 30), 255});

    DrawTriangle((Vector2){posX + side, posY + side}, 
                 (Vector2){posX + side, posY       }, 
                 (Vector2){posX       , posY + side}, 
                 (Color){(unsigned char)(color.r - 30), 
                         (unsigned char)(color.g - 30), 
                         (unsigned char)(color.b - 30), 255});
    
    DrawRectangle(posX + side*0.15f, posY + side*0.15f, side*0.7f, side*0.7f, color);
}

void createNewPiece(OutPiece& piece){
    int fullWeight = 0;
    for (int i = 0; i<15; i++){
        fullWeight += weights[i];
    }

    SetRandomSeed(GetTime()*34563673);
    int rIndex = GetRandomValue(0, fullWeight);
    int sum = 0;

    for (int i = 0; i<15; i++){
        sum += weights[i];
        if (rIndex <= sum){
            piece.Shape = i;
            break;
        }
    }

    std::cout << "lol";

    piece.posX = nx/2;
    piece.posY = 0;
    piece.rotation = 0;
    piece.Initialized = true;
    piece.fastFalling = false;
    piece.color = (Color){(unsigned char)GetRandomValue(80, 225), 
                          (unsigned char)GetRandomValue(80, 225), 
                          (unsigned char)GetRandomValue(80, 225), 255};
}

void swapPieces(OutPiece& p, Piece& P){
    std::swap(p.color, P.color);
    std::swap(p.posX, P.posX);
    std::swap(p.posY, P.posY);
    std::swap(p.rotation, P.rotation);
    std::swap(p.Shape, P.pieceShape);
    std::swap(p.Initialized, P.Initialized);
    std::swap(p.fastFalling, P.fastFalling);
}

class Game{
    private:
        int sx, sy, sq, gap;
        unsigned int resolution;
        int difResolutions[8][2] = {{900, 500}, {1280, 720}, {1366, 768}, {1440, 900}, {1536, 864}, {1920, 1080}, {2560, 1440}, {3840, 2160}};

        int mx, my;
        bool ml, mr, mc; // mouse left, right, center
        BoardType Board;

        int stage; // 0 menu, 1 juego, 2 settings
        int stagePrev;
        bool playing; // true = jugando, false = perdio

        std::string bestScore; // mejor tiempo

        Music tetrisTheme; 
        float volume;

        Piece piece;

        OutPiece pieceNext;
        OutPiece pieceSave;

        unsigned int gamePoints;
        unsigned int points;
        unsigned int time1;
        unsigned int time0;

    // STAGE 0
    void loopMenu(){
        if (ml){
            if (mouseInRect(mx, my, gap, gap*4.5, MeasureText("Play", gap*2/3), gap*2/3)){
                this->stage = 1;
                this->stagePrev = 0;

                this->time0 = GetTime();
                this->gamePoints = 0;
                this->playing = true;

                for (int i = 0; i<nx; i++){
                    for (int j = 0; j<ny; j++){
                        this->Board[i][j] = BLANK;
                    }
                }
            }
            if (mouseInRect(mx, my, gap, gap*5.5, MeasureText("Settings", gap*2/3), gap*2/3)){
                this->stage = 2;
                this->stagePrev = 0;
            }
            if (mouseInRect(mx, my, gap, gap*6.5, MeasureText("Quit"    , gap*2/3), gap*2/3)){
                CloseWindow();
            }
        }
    }

    void drawMenu(){
        ClearBackground(BACKGROUND);

        DrawText("TETRIS", gap, gap, gap*2, WHITE);
    
        DrawText("Play"    , gap, gap*4.5, gap*2/3, mouseInRect(mx, my, gap, gap*4.5, MeasureText("Play"    , gap*2/3), gap*2/3) ? GRAY : WHITE);
        DrawText("Settings", gap, gap*5.5, gap*2/3, mouseInRect(mx, my, gap, gap*5.5, MeasureText("Settings", gap*2/3), gap*2/3) ? GRAY : WHITE);
        DrawText("Quit"    , gap, gap*6.5, gap*2/3, mouseInRect(mx, my, gap, gap*6.5, MeasureText("Quit"    , gap*2/3), gap*2/3) ? GRAY : WHITE);

        DrawText("Best Score:", gap, gap*8, gap/2, GRAY);
        DrawText(strToChar(bestScore), gap + MeasureText("Best Score: ", gap/2), gap*8, gap/2, GRAY);
    }

    // STAGE 1
    void loopGame(){
        if (ml){
            if (mouseInRect(mx, my, gap + sq*nx, sq*ny, MeasureText("Settings", gap*0.5f), gap*0.5f)){
                this->stage = 2;
                this->stagePrev = 1;
                this->time0 -= GetTime();
            }
        }

        if (playing){
            this->time1 = GetTime();
            this->points = gamePoints + (time1 - time0)*10;

            if (!pieceNext.Initialized){
                createNewPiece(pieceNext);
            }
            if (!piece.Initialized){
                piece.init(pieceNext);
            }
            if (piece.Initialized){
                this->piece.fall(time0);
                this->piece.move(Board);
                this->piece.rotate(Board);
                if (IsKeyPressed(KEY_SPACE)){
                    if (pieceSave.Initialized){
                        swapPieces(pieceSave, piece);
                    } else {
                        swapPieces(pieceSave, piece);
                        piece.init(pieceNext);
                    }
                }
                if (IsKeyPressed(KEY_S)){
                    this->piece.SetFastFalling();
                }
                if (piece.shouldFreeze(Board)){
                    for (int k = 0; k<5; k++){ // freezea la pieza
                        int coordX, coordY;
                        coordX = piece.shapes[piece.pieceShape][piece.rotation%4][k][0] + piece.posX;
                        coordY = piece.shapes[piece.pieceShape][piece.rotation%4][k][1] + piece.posY;
                        this->Board[coordX][coordY] = piece.color;
                        this->piece.Initialized = false;
                    }

                    bool fullLine;
                    int NfullLines = 0;
                    for (int j = 1; j<ny; j++){ // borra las lineas llenas
                        fullLine = true;
                        for (int i = 0; i<nx; i++){
                            if (Board[i][j].a == 0){
                                fullLine = false;
                                break;
                            }
                        }
                        if (fullLine){
                            NfullLines++;
                            for (int k = j-1; k>=0; k--){
                                for (int i = 0; i<nx; i++){
                                    this->Board[i][k+1] = Board[i][k];
                                }
                            }
                        } 
                    }
                    switch (NfullLines){
                        case 0: this->gamePoints +=    0; break;
                        case 1: this->gamePoints +=  100; break;
                        case 2: this->gamePoints +=  250; break;
                        case 3: this->gamePoints +=  500; break;
                        case 4: this->gamePoints += 1000; break;
                    }

                    for (int i = 0; i<nx; i++){ // detecta si perdiste
                        if(Board[i][0].a != 0){
                            this->playing = false;
                            break;
                        }
                    }
                }
            }
        } else  {
            if (ml){
                if (mouseInRect(mx, my, gap + sq*nx, sq*ny - gap, MeasureText("Return to Menu", gap*0.5f), gap*0.5f)){
                    this->stage = 0;
                    this->stagePrev = 1;

                    std::ofstream wfile("data/scores.txt", std::ios::app);
                    wfile << points << '\n';
                    wfile.close();

                    // get the best score from the games played
                    std::ifstream rfile("data/scores.txt");

                    std::string line;
                    int bestScoreInt = 0;

                    while (std::getline(rfile, line)) {
                        if (bestScoreInt < strToInt(line)) {
                            bestScoreInt = strToInt(line);
                        }
                    }

                    this->bestScore = intToStr(bestScoreInt);

                    rfile.close();
                }
            }
        }
    }

    void drawGame(){
        ClearBackground(BACKGROUND);

        DrawRectangle(gap/2, gap/2, sq*nx, sq*ny, BLACK);
        for (int i = 0; i<nx; i++){
            for (int j = 0; j<ny; j++){
                if (Board[i][j].a != 0) {
                    DrawTetrisSq(gap/2.0f + i*sq, gap/2.0f + j*sq, sq, Board[i][j]);
                }
            }
        }
        if (piece.Initialized){
            float coords[2];
            for (int k = 0; k<5; k++){
                coords[0] = piece.shapes[piece.pieceShape][piece.rotation%4][k][0] + piece.posX;
                coords[1] = piece.shapes[piece.pieceShape][piece.rotation%4][k][1] + piece.posY;
                DrawTetrisSq(gap/2 + coords[0]*sq, gap/2 + coords[1]*sq, sq, piece.color);
            }
        }

        if (!playing) {
            DrawText("You LOSE", gap + sq*nx, gap*4.0f, gap, RED);
            DrawText("Return to Menu", gap + sq*nx, sq*ny - gap, gap*0.5f, mouseInRect(mx, my,  gap + sq*nx, sq*ny - gap, MeasureText("Return to Menu", gap*0.5f), gap*0.5f) ? GRAY : WHITE);
        }

        DrawText("Next Piece"  , gap + sq*nx, gap*0.5f       , gap*0.333f, WHITE);
        DrawRectangle(gap + sq*nx, gap        , sq*5, sq*4, BLACK);

        if (pieceNext.Initialized){
            for (int k = 0; k<5; k++){
                float coords[2];
                coords[0] = gap + sq*nx + shapes[pieceNext.Shape][0][k][0]*sq + (sq*5 - shapeDims[pieceNext.Shape][0]*sq)/2;
                coords[0] += shapeDims[pieceNext.Shape][0] > 2 ? sq : 0;
                coords[1] = gap         + shapes[pieceNext.Shape][0][k][1]*sq + (sq*4 - shapeDims[pieceNext.Shape][1]*sq)/2;
                DrawTetrisSq(coords[0], coords[1], sq, pieceNext.color);
            }
        }

        DrawText("Saved Piece ", gap + sq*nx, gap*1.5f + sq*4, gap*0.333f, WHITE);
        DrawRectangle(gap + sq*nx, gap*2 + sq*4, sq*5, sq*4, BLACK);

        if (pieceSave.Initialized){
            for (int k = 0; k<5; k++){
                float coords[2];
                coords[0] = gap   + sq*nx + shapes[pieceSave.Shape][0][k][0]*sq + (sq*5 - shapeDims[pieceSave.Shape][0]*sq)/2;
                coords[0] += shapeDims[pieceSave.Shape][0] > 2 ? sq : 0;
                coords[1] = gap*2 + sq*4  + shapes[pieceSave.Shape][0][k][1]*sq + (sq*4 - shapeDims[pieceSave.Shape][1]*sq)/2;
                DrawTetrisSq(coords[0], coords[1], sq, pieceSave.color);
            }
        }

        DrawText("Points: ", gap*2.5f + sq*nx + sq*4, gap*0.5f, gap, WHITE);
        std::string pointsStdStr = intToStr(points);
        char* pointsChar = strToChar(pointsStdStr);
        DrawText(pointsChar, gap*2.5f + sq*nx + sq*4 + MeasureText("Points: ", gap), gap*0.5f, gap, WHITE);
        delete[] pointsChar;

        std::string cronoStdStr = cronometro(time1 - time0); 
        char* cronoChar = strToChar(cronoStdStr);
        DrawText(cronoChar, gap*2.5f + sq*nx + sq*4, gap*2.0f, gap, WHITE);
        delete[] cronoChar;

        DrawText("Settings", gap + sq*nx, sq*ny, gap*0.5f, mouseInRect(mx, my,  gap + sq*nx, sq*ny, MeasureText("Settings", gap*0.5f), gap*0.5f) ? GRAY : WHITE);
    }

    // STAGE 2
    void loopSetg(){
        if (ml){
            if (mouseInRect(mx, my, gap, gap*8, MeasureText("Return", gap*2/3), gap*2/3)){
                if (stagePrev == 1) time0 += GetTime();
                this->stage = stagePrev;
                this->stagePrev = 2;
            }
            if (mouseInRect(mx, my, gap*7, gap*2.383f, gap*0.9f, gap*0.9f)){
                SetMasterVolume(0);
            }
            if (mouseInRect(mx, my, gap*7.0f, gap*3.533f, gap, gap)){
                this->resolution--;
            }
            if (mouseInRect(mx, my, gap*8.2f, gap*3.533f, gap, gap)){
                this->resolution++;
            }
            if (mouseInRect(mx, my, gap*9.4f, gap*3.533f, gap*1.6f, gap)){
                this->sx = difResolutions[resolution%8][0];
                this->sy = difResolutions[resolution%8][1];
                this->gap = sy/10;
                this->sq = (sy - gap)/ny;

                SetWindowSize(sx, sy);
            }
        }
        if(IsMouseButtonDown(0)){
            if(mouseInRect(mx, my, gap*8, gap*8/3, gap*5, gap/3)){
                this->volume = (mx - gap*8)/(float)(gap*5);
                SetMasterVolume(volume);
            }
        }
    }

    void drawSetg(){
        ClearBackground(BACKGROUND); // gap*7 en x empiezan los true setting
        DrawText("Settings", gap, gap, gap, WHITE);

        // BARRA DE VOLUMEN
        DrawText("Volume", gap*2, gap*2.5, gap*2/3, WHITE);

        Rectangle volumeBar;
        volumeBar.x = gap*8;
        volumeBar.y = gap*2.75;
        volumeBar.width = gap*5;
        volumeBar.height = gap/6;
        DrawRectangleRounded(volumeBar, 1, 0, GRAY);

        Rectangle currentVolumeBar = volumeBar;
        currentVolumeBar.width *= volume;
        DrawRectangleRounded(currentVolumeBar, 1, 0, GREEN);
        DrawCircle(gap*8 + gap*volume*5, gap*2.8333f, gap/6, WHITE);

        DrawRectangle(gap*7, gap*2.588f, gap*0.4, gap*0.5f, GRAY);
        DrawCircleSector((Vector2){gap*7.5f, gap*2.833f}, gap*0.2f, -90, 90, 0, GRAY);
        DrawCircleSector((Vector2){gap*7.55f, gap*2.833f}, gap*0.4f, 90, 270, 0, GRAY);

        if (GetMasterVolume() != 0){
            DrawRing((Vector2){gap*7.5f, gap*2.833f}, gap*0.25f, gap*0.28f, -70, 70, 0, GRAY);
            DrawRing((Vector2){gap*7.5f, gap*2.833f}, gap*0.35f, gap*0.38f, -70, 70, 0, GRAY);
        } else {
            DrawLineEx((Vector2){gap*7.0f, gap*2.383f}, (Vector2){gap*7.8f, gap*3.283f}, gap*0.1, RED);
        }

        // ELEGIR RESOLUCION
        DrawText("Resolution", gap*2, gap*3.7, gap*2/3, WHITE);

        DrawRectangle(gap*7.00f, gap*3.533f, gap, gap, GRAY);
        DrawText("-", gap*7.301f, gap*3.58f, gap, WHITE);

        DrawRectangle(gap*8.2f, gap*3.533f, gap, gap, GRAY);
        DrawText("+", gap*8.45f, gap*3.58f, gap, WHITE);

        DrawRectangle(gap*9.4f, gap*3.533f, gap*1.6f, gap, GRAY);
        DrawText("Apply", gap*9.5f, gap*3.783f, gap/2, WHITE);

        std::string resolutionStdStr = intToStr(difResolutions[resolution%8][0]) + (std::string)" x " + intToStr(difResolutions[resolution%8][1]);
        char* resolutionCharP = strToChar(resolutionStdStr);
        DrawText(resolutionCharP, gap*11.5, gap*3.7f, gap*2/3, WHITE);
        delete[] resolutionCharP;
    
        // VOLVER AL MENU O LA PARTIDA
        DrawText("Return", gap, gap*8, gap*2/3, mouseInRect(mx, my, gap, gap*8, MeasureText("Return", gap*2/3), gap*2/3) ? GRAY : WHITE);
    }

    public:
    // CONSTRUCTOR
    Game(){
        this->stage = 0;

        InitAudioDevice();
        this->tetrisTheme = LoadMusicStream("assets/tetrisTheme.mp3");

        // get the best score from the games played
        std::ifstream scoresFile("data/scores.txt");

        std::string line;
        int bestScoreInt = 0;

        while (std::getline(scoresFile, line)) {
            if (bestScoreInt < strToInt(line)) {
                bestScoreInt = strToInt(line);
            }
        }

        this->bestScore = intToStr(bestScoreInt);

        scoresFile.close();

        // get the previus config
        std::ifstream configFile("data/config.txt");

        std::string volume10kConfig, resolutionConfig;
        std::getline(configFile, volume10kConfig);
        this->volume = strToInt(volume10kConfig)/10000.0f;
        SetMasterVolume(volume);

        std::getline(configFile, resolutionConfig);
        this->resolution = strToInt(resolutionConfig);

        configFile.close();

        this->sx = difResolutions[resolution%8][0];
        this->sy = difResolutions[resolution%8][1];
        this->gap = sy/10;
        this->sq = (sy - gap)/ny;

        InitWindow(sx, sy, "Tetris");
        SetWindowPosition(0,0);
        SetTargetFPS(60);
    }

    // DERIVO EL VOID LOOP Y VOID DRAW AL PROPIO DE CADA STAGE
    void loop(){
        this->ml = IsMouseButtonPressed(0);
        this->mr = IsMouseButtonPressed(1);
        this->mc = IsMouseButtonPressed(2);

        this->mx = GetMouseX();
        this->my = GetMouseY();

        if (!IsMusicStreamPlaying(tetrisTheme)){
            PlayMusicStream(tetrisTheme);
        } else {
            UpdateMusicStream(tetrisTheme);
        }

        switch(stage){
            case 0: loopMenu(); break;
            case 1: loopGame(); break;
            case 2: loopSetg(); break;
        }
    }

    void draw(){
        switch(stage){
            case 0: drawMenu(); break;
            case 1: drawGame(); break;
            case 2: drawSetg(); break;
        }
    }

    void unload(){
        UnloadMusicStream(tetrisTheme);
    }

    void saveConfig(){
        std::ofstream configFile("data/config.txt");
        configFile << (int)(GetMasterVolume()*10000) << '\n' << resolution;
        configFile.close();
    }
};

int main(void) {
    Game game;

    while(!WindowShouldClose()){
        BeginDrawing();
            game.draw();
        EndDrawing();

        game.loop();
    }

    game.unload();
    game.saveConfig();
    return 0;
}