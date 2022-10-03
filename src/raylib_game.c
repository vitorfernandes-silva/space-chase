/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "screens.h"  // NOTA: Declara variáveis ​​globais (externas) e funções de tela

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Definição de variáveis ​​compartilhadas (global)
// NOTA: Essas variáveis ​​são compartilhadas entre os módulos através screens.h
//----------------------------------------------------------------------------------
GameScreen currentScreen = 0;
Font font = { 0 };
Music music = { 0 };
Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Definição de Variáveis ​​Locais (local para este módulo)
//----------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

// Variáveis ​​necessárias para gerenciar transições de tela (fade-in, fade-out)
static float transAlpha = 0.0f;
static bool onTransition = false;
static bool transFadeOut = false;
static int transFromScreen = -1;
static int transToScreen = -1;

//----------------------------------------------------------------------------------
// Declaração de Funções Locais
//----------------------------------------------------------------------------------
static void ChangeToScreen(int screen);     // Mude para a tela, sem efeito de transição

static void TransitionToScreen(int screen); // Solicitar transição para a próxima tela
static void UpdateTransition(void);         // Atualizar efeito de transição
static void DrawTransition(void);           // Desenhar efeito de transição (retângulo de tela inteira)

static void UpdateDrawFrame(void);          // Atualizar e desenhar um quadro

//----------------------------------------------------------------------------------
// Ponto de entrada principal
//----------------------------------------------------------------------------------
int main(void)
{
    // Inicialização
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "raylib game template");

    InitAudioDevice();      // Inicializar dispositivo de áudio

    // Carregar dados globais (que devem estar disponíveis em todas as telas, ou seja, fonte)
    font = LoadFont("resources/mecha.png");
    music = LoadMusicStream("resources/jupiterbyd3nomite2.wav");
    fxCoin = LoadSound("resources/coin.wav");

    SetMusicVolume(music, 1.0f);
    PlayMusicStream(music);

    // Configura e inicia a primeira tela
    currentScreen = LOGO;
    InitLogoScreen();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60); // Configura nosso jogo para rodar a 60 frames por segundo
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detecta o botão de fechamento da janela ou a tecla ESC
    {
        UpdateDrawFrame();
    }
#endif

    // Desinicialização
    //--------------------------------------------------------------------------------------
    // Descarregue os dados da tela atual antes de fechar
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case DIALOG: UnloadDialogScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        case RANKING: UnloadRankingScreen(); break;
        case CREDITS: UnloadCreditsScreen(); break;
        default: break;
    }

    // Descarregar dados globais carregados
    UnloadFont(font);
    UnloadMusicStream(music);
    UnloadSound(fxCoin);

    CloseAudioDevice(); // Fecha o contexto de áudio

    CloseWindow(); // Fechar janela e contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Definição de funções específicas do módulo
//----------------------------------------------------------------------------------
// Mude para a próxima tela, sem transição
static void ChangeToScreen(int screen)
{
    // Descarregue a tela atual
    switch (currentScreen)
    {
        case LOGO: UnloadLogoScreen(); break;
        case DIALOG: UnloadDialogScreen(); break;
        case TITLE: UnloadTitleScreen(); break;
        case OPTIONS: UnloadOptionsScreen(); break;
        case GAMEPLAY: UnloadGameplayScreen(); break;
        case ENDING: UnloadEndingScreen(); break;
        case RANKING: UnloadRankingScreen(); break;
        case CREDITS: UnloadCreditsScreen(); break;

        default: break;
    }

    // Iniciar próxima tela
    switch (screen)
    {
        case LOGO: InitLogoScreen(); break;
        case DIALOG: InitDialogScreen(); break;
        case TITLE: InitTitleScreen(); break;
        case OPTIONS: InitOptionsScreen(); break;
        case GAMEPLAY: InitGameplayScreen(); break;
        case ENDING: InitEndingScreen(); break;
        case RANKING: InitRankingScreen(); break;
        case CREDITS: InitCreditsScreen(); break;

        default: break;
    }

    currentScreen = screen;
}

// Solicitar transição para a próxima tela
static void TransitionToScreen(int screen)
{
    onTransition = true;
    transFadeOut = false;
    transFromScreen = currentScreen;
    transToScreen = screen;
    transAlpha = 0.0f;
}

