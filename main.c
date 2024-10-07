#include <stdio.h>
#include <raylib.h>
#include <math.h>

// Estrutura para representar o pássaro
typedef struct Bird {
    float x;
    float y;
    float g;
    bool flaping;
    Color color; // Cor do pássaro
} Bird;

// Estrutura para representar os canos
typedef struct Pipe {
    int x;
    int height;
    bool isActive;
} Pipe;

void ResetGame(Bird *bird, Pipe *pipe, int *score, int *speed, bool *gameOver, int *lastScore) {
    // Reiniciar pássaro
    bird->x = (GetScreenWidth() - 300) / 2; // Posição inicial do pássaro
    bird->y = GetScreenHeight() / 2;
    bird->g = 0;
    bird->flaping = false;

    // Reiniciar canos
    for (int i = 0; i < 2; i++) {
        pipe[i].x = 400 + i * 300; // Distância entre os canos
        pipe[i].height = GetRandomValue(100, GetScreenHeight() - 250); // Altura aleatória para os canos
        pipe[i].isActive = true;
    }

    // Reiniciar placar e estado do jogo
    *score = 0;
    *speed = 2;
    *gameOver = false;
    *lastScore = 0; // Reinicia o lastScore para evitar problemas no aumento de velocidade
}

// Função para interpolar entre duas cores
Color LerpColor(Color a, Color b, float t) {
    return (Color){
        (unsigned char)(a.r + t * (b.r - a.r)),
        (unsigned char)(a.g + t * (b.g - a.g)),
        (unsigned char)(a.b + t * (b.b - a.b)),
        255 // Alpha fixo
    };
}

// Função para salvar o *highscore* em um arquivo
void SaveHighscore(int highscore) {
    FILE *file = fopen("resources/Highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", highscore);
        fclose(file);
        printf("Highscore salvo: %d\n", highscore);
    } else {
        printf("Erro ao salvar o highscore.\n");
    }
}

// Função para carregar o *highscore* do arquivo
int LoadHighscore() {
    FILE *file = fopen("resources/Highscore.txt", "r");
    int highscore = 0;

    if (file != NULL) {
        fscanf(file, "%d", &highscore);
        fclose(file);
        printf("Highscore carregado: %d\n", highscore);
    } else {
        printf("Nenhum highscore salvo encontrado.\n");
    }

    return highscore;
}

