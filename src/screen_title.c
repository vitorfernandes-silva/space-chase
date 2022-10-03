/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Title Screen Functions Definitions (Init, Update, Draw, Unload)
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
Texture2D texture_title; 
//----------------------------------------------------------------------------------
// Definição das Funções da Tela de Título
//----------------------------------------------------------------------------------

// Lógica de inicialização da tela de título
void InitTitleScreen(void)
{
    // TODO: Inicialize as variáveis ​​da tela TITLE aqui!
    framesCounter = 0;
    finishScreen = 0;
    texture_title = LoadTexture("resources/bkg_title.png");
}

// Lógica de atualização da tela de título
void UpdateTitleScreen(void)
{
    // TODO: Atualize as variáveis ​​da tela TITLE aqui!

    // Pressione ENTER para mudar para a tela GAMEPLAY
    if (IsKeyPressed(KEY_ENTER))
    {
        finishScreen = 1; // OPTIONS
        PlaySound(fxCoin);
    }

    // Pressione R para mudar para a tela de RANKING
    if (IsKeyPressed(KEY_R))
    {
        finishScreen = 4; // RANKING
        PlaySound(fxCoin);
    }
    
}

// Lógica de desenho da tela de título
void DrawTitleScreen(void)
{
    // TODO: Desenhe a tela TITLE aqui!
    DrawTexture(texture_title, screenWidth/2 - texture_title.width/2, screenHeight/2 - texture_title.height/2, WHITE);
    
}

// Lógica de descarregamento da tela de título
void UnloadTitleScreen(void)
{
    // TODO: Descarregue as variáveis ​​da tela TITLE aqui!
    UnloadTexture(texture_title);
}

// A tela de título deve terminar?
int FinishTitleScreen(void)
{
    return finishScreen;
}