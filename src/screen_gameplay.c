/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Gameplay Screen Functions Definitions (Init, Update, Draw, Unload)
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"
#include "screens.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Definições
//----------------------------------------------------------------------------------

#define PLAYER_BASE_SIZE    20.0f // Tamanho da base do triângulo isósceles [nave]
#define PLAYER_SPEED        6.0f // Velocidade da nave
#define PLAYER_MAX_SHOOTS   10 // Quantidade máxima de tiros da nave

#define METEORS_SPEED       3 // Velocidade dos meteoros
#define MAX_BIG_METEORS     6 // Quantidade máxima de meteoros grandes
#define MAX_MEDIUM_METEORS  12 // Quantidade máxima de meteoros médios
#define MAX_SMALL_METEORS   24 // Quantidade máxima de meteoros pequenos

//----------------------------------------------------------------------------------
// Definição de Tipos e Estruturas
//----------------------------------------------------------------------------------

typedef struct Player {
    Vector2 position; // Posição em (x,y) da nave
    Vector2 speed; // velocidade em (x,y) da nave
    float acceleration; // aceleração da nave
    float rotation; // rotação da nave
    Vector3 collider; // colisor em (x,y,z)
    Color color; // Cor da nave
} Player;

typedef struct Shoot {
    Vector2 position; // Posição do tiro em (x,y) (que sai da nave)
    Vector2 speed; // Velocidade do tiro em (x,y) (que sai da nave)
    float radius; // Tamanho do raio do tiro
    float rotation; // Rotação do tiro
    int lifeSpawn; // Cria da vida
    bool active; // Ativo (verdadeiro ou falso)
    Color color; // Cor do tiro
} Shoot;

typedef struct Meteor {
    Vector2 position; // Posição do meteoro em (x,y)
    Vector2 speed; // Velocidade do meteoro em (x,y)
    float radius; // Tamanho do raio do meteoro
    bool active; // Ativo (Verdadeiro ou Falso)
    Color color; // Cor do meteoro
} Meteor;

typedef struct
{
    char nickname[9]; // Nome do jogador
    int pontos; // Score do jogador
}Jogador;


//----------------------------------------------------------------------------------
// Definição de variáveis ​​globais
//----------------------------------------------------------------------------------

Texture2D texture_gameplay; // Variável para textura da tela de gameplay
Font font_gameplay;
Jogador *jogador;
int countJogador = 0;
extern int tamJ = 10;
static int score = 0;
static const int screenWidth = 800; // Largura da tela
static const int screenHeight = 450; // Altura da tela
static int framesCounter = 0; // Contador de frames
static int finishScreen = 0; // Variável que controla a transição das telas
static bool gameOver = false; // Condição de parada do jogo
static bool pause = false; // Variável de pause do jogo
static bool victory = false; // Condição de vitória

// NOTA: O triângulo definido é isósceles com ângulos comuns de 70 graus.
static float shipHeight = 0.0f; // altura da nave
static Player player = { 0 }; // Struct estática do tipo Player
static Shoot shoot[PLAYER_MAX_SHOOTS] = { 0 }; // Vetor de Struct estática do tipo Shoot
static Meteor bigMeteor[MAX_BIG_METEORS] = { 0 }; // Vetor de Struct estática do tipo Meteor 
static Meteor mediumMeteor[MAX_MEDIUM_METEORS] = { 0 }; // Vetor de Struct estática do tipo Meteor
static Meteor smallMeteor[MAX_SMALL_METEORS] = { 0 }; // Vetor de Struct estática do tipo Meteor
static int midMeteorsCount = 0; // contador de meteoros médios
static int smallMeteorsCount = 0; // contador de meteoros pequenos
static int destroyedMeteorsCount = 0; // contador de meteoros destruídos

//----------------------------------------------------------------------------------
// **************** Definição das funções da tela de jogabilidade ****************
//----------------------------------------------------------------------------------

FILE *fopen_e_teste2(char *caminho, char *modo)
{
    FILE *f;
    f = fopen(caminho, modo);
    if (f == NULL)
    {
        perror("ERRO NA ABERTURA DO ARQUIVO!\n");
        exit(1);
    }
    return f;
}

void CriaJogador(Jogador *jogadores)
{
    FILE *rank = fopen_e_teste2("ranking_jogadores.bin", "ab");
    fwrite(jogadores, sizeof(jogador), tamJ, rank);
    fclose(rank);
}

