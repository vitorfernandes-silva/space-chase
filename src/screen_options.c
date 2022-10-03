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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_CHARS 9


//----------------------------------------------------------------------------------
// Definição de variáveis ​​do módulo (local)
//----------------------------------------------------------------------------------
Texture2D texture_options;
Font fontWonderr;
static const int screenWidth = 800;
static const int screenHeight = 450;
static int framesCounter = 0;
static int finishScreen = 0;
Rectangle textBox = { screenWidth/2.0f - 100, 180, 225, 50 };
bool mouseOnText = false;
char name[MAX_INPUT_CHARS + 1] = "\0"; // NOTE: One extra space required for null terminator char '\0'
int letterCount = 0;

//----------------------------------------------------------------------------------
// Definição de funções da tela de opções
//----------------------------------------------------------------------------------

FILE *fopen_e_teste(char *caminho, char *modo)
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

void CarregaNomeJogador(char *nickname)
{
    FILE *f = fopen_e_teste("nome_jogadores.bin", "ab");
    fwrite(nickname, sizeof(char), strlen(nickname), f);
    fclose(f);
}

// Lógica de inicialização da tela de opções
void InitOptionsScreen(void)
{
    // TODO: Inicialize as variáveis ​​da tela OPTIONS aqui!
    framesCounter = 0;
    finishScreen = 0;
    texture_options = LoadTexture("resources/bkg_gameplay.png");
    fontWonderr = LoadFont("resources/8-bit_wonder.ttf");
}

// Lógica de atualização da tela de opções
void UpdateOptionsScreen(void)
{
    // TODO: Atualize as variáveis ​​da tela OPTIONS aqui!
     if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            // Defina o cursor da janela para o I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Obter char pressionado (caractere unicode) na fila
            int key = GetCharPressed();

            // Verifique se mais caracteres foram pressionados no mesmo quadro
            while (key > 0)
            {
                // NOTE: Permitir apenas chaves no intervalo [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount+1] = '\0'; // Adiciona terminador nulo no final da string.
                    letterCount++;
                }

                key = GetCharPressed();  // Verifique o próximo caractere na fila
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;

    // Pressione ENTER para mudar para a tela GAMEPLAY
    if (IsKeyPressed(KEY_ENTER))
    {
        CarregaNomeJogador(name);
        finishScreen = 1; // GAMEPLAY
        PlaySound(fxCoin);
    }
    
}

// Lógica de desenho de tela de opções
void DrawOptionsScreen(void)
{
    // TODO: Desenhe a tela OPÇÕES aqui!
    // ClearBackground(RAYWHITE);
    DrawTexture(texture_options, screenWidth/2 - texture_options.width/2, screenHeight/2 - texture_options.height/2, WHITE);

        DrawTextEx(fontWonderr,TextSubtext(TextFormat("INSIRA SEU NOME"), 0, framesCounter/10), (Vector2){60.0f,65.0f}, 50, 2, WHITE);

        DrawRectangleRec(textBox, LIGHTGRAY);
        if (mouseOnText)
        {
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, BLACK);
        }
        else
        {
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
        }

        DrawTextEx(fontWonderr, name, (Vector2){(int)textBox.x + 8, (int)textBox.y + 15}, 17, 1, MAROON);

        DrawTextEx(fontWonderr,TextFormat("CARACTERES (%iI%i)", letterCount, MAX_INPUT_CHARS),(Vector2){280.0f, 250.0f}, 18, 2, WHITE);

        if (mouseOnText)
        {
            if (letterCount < MAX_INPUT_CHARS)
            {
                // Draw blinking underscore char
                if (((framesCounter/20)%2) == 0)
                {
                    DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
                }
            }
            else
            {
                DrawTextEx(fontWonderr,TextFormat("PRESSIONE BACKSPACE PARA DELETAR CARACTERES."), (Vector2){230.0f, 300.0f}, 8, 1, GRAY);
            }
        }
}

// Lógica de descarregamento da tela de opções
void UnloadOptionsScreen(void)
{
    // TODO: Descarregue as variáveis ​​da tela OPTIONS aqui!
    UnloadTexture(texture_options);
    UnloadFont(fontWonderr);
}

// Check if any key is pressed
// NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}

// A tela de opções deve terminar?
int FinishOptionsScreen(void)
{
    return finishScreen;
}