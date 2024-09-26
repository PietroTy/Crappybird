#include <stdio.h>
#include <raylib.h>

// Estrutura para representar o pássaro
typedef struct Bird {
    float x;
    float y;
    float g;
    bool flaping;
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

int main(void) {
    Bird bird;
    Pipe pipe[2]; // Três canos simultâneos na tela
    int score = 0;
    int speed = 2;
    bool gameOver = false;
    int lastScore = 0; // Nova variável para rastrear a última pontuação que aumentou a velocidade

    InitWindow(600, 600, "Crappy bird");
    SetTargetFPS(60);

    ResetGame(&bird, pipe, &score, &speed, &gameOver, &lastScore);

    while (!WindowShouldClose()) {
        // Atualização do tempo e lógica do jogo
        if (gameOver) {
            if (IsKeyPressed(KEY_R)) {
                ResetGame(&bird, pipe, &score, &speed, &gameOver, &lastScore);
            }
            BeginDrawing();
            ClearBackground(BLACK);
            const char *lostMessage = "You Lost!";
            const char *scoreMessage = TextFormat("Score: %d", score);
            const char *resetMessage = "press R to reset";
            DrawText(lostMessage, (GetScreenWidth() - MeasureText(lostMessage, 100)) / 2, (GetScreenHeight() - 200) / 2, 100, WHITE);
            DrawText(scoreMessage, (GetScreenWidth() - MeasureText(scoreMessage, 60)) / 2, (GetScreenHeight() + 100) / 2, 60, WHITE);
            DrawText(resetMessage, (GetScreenWidth() - MeasureText(resetMessage, 30)) / 2, (GetScreenHeight() + 300) / 2, 30, WHITE);
            EndDrawing();
            continue;
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
        if (score >= lastScore + 5) {
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
                pipe[i].isActive = false; // Mark this pipe as scored
            }
        }

        // Desenhar
        BeginDrawing();
        ClearBackground(DARKBLUE);

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

        // Desenhar canos
        for (int i = 0; i < 2; i++) {
            DrawRectangle(pipe[i].x, 0, 50, pipe[i].height, DARKGREEN);
            DrawRectangle(pipe[i].x - 5, pipe[i].height - 15, 60, 15, LIME);
            DrawRectangle(pipe[i].x, pipe[i].height + 150, 50, GetScreenHeight() - pipe[i].height - 150, DARKGREEN);
            DrawRectangle(pipe[i].x - 5, pipe[i].height + 150, 60, 15, LIME);
        }

        // Mostrar o placar
        DrawText(TextFormat("Score: %d", score), 10, 10, 30, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}