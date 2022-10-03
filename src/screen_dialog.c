
/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Dialog Screen Functions Definitions (Init, Update, Draw, Unload)
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
// Module Variables Definition (local)
//----------------------------------------------------------------------------------
Texture2D texture_dialog;
Font fontWonder;
static const int screenWidth = 800;
static const int screenHeight = 450;
static int framesCounter = 0;
static int finishScreen = 0;

// NARRATIVA 
const char message1[] = "Partindo de uma releitura do clássico e nostálgico jogo dos anos 70 do Nintendo, \no Gladius, elaboramos um jogo inspirado e com alguns recursos e visuais mais modernos, \nporém mantendo a essência do 8-bits. \n\n“Em um mundo onde a tecnologia havia avançado exponencialmente e a humanidade vivia em \nestações espaciais futuristas pois o planeta Terra havia sido desolada por \numa guerra nuclear sem fim, naves interestelares encontram diversos desafios para a \nmineração de combustíveis, ao adentrar pelo cinturão de asteroides situado entre \nMarte e Júpiter, a tripulação da nave 0800xTB acaba entrando em apuros e cabe \na você ajudar essa tripulação a superar este obstáculo para chegar \na salvo na nave-mãe." "\n\nO objetivo do jogo é desviar dos asteroides que aparecerem na tela, inicialmente, \no jogador tem a opção de somente desviar, porém, a nave é equipada com um míssil que pode destruir \nos asteroides, o problema nisso? Os asteroides acabam se dividindo em vários detritos \nque podem tornar a vida do jogador ainda mais difícil. A pontuação é dada por asteroides destruídos. \n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tA tripulação da nave 0800xTB depende de você, boa sorte.";
const char message2[] = "APERTE ESPAÇO PARA ACELERAR OU ENTER PARA PULAR.";
//----------------------------------------------------------------------------------
// DIALOG Screen Functions Definition
//----------------------------------------------------------------------------------

// DIALOG Screen Initialization logic
void InitDialogScreen(void)
{
    // TODO: Initialize DIALOG screen variables here!
    framesCounter = 0;
    finishScreen = 0;
    texture_dialog = LoadTexture("resources/bkg_gameplay.png");
    fontWonder = LoadFont("resources/8-bit_wonder.ttf");
}

// DIALOG Screen Update logic
void UpdateDialogScreen(void)
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

// DIALOG Screen Draw logic
void DrawDialogScreen(void)
{
    // TODO: Draw DIALOG screen here!
    DrawTexture(texture_dialog, screenWidth/2 - texture_dialog.width/2, screenHeight/2 - texture_dialog.height/2, WHITE);


        DrawText(TextSubtext(message1, 0, framesCounter/3), 30, 30, 15, WHITE);
        DrawText(TextSubtext(message2, 0, framesCounter), 220, 430, 12, GRAY);
        //DrawTextEx(fontWonder,TextSubtext(message2, 0, framesCounter/10),(Vector2){30.0f,130.0f}, 13, 2, WHITE);
}

// DIALOG Screen Unload logic
void UnloadDialogScreen(void)
{
    // TODO: Unload DIALOG screen variables here!
    UnloadTexture(texture_dialog);
    UnloadFont(fontWonder);
}

// DIALOG Screen should finish?
int FinishDialogScreen(void)
{
    return finishScreen;
}