int main(void) {
    Bird bird;
    Pipe pipe[2]; // Dois canos simultâneos na tela
    int score = 0;
    int speed = 2;
    bool gameOver = true;
    bool store = true;
    int lastScore = 0; // Nova variável para rastrear a última pontuação que aumentou a velocidade
    int highscore = LoadHighscore(); // Carrega o *highscore* ao iniciar o jogo

    InitWindow(600, 600, "CrappyBird");
    SetWindowIcon(LoadImage("resources/iconeCrappy.png"));
    SetTargetFPS(60);

    ResetGame(&bird, pipe, &score, &speed, &gameOver, &lastScore);
    
    float startTime = GetTime(); // Tempo inicial do jogo

    Color darkBlue = DARKBLUE;
    Color skyBlue = SKYBLUE;
    Color lime = LIME;
    Color darkGreen = DARKGREEN;

    while (!WindowShouldClose()) {
        // Atualização do tempo e lógica do jogo
        if (gameOver) {

            if (store) {



            } else {

                if (score > highscore) {
                    highscore = score;
                    SaveHighscore(highscore); // Salva o novo *highscore*
                }

                if (IsKeyPressed(KEY_R)) {
                    ResetGame(&bird, pipe, &score, &speed, &gameOver, &lastScore);
                }

                if (IsKeyPressed(KEY_R)) {
                    ResetGame(&bird, pipe, &score, &speed, &gameOver, &lastScore);
                }

                BeginDrawing();
                ClearBackground(BLACK);
                const char *lostMessage = "You Lost!";
                const char *scoreMessage = TextFormat("Score: %d", score);
                const char *highscoreMessage = TextFormat("Highscore: %d", highscore);
                const char *resetMessage = "press R to reset or S to store";
                DrawText(lostMessage, (GetScreenWidth() - MeasureText(lostMessage, 100)) / 2, (GetScreenHeight() - 200) / 2, 100, WHITE);
                DrawText(scoreMessage, (GetScreenWidth() - MeasureText(scoreMessage, 60)) / 2, (GetScreenHeight() + 100) / 2, 60, WHITE);
                DrawText(highscoreMessage, (GetScreenWidth() - MeasureText(highscoreMessage, 40)) / 2, (GetScreenHeight() + 260) / 2, 40, WHITE);
                DrawText(resetMessage, (GetScreenWidth() - MeasureText(resetMessage, 30)) / 2, (GetScreenHeight() + 390) / 2, 30, WHITE);
                EndDrawing();
                continue;

            }


        }

        // Gravidade no pássaro
        bird.g += 0.5f; 
        bird.y += bird.g;

        // Controle do pássaro
        if (IsKeyPressed(KEY_SPACE)) {
            bird.g = -8.0f; // Pulo
        }

        // Verifica se o pássaro está subindo ou caindo
        if (bird.g < 0) {
            bird.flaping = true;  // Pássaro está subindo
        } else {
            bird.flaping = false; // Pássaro está caindo
        }

        // Movimento dos canos
        for (int i = 0; i < 2; i++) {
            pipe[i].x -= speed;

            if (pipe[i].x < -50) {
                pipe[i].x = GetScreenWidth();
                pipe[i].height = GetRandomValue(100, GetScreenHeight() - 250); // Gerar nova altura
                pipe[i].isActive = true;
            }
        }

        // Aumenta a velocidade do jogo a cada 5 pontos
        if (score >= lastScore + 5 && speed <= 7) {
            speed += 1;   // Incrementa a velocidade em 1
            lastScore = score;  // Atualiza o último marco
        }

        // Verificar colisão com os canos ou com os limites da tela
        for (int i = 0; i < 2; i++) {
            if ((bird.x + 40 > pipe[i].x && bird.x < pipe[i].x + 50) &&
                (bird.y < pipe[i].height || bird.y + 40 > pipe[i].height + 150)) {
                gameOver = true; // Colisão com o cano
            }
        }

        if (bird.y > GetScreenHeight() || bird.y < 0) gameOver = true; // Colisão com o teto ou o chão

        for (int i = 0; i < 2; i++) {
            if (pipe[i].isActive && bird.x > pipe[i].x + 50) {
                score++;
                pipe[i].isActive = false; // Marca o cano como pontuado
            }
        }

        // Alternar gradualmente entre SKYBLUE e DARKBLUE
        float elapsedTime = GetTime() - startTime;
        float cycleTime = 40.0f; // Tempo total para ciclo completo (20s clareando e 20s escurecendo)
        float t = fmod(elapsedTime, cycleTime) / cycleTime; // Valor de interpolação entre 0 e 1

        Color backgroundColor;
        if (t < 0.5f) {
            // Transição de SKYBLUE para DARKBLUE
            backgroundColor = LerpColor(skyBlue, darkBlue, t * 2);
        } else {
            // Transição de DARKBLUE para SKYBLUE
            backgroundColor = LerpColor(darkBlue, skyBlue, (t - 0.5f) * 2);
        }

        Color pipeColor;
        if (t < 0.5f) {
            pipeColor = LerpColor(lime, darkGreen, t * 2);
        } else {
            pipeColor = LerpColor(darkGreen, lime, (t - 0.5f) * 2);
        }

        Color pipeBorderColor;
        if (t < 0.5f) {
            pipeBorderColor = LerpColor(darkGreen, lime, t * 2);
        } else {
            pipeBorderColor = LerpColor(lime, darkGreen, (t - 0.5f) * 2);
        }

        // Desenhar
        BeginDrawing();
        ClearBackground(backgroundColor); // Alternar entre fundo claro e escuro

        // Desenhar pássaro
        DrawRectangle(bird.x, bird.y, 40, 40, YELLOW);
        DrawRectangle(bird.x + 10, bird.y + 10, 20, 20, WHITE);
        DrawRectangle(bird.x + 20, bird.y + 10, 10, 10, BLACK);
        DrawRectangle(bird.x + 30, bird.y + 20, 20, 10, ORANGE);
        if (bird.flaping) {
            DrawRectangle(bird.x - 15, bird.y + 20, 30, 20, ORANGE);
        } else {
            DrawRectangle(bird.x - 15, bird.y + 10, 30, 20, ORANGE);
        }

        // Desenhar canos com a cor alternada junto ao céu
        for (int i = 0; i < 2; i++) {
            DrawRectangle(pipe[i].x, 0, 50, pipe[i].height, pipeColor);
            DrawRectangle(pipe[i].x - 5, pipe[i].height - 15, 60, 15, pipeBorderColor);
            DrawRectangle(pipe[i].x, pipe[i].height + 150, 50, GetScreenHeight() - pipe[i].height - 150, pipeBorderColor);
            DrawRectangle(pipe[i].x - 5, pipe[i].height + 150, 60, 15, pipeColor);
        }

        // Mostrar o placar
        DrawText(TextFormat("Score: %d", score), 10, 10, 30, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
