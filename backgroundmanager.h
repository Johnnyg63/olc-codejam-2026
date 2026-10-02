#pragma once
#include "olcPixelGameEngine3.h"

class BackgroundManager : public olc::PixelGameEngine {
public:
    void Initialize(olc::PixelGameEngine* engine) {
        ptrPGE = engine;
    }

    bool LoadBackground(const std::string& filePath) {
        if(ptrPGE == nullptr) return false;

        bool res = false;
        //auto imgConfig = olc::ImageConfig();
        //imgConfig.InRAM = true; // Store the background image in RAM and process them using CPU
        res = ptrPGE->CreateImageFromFile(imgBackground, filePath);
        return res;
    }

    void Update(float fElapsedTime) {
        // Update background logic here
        // TODO: Add cool code to update background elements
    }

    void Draw() {
        if(ptrPGE == nullptr) return;
        olc::vf2d scale = { 
            static_cast<float>(ptrPGE->ScreenSize().x) / imgBackground.Size().x,
            static_cast<float>(ptrPGE->ScreenSize().y) / imgBackground.Size().y
        };

        //scale = olc::vf2d(ptrPGE->ScreenSize() / imgBackground.Size());
        ptrPGE->GetDraw().Image(imgBackground, {0, 0});

    }

private:
    olc::PixelGameEngine* ptrPGE = nullptr;
    olc::Image imgBackground;
};