void CarregaScoreJogador(int Score)
{
    FILE *f = fopen_e_teste2("nome_jogadores.bin", "rb");
    jogador = (Jogador*)malloc(tamJ * sizeof(Jogador));
    if (jogador == NULL)
    {
        perror("Erro na alocacao [JOGADOR]\n");
        exit(1);
    }
    fseek(f, sizeof(char), SEEK_END);
    while (fread(&jogador[countJogador], sizeof(Jogador), 1, f) >0)
    {
        jogador[countJogador].pontos = Score;
        countJogador++;
    }
    fclose(f);
    CriaJogador(jogador);
}

// Lógica de inicialização da tela de jogabilidade
//----------------------------------------------------------------------------------

void InitGameplayScreen(void)
{
    // TODO: Inicialize as variáveis ​​da tela GAMEPLAY aqui!
    //texture_gameplay = LoadTexture("resources/bkg_gameplay.png");
    framesCounter = 0;
    finishScreen = 0;
    int posx, posy; // Posição em X e Y
    int velx, vely; // Velocidade em X e Y
    bool correctRange = false; // Intervalo Correto
    victory = false; // Vitória não foi conquistada ainda, por isso [false]
    pause = false; // o jogo não foi pausado por isso [false]
    texture_gameplay = LoadTexture("resources/bkg_gameplay.png");
    font_gameplay = LoadFont("resources/8-bit_wonder.ttf");

    shipHeight = (PLAYER_BASE_SIZE/2)/tanf(20*DEG2RAD); // Cálculo da altura da nave

    // -> Inicialização do player
    player.position = (Vector2){screenWidth/2, screenHeight/2 - shipHeight/2}; // Nasce no meio da tela
    player.speed = (Vector2){0, 0}; // Velocidade em (x,y) = 0
    player.acceleration = 0; // aceleração = 0
    player.rotation = 0; // rotação = 0
    player.collider = (Vector3){player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight/2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight/2.5f), 12};
    player.color = LIGHTGRAY;

    destroyedMeteorsCount = 0; // Inicializando o contador de meteoros destruídos

    // Inicialização do shoot (tiros[10])
    for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
    {
        shoot[i].position = (Vector2){0, 0}; // Posição = 0
        shoot[i].speed = (Vector2){0, 0}; // Velocidade = 0
        shoot[i].radius = 2; // Raio = 2
        shoot[i].active = false; // Não está ativo no momento
        shoot[i].lifeSpawn = 0; // Cria da vida
        shoot[i].color = WHITE; // Cor branca
    }
    // Coloca os meteoros em movimentação aleatória
    for (int i = 0; i < MAX_BIG_METEORS; i++)
    {
        posx = GetRandomValue(0, screenWidth);

        while (!correctRange)
        {
            if (posx > screenWidth/2 - 150 && posx < screenWidth/2 + 150) posx = GetRandomValue(0, screenWidth);
            else correctRange = true;
        }

        correctRange = false;

        posy = GetRandomValue(0, screenHeight);

        while (!correctRange)
        {
            if (posy > screenHeight/2 - 150 && posy < screenHeight/2 + 150)  posy = GetRandomValue(0, screenHeight);
            else correctRange = true;
        }

        bigMeteor[i].position = (Vector2){posx, posy};

        correctRange = false;
        velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
        vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

        while (!correctRange)
        {
            if (velx == 0 && vely == 0)
            {
                velx = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
                vely = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
            }
            else correctRange = true;
        }

        bigMeteor[i].speed = (Vector2){velx, vely};
        bigMeteor[i].radius = 40;
        bigMeteor[i].active = true;
        bigMeteor[i].color = BLUE;
    }

    for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
    {
        mediumMeteor[i].position = (Vector2){-100, -100};
        mediumMeteor[i].speed = (Vector2){0,0};
        mediumMeteor[i].radius = 20;
        mediumMeteor[i].active = false;
        mediumMeteor[i].color = BLUE;
    }

    for (int i = 0; i < MAX_SMALL_METEORS; i++)
    {
        smallMeteor[i].position = (Vector2){-100, -100};
        smallMeteor[i].speed = (Vector2){0,0};
        smallMeteor[i].radius = 10;
        smallMeteor[i].active = false;
        smallMeteor[i].color = BLUE;
    }

    midMeteorsCount = 0;
    smallMeteorsCount = 0;
}

//----------------------------------------------------------------------------------------
// Lógica de atualização da tela de jogabilidade
//----------------------------------------------------------------------------------------

