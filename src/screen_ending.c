/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Definições das Funções da Tela Final (Init, Update, Draw, Unload)
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

// ----------------------------------------------------------------------------------
// Definição de variáveis ​​do módulo (local)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;
static int framesCounter = 0; 
static int finishScreen = 0;
Texture2D texture_ending;
//----------------------------------------------------------------------------------
// Definição das funções da tela final
//----------------------------------------------------------------------------------

// Finalizando a lógica de inicialização da tela
void InitEndingScreen(void)
{
    // TODO: Inicialize as variáveis ​​da tela ENDING aqui!
    framesCounter = 0;
    finishScreen = 0;
}

// Lógica de atualização de tela final
void UpdateEndingScreen(void)
{
    // TODO: Atualize as variáveis ​​da tela ENDING aqui!

    // Pressione enter ou toque para retornar à tela RANKING
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        finishScreen = 1; // RANKING
        PlaySound(fxCoin);
    }
}

// Finalizando a lógica de desenho da tela
void DrawEndingScreen(void)
{
    // TODO: Desenhe a tela FINAL aqui!
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLUE);
    texture_ending = LoadTexture("resources/bkg_ending.png");
    DrawTexture(texture_ending, screenWidth/2 - texture_ending.width/2, screenHeight/2 - texture_ending.height/2, WHITE);
}

// Finalizando a lógica de descarregamento da tela
void UnloadEndingScreen(void)
{
    // TODO: Descarregue as variáveis ​​da tela ENDING aqui!
    UnloadTexture(texture_ending);
}

// A tela final deve terminar?
int FinishEndingScreen(void)
{
    return finishScreen;
}