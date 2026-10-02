#pragma once
#include "olcPixelGameEngine3.h"
#include <vector>

enum class PhaseState { 
    BLUE_ACTIVE, 
    RED_ACTIVE,
    GREEN_ACTIVE,
    BLANK_ACTIVE 
};

struct Rectangle {
    // 	draw.Rect({10.0f, 10.0f}, {10.0f, 10.0f}, olc::Colour::WHITE);
    olc::vf2d position = {0.0f, 0.0f};
    olc::vf2d size     = {1.0f, 1.0f};
    olc::Pixel colour  = olc::Colour::WHITE;
};

struct Platform {
    Rectangle bounds;
    PhaseState matchingState;
};

class LevelManager_hold : public olc::PixelGameEngine {
    
public:
    PhaseState currentPhase = PhaseState::BLUE_ACTIVE;
    float phaseTimer = 0.0f;
    const float PHASE_DURATION = 4.0f; // Switch states every 4 seconds
    std::vector<Platform> platforms;
    olc::PixelGameEngine* ptrPGE = nullptr;

    void Initialize(olc::PixelGameEngine* engine) {
        // Store the pointer to the PixelGameEngine instance for later use
        ptrPGE = engine;
        // Floor / Ground
        platforms.clear(); // Clear any existing platforms before initializing new ones
        // temp code to get us up and running quickly

        platforms.push_back({ { { 0.0f, (float)ptrPGE->GetScreen().Size().y - 30.0f }, { (float)ptrPGE->GetScreen().Size().x, 30.0f }, olc::Colour::BLUE }, PhaseState::BLUE_ACTIVE });
        platforms.push_back({ { { 0.0f, (float)ptrPGE->GetScreen().Size().y - 30.0f }, { (float)ptrPGE->GetScreen().Size().x, 30.0f }, olc::Colour::RED }, PhaseState::RED_ACTIVE });
        
        // Alternating level layout platforms
        platforms.push_back({ { { 150.0f, 300.0f }, { 120.0f, 20.0f }, olc::Colour::BLUE }, PhaseState::BLUE_ACTIVE });
        platforms.push_back({ { { 320.0f, 230.0f }, { 120.0f, 20.0f }, olc::Colour::RED }, PhaseState::RED_ACTIVE });
        platforms.push_back({ { { 500.0f, 160.0f }, { 120.0f, 20.0f }, olc::Colour::BLUE }, PhaseState::BLUE_ACTIVE });
        
        // Hazard platform (can only pass through safely when it matches phase)
        platforms.push_back({ { { 320.0f, 330.0f }, { 120.0f, 20.0f }, olc::Colour::RED }, PhaseState::RED_ACTIVE });
    }

    void Update(float fElapsedTime) {
        phaseTimer += fElapsedTime;
        if (phaseTimer >= PHASE_DURATION) {
            phaseTimer = 0.0f;
            // Alternate the state
            currentPhase = (currentPhase == PhaseState::BLUE_ACTIVE) 
                           ? PhaseState::RED_ACTIVE 
                           : PhaseState::BLUE_ACTIVE;
        }
    }

    void Draw() {
        if (!ptrPGE) return;
        for (const auto& platform : platforms) {
            bool isActive = (platform.matchingState == currentPhase);
            
            if (platform.matchingState == PhaseState::BLUE_ACTIVE) {
                if (isActive) {
                    DrawRectangle(platform, olc::Colour::DARK_BLUE, true);
                    DrawRectangle(platform, olc::Colour::BLUE, false);
            
                } else {
                    // Phantom state (transparent ghost)
                    DrawRectangle(platform, olc::Colour::BLUE, false);
                }
            } else {
                if (isActive) {
                    DrawRectangle(platform, olc::Colour::DARK_RED, true);
                    DrawRectangle(platform, olc::Colour::RED, false);
            
                } else {
                    // Phantom state (transparent ghost)
                    DrawRectangle(platform, olc::Colour::RED, false);
                }
            }
        }

        // Draw Timer Bar UI at the top
        float progressWidth = ptrPGE->GetScreen().Size().x * (1.0f - (phaseTimer / PHASE_DURATION));
        olc::Pixel barColor = (currentPhase == PhaseState::BLUE_ACTIVE) ? olc::Colour::BLUE : olc::Colour::RED;
        ptrPGE->GetDraw().FilledRoundedRect({10.0f,10.0f}, {progressWidth, 10.0f}, 5.0f, barColor);
    }

private:
    void DrawRectangle(const Platform& platform, const olc::Pixel col, const bool isFillRec = true) {

        const olc::Pixel tint = olc::Colour::WHITE;
        const int32_t faces = 32;

        if(isFillRec)
        {
            //ptrPGE->GetDraw().FilledRect(platform.bounds.position, platform.bounds.size, col);
            ptrPGE->GetDraw().FilledRoundedRect(platform.bounds.position, platform.bounds.size, platform.bounds.size.y * 0.5f, col, tint, faces);
        }
        else
        {
            //ptrPGE->GetDraw().Rect(platform.bounds.position, platform.bounds.size, col);
            ptrPGE->GetDraw().RoundedRect(platform.bounds.position, platform.bounds.size, platform.bounds.size.y * 0.5f, col, tint, faces);
        }
    }

};