// Atualizar efeito de transição (fade-in, fade-out)
static void UpdateTransition(void)
{
    if (!transFadeOut)
    {
        transAlpha += 0.05f;

        // NOTA: Devido à representação interna flutuante, a condição salta em 1.0f em vez de 1.05f
        // Por esse motivo, comparamos com 1.01f, para evitar a última parada de carregamento do quadro

        if (transAlpha > 1.01f)
        {
            transAlpha = 1.0f;

            // Descarregue a tela atual
            switch (transFromScreen)
            {
                case LOGO: UnloadLogoScreen(); break;
                case DIALOG: UnloadDialogScreen(); break;
                case TITLE: UnloadTitleScreen(); break;
                case OPTIONS: UnloadOptionsScreen(); break;
                case GAMEPLAY: UnloadGameplayScreen(); break;
                case ENDING: UnloadEndingScreen(); break;
                case RANKING: UnloadRankingScreen(); break;
                case CREDITS: UnloadCreditsScreen(); break;
                default: break;
            }

            // Carregar próxima tela
            switch (transToScreen)
            {
                case LOGO: InitLogoScreen(); break;
                case DIALOG: InitDialogScreen(); break;
                case TITLE: InitTitleScreen(); break;
                case OPTIONS: InitOptionsScreen(); break;
                case GAMEPLAY: InitGameplayScreen(); break;
                case ENDING: InitEndingScreen(); break;
                case RANKING: InitRankingScreen(); break;
                case CREDITS: InitCreditsScreen(); break;

                default: break;
            }

            currentScreen = transToScreen;

            // Ative o efeito fade out para a próxima tela carregada
            transFadeOut = true;
        }
    }
    else  // Lógica de transição gradual
    {
        transAlpha -= 0.02f;

        if (transAlpha < -0.01f)
        {
            transAlpha = 0.0f;
            transFadeOut = false;
            onTransition = false;
            transFromScreen = -1;
            transToScreen = -1;
        }
    }
}

// Desenhar efeito de transição (retângulo de tela inteira)
static void DrawTransition(void)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
}

// Atualize e desenhe o quadro do jogo
static void UpdateDrawFrame(void)
{
    // Atualizar
    //----------------------------------------------------------------------------------
    UpdateMusicStream(music); // NOTA: A música continua tocando entre as telas

    if (!onTransition)
    {
        switch(currentScreen)
        {
            case LOGO:
            {
                UpdateLogoScreen();

                if (FinishLogoScreen()) TransitionToScreen(DIALOG);

            } break;
            case DIALOG:
            {
                UpdateDialogScreen();

                if (FinishDialogScreen()) TransitionToScreen(TITLE);

            } break;
            case TITLE:
            {
                UpdateTitleScreen();

                if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
                        else if (FinishTitleScreen() == 4) TransitionToScreen(RANKING);

            } break;
            case OPTIONS:
            {
                UpdateOptionsScreen();

                if (FinishOptionsScreen() == 1) TransitionToScreen(GAMEPLAY);

            } break;
            case GAMEPLAY:
            {
                UpdateGameplayScreen();

                if (FinishGameplayScreen()) TransitionToScreen(ENDING);
                //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

            } break;
            case ENDING:
            {
                UpdateEndingScreen();

                if (FinishEndingScreen()) TransitionToScreen(RANKING);

            } break;
            case RANKING:
            {
                UpdateRankingScreen();
                
                if(FinishRankingScreen()) TransitionToScreen(CREDITS);
            } break;

            case CREDITS:
            {
                UpdateCreditsScreen();

                if(FinishCreditsScreen()) TransitionToScreen(TITLE);
            }
            default: break;
        }
    }
    else UpdateTransition(); // Transição de atualização (fade-in, fade-out)
    //----------------------------------------------------------------------------------

    // Desenhar
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        switch(currentScreen)
        {
            case LOGO: DrawLogoScreen(); break;
            case DIALOG: DrawDialogScreen(); break;
            case TITLE: DrawTitleScreen(); break;
            case OPTIONS: DrawOptionsScreen(); break;
            case GAMEPLAY: DrawGameplayScreen(); break;
            case ENDING: DrawEndingScreen(); break;
            case RANKING: DrawRankingScreen(); break;
            case CREDITS: DrawCreditsScreen(); break;
            default: break;
        }

        // Desenhe um retângulo de tela cheia na frente de tudo
        if (onTransition) DrawTransition();

        // DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}