void UpdateGameplayScreen(void)
{
    // TODO: Atualize as variáveis ​​da tela GAMEPLAY aqui!
    score = 0;
    gameOver = false;
        if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            // Player logic: rotation
            if (IsKeyDown(KEY_LEFT)) player.rotation -= 5;
            if (IsKeyDown(KEY_RIGHT)) player.rotation += 5;

            // Player logic: speed
            player.speed.x = sin(player.rotation*DEG2RAD)*PLAYER_SPEED;
            player.speed.y = cos(player.rotation*DEG2RAD)*PLAYER_SPEED;

            // Player logic: acceleration
            if (IsKeyDown(KEY_UP))
            {
                if (player.acceleration < 1) player.acceleration += 0.04f;
            }
            else
            {
                if (player.acceleration > 0) player.acceleration -= 0.02f;
                else if (player.acceleration < 0) player.acceleration = 0;
            }
            if (IsKeyDown(KEY_DOWN))
            {
                if (player.acceleration > 0) player.acceleration -= 0.04f;
                else if (player.acceleration < 0) player.acceleration = 0;
            }

            // Player logic: movement
            player.position.x += (player.speed.x*player.acceleration);
            player.position.y -= (player.speed.y*player.acceleration);

            // Collision logic: player vs walls
            if (player.position.x > screenWidth + shipHeight) player.position.x = -(shipHeight);
            else if (player.position.x < -(shipHeight)) player.position.x = screenWidth + shipHeight;
            if (player.position.y > (screenHeight + shipHeight)) player.position.y = -(shipHeight);
            else if (player.position.y < -(shipHeight)) player.position.y = screenHeight + shipHeight;

            // Player shoot logic
            if (IsKeyPressed(KEY_SPACE))
            {
                for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
                {
                    if (!shoot[i].active)
                    {
                        shoot[i].position = (Vector2){ player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight) };
                        shoot[i].active = true;
                        shoot[i].speed.x = 1.5*sin(player.rotation*DEG2RAD)*PLAYER_SPEED;
                        shoot[i].speed.y = 1.5*cos(player.rotation*DEG2RAD)*PLAYER_SPEED;
                        shoot[i].rotation = player.rotation;
                        break;
                    }
                }
            }

            // Shoot life timer
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if (shoot[i].active) shoot[i].lifeSpawn++;
            }

            // Shot logic
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movement
                    shoot[i].position.x += shoot[i].speed.x;
                    shoot[i].position.y -= shoot[i].speed.y;

                    // Collision logic: shoot vs walls
                    if  (shoot[i].position.x > screenWidth + shoot[i].radius)
                    {
                        shoot[i].active = false;
                        shoot[i].lifeSpawn = 0;
                    }
                    else if (shoot[i].position.x < 0 - shoot[i].radius)
                    {
                        shoot[i].active = false;
                        shoot[i].lifeSpawn = 0;
                    }
                    if (shoot[i].position.y > screenHeight + shoot[i].radius)
                    {
                        shoot[i].active = false;
                        shoot[i].lifeSpawn = 0;
                    }
                    else if (shoot[i].position.y < 0 - shoot[i].radius)
                    {
                        shoot[i].active = false;
                        shoot[i].lifeSpawn = 0;
                    }

                    // Life of shoot
                    if (shoot[i].lifeSpawn >= 60)
                    {
                        shoot[i].position = (Vector2){0, 0};
                        shoot[i].speed = (Vector2){0, 0};
                        shoot[i].lifeSpawn = 0;
                        shoot[i].active = false;
                    }
                }
            }

            // Collision logic: player vs meteors
            player.collider = (Vector3){player.position.x + sin(player.rotation*DEG2RAD)*(shipHeight/2.5f), player.position.y - cos(player.rotation*DEG2RAD)*(shipHeight/2.5f), 12};

            for (int a = 0; a < MAX_BIG_METEORS; a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, bigMeteor[a].position, bigMeteor[a].radius) && bigMeteor[a].active) 
                {
                    gameOver = true;
                    CarregaScoreJogador(score);
                    finishScreen = 1; // RANKING
                }
            }

            for (int a = 0; a < MAX_MEDIUM_METEORS; a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, mediumMeteor[a].position, mediumMeteor[a].radius) && mediumMeteor[a].active)
                {
                    gameOver = true;
                    CarregaScoreJogador(score);
                    finishScreen = 1; // RANKING
                }
            }

            for (int a = 0; a < MAX_SMALL_METEORS; a++)
            {
                if (CheckCollisionCircles((Vector2){player.collider.x, player.collider.y}, player.collider.z, smallMeteor[a].position, smallMeteor[a].radius) && smallMeteor[a].active)
                {
                    gameOver = true;
                    CarregaScoreJogador(score);
                    finishScreen = 1; // RANKING
                }
            }

            // Meteors logic: big meteors
            for (int i = 0; i < MAX_BIG_METEORS; i++)
            {
                if (bigMeteor[i].active)
                {
                    // Movement
                    bigMeteor[i].position.x += bigMeteor[i].speed.x;
                    bigMeteor[i].position.y += bigMeteor[i].speed.y;

                    // Collision logic: meteor vs wall
                    if  (bigMeteor[i].position.x > screenWidth + bigMeteor[i].radius) bigMeteor[i].position.x = -(bigMeteor[i].radius);
                    else if (bigMeteor[i].position.x < 0 - bigMeteor[i].radius) bigMeteor[i].position.x = screenWidth + bigMeteor[i].radius;
                    if (bigMeteor[i].position.y > screenHeight + bigMeteor[i].radius) bigMeteor[i].position.y = -(bigMeteor[i].radius);
                    else if (bigMeteor[i].position.y < 0 - bigMeteor[i].radius) bigMeteor[i].position.y = screenHeight + bigMeteor[i].radius;
                }
            }

            // Meteors logic: medium meteors
            for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
            {
                if (mediumMeteor[i].active)
                {
                    // Movement
                    mediumMeteor[i].position.x += mediumMeteor[i].speed.x;
                    mediumMeteor[i].position.y += mediumMeteor[i].speed.y;

                    // Collision logic: meteor vs wall
                    if  (mediumMeteor[i].position.x > screenWidth + mediumMeteor[i].radius) mediumMeteor[i].position.x = -(mediumMeteor[i].radius);
                    else if (mediumMeteor[i].position.x < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.x = screenWidth + mediumMeteor[i].radius;
                    if (mediumMeteor[i].position.y > screenHeight + mediumMeteor[i].radius) mediumMeteor[i].position.y = -(mediumMeteor[i].radius);
                    else if (mediumMeteor[i].position.y < 0 - mediumMeteor[i].radius) mediumMeteor[i].position.y = screenHeight + mediumMeteor[i].radius;
                }
            }

            // Meteors logic: small meteors
            for (int i = 0; i < MAX_SMALL_METEORS; i++)
            {
                if (smallMeteor[i].active)
                {
                    // Movement
                    smallMeteor[i].position.x += smallMeteor[i].speed.x;
                    smallMeteor[i].position.y += smallMeteor[i].speed.y;

                    // Collision logic: meteor vs wall
                    if  (smallMeteor[i].position.x > screenWidth + smallMeteor[i].radius) smallMeteor[i].position.x = -(smallMeteor[i].radius);
                    else if (smallMeteor[i].position.x < 0 - smallMeteor[i].radius) smallMeteor[i].position.x = screenWidth + smallMeteor[i].radius;
                    if (smallMeteor[i].position.y > screenHeight + smallMeteor[i].radius) smallMeteor[i].position.y = -(smallMeteor[i].radius);
                    else if (smallMeteor[i].position.y < 0 - smallMeteor[i].radius) smallMeteor[i].position.y = screenHeight + smallMeteor[i].radius;
                }
            }

            // Collision logic: player-shoots vs meteors
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if ((shoot[i].active))
                {
                    for (int a = 0; a < MAX_BIG_METEORS; a++)
                    {
                        if (bigMeteor[a].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, bigMeteor[a].position, bigMeteor[a].radius))
                        {
                            shoot[i].active = false;
                            shoot[i].lifeSpawn = 0;
                            bigMeteor[a].active = false;
                            destroyedMeteorsCount++;
                            score += 100;

                            for (int j = 0; j < 2; j ++)
                            {
                                if (midMeteorsCount%2 == 0)
                                {
                                    mediumMeteor[midMeteorsCount].position = (Vector2){bigMeteor[a].position.x, bigMeteor[a].position.y};
                                    mediumMeteor[midMeteorsCount].speed = (Vector2){cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1};
                                }
                                else
                                {
                                    mediumMeteor[midMeteorsCount].position = (Vector2){bigMeteor[a].position.x, bigMeteor[a].position.y};
                                    mediumMeteor[midMeteorsCount].speed = (Vector2){cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED};
                                }

                                mediumMeteor[midMeteorsCount].active = true;
                                midMeteorsCount ++;
                            }
                            //bigMeteor[a].position = (Vector2){-100, -100};
                            bigMeteor[a].color = RED;
                            a = MAX_BIG_METEORS;
                        }
                    }

                    for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
                    {
                        if (mediumMeteor[b].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, mediumMeteor[b].position, mediumMeteor[b].radius))
                        {
                            shoot[i].active = false;
                            shoot[i].lifeSpawn = 0;
                            mediumMeteor[b].active = false;
                            destroyedMeteorsCount++;
                            score += 50;

                            for (int j = 0; j < 2; j ++)
                            {
                                 if (smallMeteorsCount%2 == 0)
                                {
                                    smallMeteor[smallMeteorsCount].position = (Vector2){mediumMeteor[b].position.x, mediumMeteor[b].position.y};
                                    smallMeteor[smallMeteorsCount].speed = (Vector2){cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED*-1};
                                }
                                else
                                {
                                    smallMeteor[smallMeteorsCount].position = (Vector2){mediumMeteor[b].position.x, mediumMeteor[b].position.y};
                                    smallMeteor[smallMeteorsCount].speed = (Vector2){cos(shoot[i].rotation*DEG2RAD)*METEORS_SPEED, sin(shoot[i].rotation*DEG2RAD)*METEORS_SPEED};
                                }

                                smallMeteor[smallMeteorsCount].active = true;
                                smallMeteorsCount ++;
                            }
                            //mediumMeteor[b].position = (Vector2){-100, -100};
                            mediumMeteor[b].color = GREEN;
                            b = MAX_MEDIUM_METEORS;
                        }
                    }

                    for (int c = 0; c < MAX_SMALL_METEORS; c++)
                    {
                        if (smallMeteor[c].active && CheckCollisionCircles(shoot[i].position, shoot[i].radius, smallMeteor[c].position, smallMeteor[c].radius))
                        {
                            shoot[i].active = false;
                            shoot[i].lifeSpawn = 0;
                            smallMeteor[c].active = false;
                            destroyedMeteorsCount++;
                            score += 25;
                            smallMeteor[c].color = YELLOW;
                           // smallMeteor[c].position = (Vector2){-100, -100};
                            c = MAX_SMALL_METEORS;
                        }
                    }
                }
            }
        }

        if (destroyedMeteorsCount == MAX_BIG_METEORS + MAX_MEDIUM_METEORS + MAX_SMALL_METEORS) 
        {
            victory = true;
            CarregaScoreJogador(score);
            finishScreen = 1; // RANKING
        }
    }
    
}

