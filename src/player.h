#pragma once
#include "olcPixelGameEngine3.h"
#include "levelmanager.h"

class Player {
public:
    

private:

    olc::PixelGameEngine* ptrPGE = nullptr;
    olc::vf2d position = { 50, 300 };
    olc::vf2d velocity = { 0, 0 };
    const float SPEED = 300.0f;
    const float GRAVITY = 1200.0f;
    const float JUMP_FORCE = -500.0f;
    const float WIDTH = 24.0f;
    const float HEIGHT = 36.0f;
    bool isGrounded = false;

public:
    void Initialize(olc::PixelGameEngine* engine) {
        // Store the pointer to the PixelGameEngine instance for later use
        ptrPGE = engine;

     }

    void Update(float fElapsedTime, const LevelManager& level) {
        // Horizontal Movement Input
        if(ptrPGE == nullptr) return;
        velocity.x = 0;
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::A).bHeld || ptrPGE->GetKeyboard().GetKey(olc::Key::LEFT).bHeld)  velocity.x = -SPEED;
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::D).bHeld || ptrPGE->GetKeyboard().GetKey(olc::Key::RIGHT).bHeld) velocity.x = SPEED;

        // Jump Input
        if ((ptrPGE->GetKeyboard().GetKey(olc::Key::SPACE).bPressed || ptrPGE->GetKeyboard().GetKey(olc::Key::W).bPressed) && isGrounded) {
            velocity.y = JUMP_FORCE;
            isGrounded = false;
        }

        // Apply Gravity
        velocity.y += GRAVITY * fElapsedTime;

        // Move X and resolve X collisions
        position.x += velocity.x * fElapsedTime;
        ResolveCollisions(level, true);

        // Move Y and resolve Y collisions
        position.y += velocity.y * fElapsedTime;
        isGrounded = false; // Reset before checking
        ResolveCollisions(level, false);

        // Keep player within screen bounds (optional)
        if (position.x < 0) position.x = 0;
        if (position.x + WIDTH > ptrPGE->GetScreen().Size().x) position.x = ptrPGE->GetScreen().Size().x - WIDTH;
        if (position.y < JUMP_FORCE) position.y = JUMP_FORCE;
        if (position.y + HEIGHT > ptrPGE->GetScreen().Size().y) 
        {
            position.y = ptrPGE->GetScreen().Size().y - HEIGHT;
            velocity.y = 0; // Stop downward velocity when hitting the ground
            isGrounded = true; // Player is on the ground
        }
    }

    void Draw(PhaseState currentPhase) {
        Rectangle body = { {position.x, position.y}, {WIDTH, HEIGHT} };
        olc::Pixel playerColor = (currentPhase == PhaseState::BLUE_ACTIVE) ? olc::Colour::DARK_BLUE : olc::Colour::DARK_RED;
        olc::Pixel outlineColor = (currentPhase == PhaseState::BLUE_ACTIVE) ? olc::Colour::BLUE : olc::Colour::RED;
        
        DrawRectangle(body, playerColor, true);
        DrawRectangle(body, outlineColor, false);
    }

private:

    void DrawRectangle(const Rectangle& rect, const olc::Pixel col, const bool isFillRec = true) {
        if(ptrPGE == nullptr) return;
        if(isFillRec)
        {
            //ptrPGE->GetDraw().FilledRect(rect.position, rect.size, col);
            ptrPGE->GetDraw().FilledRoundedRect(rect.position, rect.size, rect.size.y * 0.5f, col);
        }
        else
        {
            //ptrPGE->GetDraw().Rect(platform.bounds.position, platform.bounds.size, col);  
            ptrPGE->GetDraw().RoundedRect(rect.position, rect.size, rect.size.y * 0.5f, col);
        }
    }

    void ResolveCollisions(const LevelManager& level, bool checkingX) {
        Rectangle playerBox = { {position.x, position.y}, {WIDTH, HEIGHT} };

        for (const auto& platform : level.platforms) {
            // CRITICAL MECHANICAL HOOK: Skip if platform is phase-shifted out
            if (platform.matchingState != level.currentPhase) {
                continue; 
            }

            if (CheckCollisionRecs(playerBox, platform.bounds)) {
                if (checkingX) {
                    // Moving right, hit left edge of block
                    if (velocity.x > 0) position.x = platform.bounds.position.x - WIDTH;
                    // Moving left, hit right edge of block
                    if (velocity.x < 0) position.x = platform.bounds.position.x + platform.bounds.size.x;
                    velocity.x = 0;
                } else {
                    // Falling down, hit top edge of block
                    if (velocity.y > 0) {
                        position.y = platform.bounds.position.y - HEIGHT;
                        velocity.y = 0;
                        isGrounded = true;
                    }
                    // Jumping up, hit bottom edge of block
                    if (velocity.y < 0) {
                        position.y = platform.bounds.position.y + platform.bounds.size.y;
                        velocity.y = 0;
                    }
                }
                // Refresh player box position for subsequent platform checks
                playerBox = { {position.x, position.y}, {WIDTH, HEIGHT} };
            }
        }
    }

    bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2)
    {
        bool collision = false;

        if ((rec1.position.x < (rec2.position.x + rec2.size.x)) && ((rec1.position.x + rec1.size.x) > rec2.position.x) &&
            (rec1.position.y < (rec2.position.y + rec2.size.y)) && ((rec1.position.y + rec1.size.y) > rec2.position.y)) 
        {
            collision = true;
        }

        return collision;
    }

};