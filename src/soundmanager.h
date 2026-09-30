#pragma once
#include "olcPixelGameEngine3.h"
#include "olcPGEX3_Miniaudio.h"
#include <algorithm>


struct SoundProperties{
    float pan               = 0.0f;
    float pitch             = 1.0f;
    float volume            = 0.5f;
    bool backgroundPlay     = false;
    ma_uint64 cursorMillis  = 0ull;
    
} SoundProp; 

class SoundManager : public olc::PixelGameEngine {

private:
    // sounds
	olc::ext::Miniaudio::Sound backgroundMusic;
	olc::ext::Miniaudio::Sound fxSample;

public:
    // Extensions 
	olc::ext::Miniaudio::AudioEngine* extMiniAudio;


public:
    SoundManager() {};

    olc::PixelGameEngine* ptrPGE = nullptr;

    void Initialize(olc::PixelGameEngine* engine, olc::ext::Miniaudio::AudioEngine* audioEngine) {
        ptrPGE = engine;
        extMiniAudio = audioEngine;
    }  
    
    void Update(float fElapsedTime, const SoundProperties& MasterSound) {
        // TODO Update to handle master and individual sound properties
        //SoundProp = MasterSound;
        //backgroundMusic.SetVolume(MasterSound.volume * SoundProp.volume);
        //backgroundMusic.SetPan(MasterSound.pan + SoundProp.pan);
        // backgroundMusic.SetPitch(MasterSound.pitch * SoundProp.pitch);
       
    }

    void Draw(float fElapsedTime) {
        // update sound for changes in drawing... maybe not required
    }
    
    // Plays the required sound affect TODO: Update to handle master and individual sound properties
    void PlaySoundAffect()
    {
        fxSample.Play();
    }

    bool LoadSounds()
    {
        bool res = true;
        res = extMiniAudio->CreateSoundFromFile(backgroundMusic, "assets/sounds/ItalianMom.mp3");
        ma_sound_set_position(backgroundMusic.GetMASound(), 0.0f, 0.0f, 0.0f);
        res = extMiniAudio->CreateSoundFromFile(fxSample, "assets/sounds/SampleA.wav");
        return res;
    };

    bool HandleSound(float fElapsedTime) {
        olc_IgnoreUnused(fElapsedTime);

		bool res = true;
		// toggle background playback
		if(ptrPGE->GetKeyboard().GetKey(olc::Key::K1).bPressed)
		{
			SoundProp.backgroundPlay = !SoundProp.backgroundPlay;
			if(SoundProp.backgroundPlay)
				extMiniAudio->EnableBackgroundPlayback();
			else
				extMiniAudio->DisableBackgroundPlayback();
		}

		// toggle `song1` playback/pause
 		if(ptrPGE->GetKeyboard().GetKey(olc::Key::B).bPressed)
 			backgroundMusic.Toggle();

			

        // volume
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::MINUS).bHeld)
            SoundProp.volume -= 1.0f * fElapsedTime;
        
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::EQUALS).bHeld)
            SoundProp.volume += 1.0f * fElapsedTime;
    
        // Lets keep some order to the madness
        SoundProp.pan = std::clamp(SoundProp.pan, 0.0f, 1.0f);
        SoundProp.pitch = std::clamp(SoundProp.pitch, 0.0f, 1.0f);
        SoundProp.volume = std::clamp(SoundProp.volume, 0.0f, 1.0f);
        
        ptrPGE->GetDraw().String({10, 30}, "Volume: " + std::to_string(SoundProp.volume), olc::Colour::WHITE);

        // Reset pan, pitch, and volume
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::R).bPressed)
        {
            SoundProp.pan = 0.0f;
            SoundProp.pitch = 1.0f;
            SoundProp.volume = 0.5f;
      
        }

        backgroundMusic.SetVolume(SoundProp.volume);
        backgroundMusic.SetPan(SoundProp.pan + SoundProp.pan);
        backgroundMusic.SetPitch(SoundProp.pitch * SoundProp.pitch);

		return res;
    }


};
