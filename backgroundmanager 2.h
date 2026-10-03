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
        
        // draw.ImageRect(imgTest, { 4, 4 }, imgTest.Size() * 2);
        ptrPGE->GetDraw().ImageRect(imgBackground, { 0.0f, 0.0f }, ptrPGE->GetScreen().Size());


    }

private:
    olc::PixelGameEngine* ptrPGE = nullptr;
    olc::Image imgBackground;
};