// Lógica de desenho de tela de jogabilidade
void DrawGameplayScreen(void)
{
    // TODO: Desenhe a tela GAMEPLAY aqui!
      
    DrawTexture(texture_gameplay, screenWidth/2 - texture_gameplay.width/2, screenHeight/2 - texture_gameplay.height/2, WHITE);

        DrawRectangle(0, 0, 150, 50, RED);
        DrawTextEx(font_gameplay,TextFormat("SCORE [%d]", score), (Vector2){5.0f, 5.0f}, 13, 1, WHITE);

        if (!gameOver)
        {
            // Draw spaceship
            Vector2 v1 = { player.position.x + sinf(player.rotation*DEG2RAD)*(shipHeight), player.position.y - cosf(player.rotation*DEG2RAD)*(shipHeight) };
            Vector2 v2 = { player.position.x - cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2), player.position.y - sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2) };
            Vector2 v3 = { player.position.x + cosf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2), player.position.y + sinf(player.rotation*DEG2RAD)*(PLAYER_BASE_SIZE/2) };
            DrawTriangle(v1, v2, v3, MAROON);

            // Draw meteors
            for (int i = 0; i < MAX_BIG_METEORS; i++)
            {
                if (bigMeteor[i].active) DrawCircleV(bigMeteor[i].position, bigMeteor[i].radius, DARKGRAY);
                else DrawCircleV(bigMeteor[i].position, bigMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
            }

            for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
            {
                if (mediumMeteor[i].active) DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, GRAY);
                else DrawCircleV(mediumMeteor[i].position, mediumMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
            }

            for (int i = 0; i < MAX_SMALL_METEORS; i++)
            {
                if (smallMeteor[i].active) DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, GRAY);
                else DrawCircleV(smallMeteor[i].position, smallMeteor[i].radius, Fade(LIGHTGRAY, 0.3f));
            }

            // Draw shoot
            for (int i = 0; i < PLAYER_MAX_SHOOTS; i++)
            {
                if (shoot[i].active) DrawCircleV(shoot[i].position, shoot[i].radius, RED);
            }

            if (victory)
            {
                finishScreen = 1; // Vai para a tela ENDING
                PlaySound(fxCoin);
            }

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        //else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
}

// Lógica de descarregamento da tela de jogabilidade
void UnloadGameplayScreen(void)
{
    // TO-DO: Descarregue as variáveis ​​da tela GAMEPLAY aqui!
    UnloadTexture(texture_gameplay);
    UnloadFont(font_gameplay);
}

// A tela de jogabilidade deve terminar?
int FinishGameplayScreen(void)
{
    return finishScreen;
}