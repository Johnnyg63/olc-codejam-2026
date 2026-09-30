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
    
}; 

class SoundManager : public olc::PixelGameEngine {

private:
    // sounds
	olc::ext::Miniaudio::Sound soundBackgroundMusic;
	olc::ext::Miniaudio::Sound soundFXSample;
    SoundProperties DefaultProperties; // Default Sound Properties

public:
    // Extensions 
	olc::ext::Miniaudio::AudioEngine* extMiniAudio; // Pointer to the audio engine instance

public:
    // Properties for different sound types
    SoundProperties spMasterSound; // Master Sound Properties
    SoundProperties spBackground;  // Background Music Properties
    SoundProperties spFxSound;     // Sound Effects Properties


public:
    SoundManager() {};

    olc::PixelGameEngine* ptrPGE = nullptr;

    /*
        Initializes the SoundManager with the given PixelGameEngine instance and Miniaudio AudioEngine instance.
        Parameters:
            engine - Pointer to the PixelGameEngine instance.
            audioEngine - Pointer to the Miniaudio AudioEngine instance.
    */
    void Initialize(olc::PixelGameEngine* engine, olc::ext::Miniaudio::AudioEngine* audioEngine) {
        ptrPGE = engine;
        extMiniAudio = audioEngine;
        spBackground.volume = 1.0f;
        spFxSound.volume = 1.0f;
    }  
    
    /*
        Updates the sound properties for the master sound, background music, and sound effects.
        Parameters:
            fElapsedTime - The elapsed time since the last update.
            MasterSound  - The properties for the master sound.
            Background   - The properties for the background music.
            Fx           - The properties for the sound effects.
    */
    void Update(float fElapsedTime, const SoundProperties& MasterSound, const SoundProperties& Background, const SoundProperties& Fx) {

        // Update individual sound properties based on the passed-in parameters
        spMasterSound = MasterSound;
        spBackground  = Background;
        spFxSound     = Fx; 
       
    }

    /*
        Draws the sound manager interface or updates sound-related visuals.
        Parameters:
            fElapsedTime - The elapsed time since the last draw call.
    */
    void Draw(float fElapsedTime) {
        // update sound for changes in drawing... maybe not required
    }
    
    // Plays the required sound affect TODO: Update to handle master and individual sound properties
    void PlaySoundAffect()
    {
        soundFXSample.Play();
    }

    /*
        Loads the required sound files into the audio engine.
        Returns:
            true if all sounds were loaded successfully, false otherwise.
    */
    bool LoadSounds()
    {
        bool res = true;
        res = extMiniAudio->CreateSoundFromFile(soundBackgroundMusic, "assets/sounds/background/Cheerful_Annoyance.ogg");
        ma_sound_set_position(soundBackgroundMusic.GetMASound(), 0.0f, 0.0f, 0.0f);
        res = extMiniAudio->CreateSoundFromFile(soundFXSample, "assets/sounds/fx/jump1.ogg");
        return res;
    };

    /*
        Handles the sound updates based on the elapsed time and user input.
        Parameters:
            fElapsedTime - The elapsed time since the last update.
        Returns:
            true if the sound handling was successful, false otherwise.
    */
    bool HandleSound(float fElapsedTime) {
        olc_IgnoreUnused(fElapsedTime);

		bool res = true;

        // Reset all sound properties to default if 'R' is pressed
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::R).bPressed)
        {
            spMasterSound = DefaultProperties;
            spBackground = DefaultProperties;
            spFxSound = DefaultProperties;
        }

		// toggle background playback
		if(ptrPGE->GetKeyboard().GetKey(olc::Key::K1).bPressed)
		{
			spBackground.backgroundPlay = !spBackground.backgroundPlay;
			if(spBackground.backgroundPlay)
				extMiniAudio->EnableBackgroundPlayback();
			else
				extMiniAudio->DisableBackgroundPlayback();
		}

		// toggle `song1` playback/pause
 		if(ptrPGE->GetKeyboard().GetKey(olc::Key::B).bPressed)
 			soundBackgroundMusic.Toggle();

        // Master volume
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::MINUS).bHeld)
            spMasterSound.volume -= 1.0f * fElapsedTime;
        
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::EQUALS).bHeld)
            spMasterSound.volume += 1.0f * fElapsedTime;
    
        // Lets keep some order to the madness
        spMasterSound.pan = std::clamp(spMasterSound.pan, 0.0f, 1.0f);
        spMasterSound.pitch = std::clamp(spMasterSound.pitch, 0.0f, 1.0f);
        spMasterSound.volume = std::clamp(spMasterSound.volume, 0.0f, 1.0f);

        ptrPGE->GetDraw().String({10, 30}, "Volume: " + std::to_string(spMasterSound.volume), olc::Colour::WHITE);

        // As there are three types of sounds (master, background, fx), we need to update each accordingly.
        // Master sound properties are applied to all sounds as a base multiplier
        spBackground.volume = std::clamp(spBackground.volume * spMasterSound.volume, 0.0f, 1.0f);
        soundBackgroundMusic.SetVolume(spBackground.volume);

        spFxSound.volume = std::clamp(spFxSound.volume * spMasterSound.volume, 0.0f, 1.0f);
        soundFXSample.SetVolume(spFxSound.volume);

		return res;
    }


};
