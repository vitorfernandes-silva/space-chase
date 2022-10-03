/**********************************************************************************************
*
*   raylib - Advance Game template
*
*   Screens Functions Declarations (Init, Update, Draw, Unload)
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

#ifndef SCREENS_H
#define SCREENS_H

//----------------------------------------------------------------------------------
// Definição de Tipos e Estruturas
//----------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, DIALOG, TITLE, OPTIONS, GAMEPLAY, ENDING, RANKING, CREDITS } GameScreen;

//----------------------------------------------------------------------------------
// Declaração de Variáveis ​​Globais (compartilhada por vários módulos)
//----------------------------------------------------------------------------------
extern GameScreen currentScreen;
extern Font font;
extern Music music;
extern Sound fxCoin;

#ifdef __cplusplus
extern "C" {            // Impede o desmembramento de nomes de funções
#endif

//----------------------------------------------------------------------------------
// Declaração de funções da tela de logotipo
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// Declaração de funções da tela de Diálogo
//----------------------------------------------------------------------------------

void InitDialogScreen(void);
void UpdateDialogScreen(void);
void DrawDialogScreen(void);
void UnloadDialogScreen(void);
int FinishDialogScreen(void);

//----------------------------------------------------------------------------------
// Declaração de funções da tela de título
//----------------------------------------------------------------------------------
void InitTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
void UnloadTitleScreen(void);
int FinishTitleScreen(void);

//----------------------------------------------------------------------------------
// Declaração de funções da tela de opções
//----------------------------------------------------------------------------------
void InitOptionsScreen(void);
void UpdateOptionsScreen(void);
void DrawOptionsScreen(void);
void UnloadOptionsScreen(void);
int FinishOptionsScreen(void);
void CarregaNomeJogador(char *nickname);
bool IsAnyKeyPressed(void);

//----------------------------------------------------------------------------------
// Declaração de funções da tela de jogo
//----------------------------------------------------------------------------------
void InitGameplayScreen(void);
void UpdateGameplayScreen(void);
void DrawGameplayScreen(void);
void UnloadGameplayScreen(void);
int FinishGameplayScreen(void);

//----------------------------------------------------------------------------------
// Declaração de funções de tela final
//----------------------------------------------------------------------------------
void InitEndingScreen(void);
void UpdateEndingScreen(void);
void DrawEndingScreen(void);
void UnloadEndingScreen(void);
int FinishEndingScreen(void);

//----------------------------------------------------------------------------------
// Declaração de funções de tela de ranking
//----------------------------------------------------------------------------------
void InitRankingScreen(void);
void UpdateRankingScreen(void);
void DrawRankingScreen(void);
void UnloadRankingScreen(void);
int FinishRankingScreen(void);

//----------------------------------------------------------------------------------
// Declaração de funções de tela de créditos
//----------------------------------------------------------------------------------
void InitCreditsScreen(void);
void UpdateCreditsScreen(void);
void DrawCreditsScreen(void);
void UnloadCreditsScreen(void);
int FinishCreditsScreen(void);

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H