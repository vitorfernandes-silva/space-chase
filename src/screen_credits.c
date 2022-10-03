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

//----------------------------------------------------------------------------------
// Definição de variáveis ​​do módulo (local)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static int framesCounter = 0;
static int finishScreen = 0;
Texture2D texture_credits; 
Font fontWonderrr;
const char message11[] = "Projeto final da disciplina proposta \npela professora Joice Lee Otsuka e realizada \npelos alunos João Henrique Mendes Teixeira e \nVitor Fernandes Silva \nTodas as imagens utilizadas aqui são de uso livre. \nBackground music disponibilizada por D3nomite2: \nhttps://soundcloud.com/d3nomite2 \n\nGratidão por este semestre";

//----------------------------------------------------------------------------------
// Definição de funções da tela de RANKING
//----------------------------------------------------------------------------------

// Lógica de inicialização da tela de RANKING
void InitCreditsScreen(void)
{
    // TODO: Inicialize as variáveis ​​da tela RANKING aqui!
    framesCounter = 0;
    finishScreen = 0;
    texture_credits = LoadTexture("resources/bkg_gameplay.png");
    fontWonderrr = LoadFont("resources/8-bit_wonder.ttf");

}

// Lógica de atualização da tela de RANKING
void UpdateCreditsScreen(void)
{
     // TODO: Update DIALOG screen variables here!
    if (IsKeyDown(KEY_SPACE)) framesCounter += 55;
        else framesCounter+=4;

    // Press enter or tap to change to TITLE screen
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1;   // TITLE
        PlaySound(fxCoin);
    }
}

// Lógica de desenho de tela de RANKING
void DrawCreditsScreen(void)
{
    // TODO: Desenhe a tela RANKING aqui!
    texture_credits = LoadTexture("resources/bkg_gameplay.png");

    //        DrawText(TextSubtext(message1, 0, framesCounter/3), 30, 30, 15, WHITE);

    DrawTexture(texture_credits, screenWidth/2 - texture_credits.width/2, screenHeight/2 - texture_credits.height/2, WHITE);
    DrawText(TextSubtext(message11, 0, framesCounter/3), 20, 20, 26, WHITE);

    //        DrawTextEx(fontWonder,TextSubtext(message1, 0, framesCounter/10),(Vector2){30.0f,65.0f}, 13, 2, WHITE);

}

// Lógica de descarregamento da tela de RANKING
void UnloadCreditsScreen(void)
{
    // TODO: Descarregue as variáveis ​​da tela RANKING aqui!
    UnloadTexture(texture_credits);
    UnloadFont(fontWonderrr);
}

// A tela de RANKING deve terminar?
int FinishCreditsScreen(void)
{
    return finishScreen;
}