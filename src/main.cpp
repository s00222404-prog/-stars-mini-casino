#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

namespace {
constexpr int START_W = 1280;
constexpr int START_H = 800;

const Color BG_TOP{10, 13, 38, 255};
const Color BG_BOTTOM{24, 10, 55, 255};
const Color CYAN{69, 235, 255, 255};
const Color PURPLE{158, 91, 255, 255};
const Color PINK{255, 76, 177, 255};
const Color GOLD{255, 204, 79, 255};
const Color MUTED{167, 180, 223, 255};

float Clamp01(float value) { return std::max(0.0f, std::min(1.0f, value)); }
float Smooth(float value) {
    value = Clamp01(value);
    return value * value * (3.0f - 2.0f * value);
}
bool Hit(Rectangle r, Vector2 p) { return CheckCollisionPointRec(p, r); }

void CenterText(const char* text, float x, float y, int size, Color color) {
    DrawText(text, static_cast<int>(x - MeasureText(text, size) / 2.0f),
             static_cast<int>(y), size, color);
}

void Glow(Vector2 p, float radius, Color color, float strength = 1.0f) {
    for (int i = 8; i >= 1; --i) {
        Color c = color;
        c.a = static_cast<unsigned char>(std::clamp(strength * (15 - i) * 2.0f, 0.0f, 90.0f));
        DrawCircleV(p, radius * (0.35f + i * 0.15f), c);
    }
}

void Glass(Rectangle r, Color accent, float opacity = 26.0f) {
    DrawRectangleRounded(r, 0.13f, 18,
        {255, 255, 255, static_cast<unsigned char>(opacity)});
    DrawRectangleRoundedLinesEx(r, 0.13f, 18, 1.3f, {255, 255, 255, 48});
    DrawRectangleRounded({r.x + 13, r.y + 1, r.width - 26, 2}, 0.8f, 6,
        {accent.r, accent.g, accent.b, 135});
    DrawRectangleRoundedLinesEx({r.x + 1, r.y + 1, r.width - 2, r.height - 2},
        0.13f, 18, 1.0f, {accent.r, accent.g, accent.b, 34});
}

void Button(Rectangle r, const char* label, Color accent, bool hover, float time) {
    DrawRectangleRounded(r, 0.32f, 12,
        {accent.r, accent.g, accent.b, static_cast<unsigned char>(hover ? 83 : 43)});
    DrawRectangleRoundedLinesEx(r, 0.32f, 12, 1.5f,
        {accent.r, accent.g, accent.b, static_cast<unsigned char>(hover ? 240 : 160)});
    if (hover) Glow({r.x + r.width / 2, r.y + r.height / 2},
                    8 + std::sin(time * 4.0f) * 2.0f, accent, 0.45f);
    CenterText(label, r.x + r.width / 2, r.y + r.height / 2 - 9, 17, RAYWHITE);
}

void Basketball(Vector2 p, float radius) {
    DrawCircleV(p, radius, {255, 151, 65, 255});
    DrawCircleLines(static_cast<int>(p.x), static_cast<int>(p.y), radius, {55, 29, 45, 255});
    DrawLineEx({p.x - radius * .88f, p.y}, {p.x + radius * .88f, p.y}, 2, {55, 29, 45, 255});
    DrawLineEx({p.x, p.y - radius * .9f}, {p.x, p.y + radius * .9f}, 2, {55, 29, 45, 255});
    DrawLineEx({p.x - radius * .63f, p.y - radius * .7f},
               {p.x + radius * .63f, p.y + radius * .7f}, 1.5f, {55, 29, 45, 220});
}

void Bowling(Vector2 p, float size) {
    DrawCircleV({p.x - size * .32f, p.y - size * .16f}, size * .27f, {96, 225, 255, 255});
    DrawCircleV({p.x, p.y - size * .31f}, size * .30f, {174, 123, 255, 255});
    DrawCircleV({p.x + size * .32f, p.y - size * .16f}, size * .27f, {255, 116, 193, 255});
    DrawCircleV({p.x, p.y + size * .27f}, size * .31f, RAYWHITE);
    DrawCircleV({p.x - size * .07f, p.y + size * .23f}, size * .045f, {70, 75, 130, 255});
    DrawCircleV({p.x + size * .08f, p.y + size * .31f}, size * .045f, {70, 75, 130, 255});
}

void Dice(Vector2 p, float size) {
    DrawRectangleRounded({p.x - size / 2, p.y - size / 2, size, size}, .2f, 10, RAYWHITE);
    const Vector2 dots[] = {{-.27f,-.27f},{.27f,-.27f},{0,0},{-.27f,.27f},{.27f,.27f}};
    for (Vector2 d : dots)
        DrawCircleV({p.x + d.x * size, p.y + d.y * size}, size * .065f, {76, 51, 143, 255});
}

void Bear(Vector2 p, float size) {
    const Color fur{255, 194, 119, 255};
    DrawCircleV({p.x - size * .34f, p.y - size * .30f}, size * .22f, fur);
    DrawCircleV({p.x + size * .34f, p.y - size * .30f}, size * .22f, fur);
    DrawCircleV(p, size * .43f, fur);
    DrawCircleV({p.x - size * .14f, p.y - size * .05f}, size * .04f, {45, 38, 57, 255});
    DrawCircleV({p.x + size * .14f, p.y - size * .05f}, size * .04f, {45, 38, 57, 255});
    DrawEllipse(static_cast<int>(p.x), static_cast<int>(p.y + size * .12f),
                size * .15f, size * .10f, {255, 226, 190, 255});
    DrawCircleV({p.x, p.y + size * .08f}, size * .04f, {73, 48, 53, 255});
}

void Background(int w, int h, float t) {
    DrawRectangleGradientEx({0, 0, static_cast<float>(w), static_cast<float>(h)},
        BG_TOP, BG_TOP, BG_BOTTOM, {7, 15, 42, 255});
    Glow({w * .12f + std::sin(t * .35f) * 25, h * .18f}, 105, PURPLE, .48f);
    Glow({w * .86f + std::cos(t * .28f) * 20, h * .26f}, 120, PINK, .38f);
    Glow({w * .57f, h * .95f}, 100, CYAN, .25f);
    for (int i = 0; i < 42; ++i) {
        const float x = std::fmod(i * 173.0f + std::sin(t * .25f + i) * 26.0f, static_cast<float>(w));
        const float y = std::fmod(i * 97.0f + std::cos(t * .21f + i * 2) * 21.0f, static_cast<float>(h));
        const auto alpha = static_cast<unsigned char>(20 + 27 * (.5f + .5f * std::sin(t * 1.4f + i)));
        DrawCircleV({x, y}, 1.0f + (i % 3) * .4f, {185, 211, 255, alpha});
    }
}

struct GameCard {
    Rectangle bounds;
    const char* title;
    const char* subtitle;
    const char* detail;
    Color accent;
    int type;
};
} // namespace

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(START_W, START_H, "STAR PLAY | Mini Games");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    int points = 250;
    int selected = 0;
    bool modalOpen = false;
    bool bonusClaimed = false;
    float modalProgress = 0.0f;
    float toastSeconds = 0.0f;
    std::string toast;
    std::string resultTitle = "NICE TRY!";
    std::string resultMessage = "Round complete.";
    Color resultColor = CYAN;

    while (!WindowShouldClose()) {
        const float dt = GetFrameTime();
        const float time = static_cast<float>(GetTime());
        const int w = GetScreenWidth();
        const int h = GetScreenHeight();
        const Vector2 mouse = GetMousePosition();
        const bool click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        const float margin = std::max(18.0f, w * .06f);
        const float top = h * .20f;
        const float panelW = w - margin * 2;
        const float panelH = h * .61f;
        const float gap = 16.0f;
        const float cardW = (panelW - 44 - gap * 2) / 3;
        const float cardY = top + 91;
        const float cardH = panelH - 115;
        std::vector<GameCard> cards{
            {{margin + 22, cardY, cardW, cardH}, "HOOP HERO", "Basketball challenge", "LAND THE SHOT", CYAN, 0},
            {{margin + 22 + cardW + gap, cardY, cardW, cardH}, "STRIKE ZONE", "Bowling challenge", "HIT THE PINS", PINK, 1},
            {{margin + 22 + (cardW + gap) * 2, cardY, cardW, cardH}, "LUCKY DICE", "Even or odd", "ROLL THE DICE", GOLD, 2}
        };

        if (IsKeyPressed(KEY_ESCAPE)) {
            if (modalOpen) modalOpen = false;
            else break;
        }
        if (IsKeyPressed(KEY_R)) {
            points = 250;
            toast = "DEMO SCORE RESET";
            toastSeconds = 2.0f;
        }
        toastSeconds = std::max(0.0f, toastSeconds - dt);
        modalProgress += ((modalOpen ? 1.0f : 0.0f) - modalProgress) * std::min(1.0f, dt * 9.0f);

        BeginDrawing();
        Background(w, h, time);

        DrawText("STAR PLAY", static_cast<int>(margin), static_cast<int>(h * .045f), 31, RAYWHITE);
        DrawCircleV({margin + 5, h * .045f + 47}, 3, CYAN);
        DrawText("MINI GAMES  /  DEMO ARENA", static_cast<int>(margin + 4),
                 static_cast<int>(h * .045f + 40), 11, MUTED);

        Rectangle scoreCard{w - margin - 225, h * .045f, 225, 58};
        Glass(scoreCard, PURPLE, 30);
        DrawText("DEMO POINTS", static_cast<int>(scoreCard.x + 16), static_cast<int>(scoreCard.y + 9), 10, MUTED);
        DrawText(TextFormat("%d", points), static_cast<int>(scoreCard.x + 16), static_cast<int>(scoreCard.y + 24), 24, RAYWHITE);
        DrawCircleV({scoreCard.x + scoreCard.width - 24, scoreCard.y + 29}, 9, GOLD);
        DrawPoly({scoreCard.x + scoreCard.width - 24, scoreCard.y + 29}, 5, 4, 0, RAYWHITE);

        DrawText("CHOOSE YOUR GAME", static_cast<int>(margin), static_cast<int>(h * .145f), 20, RAYWHITE);
        DrawText("Three quick challenges. Play for fun, collect demo points.",
                 static_cast<int>(margin), static_cast<int>(h * .145f + 28), 13, MUTED);

        Rectangle panel{margin, top, panelW, panelH};
        Glass(panel, PURPLE, 17);
        DrawText("THE ARENA", static_cast<int>(panel.x + 22), static_cast<int>(panel.y + 20), 11, MUTED);
        DrawText("Pick a challenge", static_cast<int>(panel.x + 22), static_cast<int>(panel.y + 38), 20, RAYWHITE);

        for (int i = 0; i < static_cast<int>(cards.size()); ++i) {
            const GameCard& c = cards[i];
            const bool hover = Hit(c.bounds, mouse);
            Rectangle r = c.bounds;
            r.y += (hover ? -4.0f : 0.0f) + (selected == i ? -1.5f : 0.0f);
            Glass(r, c.accent, selected == i ? 38 : 22);
            if (selected == i)
                DrawRectangleRoundedLinesEx({r.x - 2, r.y - 2, r.width + 4, r.height + 4},
                    .14f, 18, 2.0f, {c.accent.r, c.accent.g, c.accent.b, 220});
            Glow({r.x + r.width / 2, r.y + 72}, 23, c.accent, hover ? .42f : .20f);
            const Vector2 iconPos{r.x + r.width / 2, r.y + 72};
            if (i == 0) Basketball(iconPos, 27);
            if (i == 1) Bowling(iconPos, 48);
            if (i == 2) Dice(iconPos, 47);
            CenterText(c.title, r.x + r.width / 2, r.y + 120, 16, RAYWHITE);
            CenterText(c.subtitle, r.x + r.width / 2, r.y + 145, 11, MUTED);
            DrawLineEx({r.x + 20, r.y + 169}, {r.x + r.width - 20, r.y + 169}, 1, {255, 255, 255, 28});
            CenterText(c.detail, r.x + r.width / 2, r.y + 187, 11, c.accent);
            CenterText(i == 2 ? "EVEN / ODD" : "QUICK CHALLENGE",
                       r.x + r.width / 2, r.y + 209, 9, MUTED);
            if (hover && click) selected = i;
        }

        Rectangle playButton{w * .35f, h * .835f, w * .30f, 52};
        const Color selectedColor = selected == 0 ? CYAN : (selected == 1 ? PINK : GOLD);
        Button(playButton, "PLAY ROUND  ->", selectedColor, Hit(playButton, mouse), time);

        Rectangle bonusButton{margin, h * .835f, std::min(260.0f, w * .23f), 52};
        Button(bonusButton, bonusClaimed ? "BONUS CLAIMED" : "DAILY BONUS  +25",
               PURPLE, Hit(bonusButton, mouse), time);

        DrawText("DEMO MODE  •  NO PAYMENTS OR CASH PRIZES",
                 static_cast<int>(margin), static_cast<int>(h * .92f), 10, {131, 144, 190, 255});
        DrawText("R = reset points", static_cast<int>(w * .80f),
                 static_cast<int>(h * .92f), 10, {131, 144, 190, 255});

        if (click && Hit(bonusButton, mouse) && !bonusClaimed) {
            points += 25;
            bonusClaimed = true;
            toast = "+25 DEMO POINTS ADDED";
            toastSeconds = 2.0f;
        }

        if (click && Hit(playButton, mouse) && !modalOpen) {
            const int roll = GetRandomValue(1, 100);
            if (selected == 0) {
                if (roll <= 55) {
                    points += 20; resultTitle = "PERFECT SHOT!";
                    resultMessage = "+20 demo points. Clean basket!";
                    resultColor = CYAN;
                } else {
                    points = std::max(0, points - 5); resultTitle = "ALMOST!";
                    resultMessage = "-5 demo points. Try another shot.";
                    resultColor = PINK;
                }
            } else if (selected == 1) {
                if (roll <= 48) {
                    points += 20; resultTitle = "STRIKE!";
                    resultMessage = "+20 demo points. All pins down!";
                    resultColor = PINK;
                } else {
                    points = std::max(0, points - 5); resultTitle = "TRY AGAIN";
                    resultMessage = "-5 demo points. Some pins stayed up.";
                    resultColor = PURPLE;
                }
            } else {
                const int die = GetRandomValue(1, 6);
                if (die % 2 == 0) {
                    points += 15; resultTitle = "EVEN! YOU WIN";
                    resultMessage = TextFormat("You rolled %d. +15 demo points!", die);
                    resultColor = GOLD;
                } else {
                    points = std::max(0, points - 5); resultTitle = "ODD ROLL";
                    resultMessage = TextFormat("You rolled %d. -5 demo points.", die);
                    resultColor = PINK;
                }
            }
            modalOpen = true;
            modalProgress = 0.0f;
        }

        if (modalProgress > .005f) {
            const float a = Smooth(modalProgress);
            DrawRectangle(0, 0, w, h, {3, 4, 18, static_cast<unsigned char>(145 * a)});
            const float scale = .92f + .08f * a;
            const float mw = std::min(470.0f, w * .84f) * scale;
            const float mh = 300.0f * scale;
            Rectangle modal{w / 2.0f - mw / 2.0f, h / 2.0f - mh / 2.0f, mw, mh};
            Glow({w / 2.0f, h / 2.0f}, 75, resultColor, .27f * a);
            Glass(modal, resultColor, 40);
            CenterText("ROUND RESULT", w / 2.0f, modal.y + 22, 11, MUTED);
            CenterText(resultTitle.c_str(), w / 2.0f, modal.y + 49, 24, RAYWHITE);
            CenterText(resultMessage.c_str(), w / 2.0f, modal.y + 86, 13, {205, 213, 243, 255});
            Bear({w / 2.0f, modal.y + 151}, 35);
            CenterText("COSMETIC BEAR BADGE", w / 2.0f, modal.y + 190, 9, {255, 207, 142, 255});
            Rectangle continueButton{modal.x + modal.width * .18f, modal.y + modal.height - 49,
                                     modal.width * .64f, 35};
            Button(continueButton, "CONTINUE", resultColor, Hit(continueButton, mouse), time);
            if (modalOpen && click && Hit(continueButton, mouse)) modalOpen = false;
        }

        if (toastSeconds > 0) {
            Rectangle toastBox{w / 2.0f - 155, h * .11f, 310, 42};
            Glass(toastBox, CYAN, 30);
            CenterText(toast.c_str(), w / 2.0f, toastBox.y + 13, 12, RAYWHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
