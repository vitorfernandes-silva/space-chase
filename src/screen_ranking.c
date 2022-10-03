/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Options Screen Functions Definitions (Init, Update, Draw, Unload)
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
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char nickname[9];
    int pontos;
}RankingJogador;


//----------------------------------------------------------------------------------
// Definição de variáveis ​​do módulo (local)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static int framesCounter = 0;
static int finishScreen = 0;
Texture2D texture_ranking;
RankingJogador *Jogadores;
Font font_ranking;
int tamJogador = 10;

//----------------------------------------------------------------------------------
// Definição de funções da tela de RANKING
//----------------------------------------------------------------------------------

FILE *fopen_e_teste3(char *caminho, char *modo)
{
    FILE *fp = fopen(caminho, modo);
    if (fp == NULL)
    {
        printf("Erro na abertura do aquivo [RANKING]\n");
        exit(1);
    }
    return fp;
}

int compare(const void *a, const void *b)
{
    RankingJogador *OrdenaA = a;
    RankingJogador *OrdenaB = b;

    return (OrdenaA->pontos - OrdenaB->pontos);
}

void OrdenaRanking(RankingJogador v[], int n)
{
    qsort(v, n, sizeof(RankingJogador), compare);
}

void ChamaRanking(void)
{
    FILE *file_rank = fopen_e_teste3("ranking_jogadores.bin", "rb");
    Jogadores = (RankingJogador*)malloc(tamJogador * sizeof(RankingJogador));
    if (Jogadores == NULL)
    {
        perror("Erro na alocacao dinamica [JOGADORES]\n");
        exit(1);
    }
    int i = 0;
    while (fread(&Jogadores[i], sizeof(RankingJogador), 1, file_rank)>0)
    {
        i++;
    }
    fclose(file_rank);
    OrdenaRanking(Jogadores, tamJogador);
}
// Lógica de inicialização da tela de RANKING
void InitRankingScreen(void)
{
    // TODO: Inicialize as variáveis ​​da tela RANKING aqui!
    ChamaRanking();
    framesCounter = 0;
    finishScreen = 0;
    font_ranking = LoadFont("resources/8-bit_wonder.ttf");
}

// Lógica de atualização da tela de RANKING
void UpdateRankingScreen(void)
{
    // TODO: Atualize as variáveis ​​da tela RANKING aqui!
    // Pressione ENDER para ir para mudar para TITLE
    if (IsKeyPressed(KEY_ENTER))
    {
        // finishScreen = 1; // LOGO
        finishScreen = 2; // TITLE
    }
    
}

// Lógica de desenho de tela de RANKING
void DrawRankingScreen(void)
{
    // TODO: Desenhe a tela RANKING aqui!
    texture_ranking = LoadTexture("resources/bkg_ranking.png");
    DrawTexture(texture_ranking, screenWidth/2 - texture_ranking.width/2, screenHeight/2 - texture_ranking.height/2, WHITE);
    for (int i = 0; i < tamJogador; i++)
    {
        DrawTextEx(font_ranking, TextFormat("%s\t-\t%d\n", Jogadores[i].nickname, Jogadores[i].pontos), (Vector2){screenWidth/2.0f, (screenHeight*i)/2.0f}, 14, 1, WHITE);
    }
}

// Lógica de descarregamento da tela de RANKING
void UnloadRankingScreen(void)
{
    // TODO: Descarregue as variáveis ​​da tela RANKING aqui!
    UnloadTexture(texture_ranking);
}

// A tela de RANKING deve terminar?
int FinishRankingScreen(void)
{
    return finishScreen;
}