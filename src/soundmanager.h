#pragma once
#include "olcPixelGameEngine3.h"
#include "olcPGEX3_Miniaudio.h"
#include <algorithm>

/**
 * @brief Structure to hold sound properties such as pan, pitch, volume, and playback settings.
 */
struct SoundProperties{
    float pan               = 0.0f;
    float pitch             = 1.0f;
    float volume            = 0.125f;
    bool backgroundPlay     = false;
    ma_uint64 cursorMillis  = 0ull;
    
}; 

/**
 * @brief Structure to hold information about a sound to be loaded, including its name, file path, and whether it is background music.
 */
struct loadSound{
    std::string strName;             // Name of the sound
    std::string strPath;             // Path to the sound file
    bool bIsBackgroundMusic = false; // Indicates if the sound is background music
    bool bIsLooping = false;         // Indicates if the sound should loop
};

class SoundManager : public olc::PixelGameEngine {

private:

    SoundProperties DefaultProperties;  // Default Sound Properties
    uint32_t uniqueIDCounter = 0;       // Counter to generate unique IDs for sounds
    bool bTottleBackGoundMusic = false; // Toggle for background music playback


    struct Sounds{
        uint32_t id;                                 // Unique identifier for the sound (Automatically assigned)
        bool bIsLoaded = false;                      // Indicates if the FX sample has been loaded
        bool bIsPlaying = false;                     // Indicates if the sound is currently playing
        bool bIsBackgroundMusic = false;             // Indicates if the sound is background music else fxSound
        bool bIsLooping = false;                     // Indicates if the sound is set to loop
        std::string strName;                         // Name of the sound
        olc::ext::Miniaudio::Sound* pMiniAudioSound; // Auto created by the audio engine
    };

    
public:
    
	olc::ext::Miniaudio::AudioEngine* extMiniAudio; // Pointer to the audio engine instance

    std::vector<Sounds> vecSounds; // Container for all loaded sounds
    SoundProperties spMasterSound; // Master Sound Properties
    SoundProperties spBackground;  // Background Music Properties
    SoundProperties spFxSound;     // Sound Effects Properties

public:
    
    SoundManager() = default;
    
    ~SoundManager(){
        // Clean up
        if(vecSounds.size() > 0)
        {
            for(auto& sound : this->vecSounds){
                if(sound.pMiniAudioSound) {
                    extMiniAudio->DestroySound(*sound.pMiniAudioSound);
                    delete sound.pMiniAudioSound;
                }
            }
        }
        this->vecSounds.clear();
    }
    
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
        spMasterSound.volume = 0.125f;
        spBackground.volume = 0.5f;
        spFxSound.volume = 0.5f;
    }
    
    
    /*
        Updates the sound manager, including the states of all loaded sounds.
        Parameters:
            fElapsedTime - The elapsed time since the last update.
    */
    void Update(float fElapsedTime) {
        olc_IgnoreUnused(fElapsedTime);
        UpdateSoundStates();
       
    }
    
    /*
        Updates the playing state of each sound in vecSounds based on whether they are currently playing or not.
     */
    void UpdateSoundStates() {
     
        for(auto& sound : vecSounds)
        {
            if(sound.bIsPlaying && sound.pMiniAudioSound)
            {
                if(!sound.pMiniAudioSound->IsPlaying())
                {
                    sound.bIsPlaying = false;
                }
            }
        }
    }

        
    /*
        Plays the sound effect corresponding to the given sound ID.
        Parameters:
            soundID - The unique identifier of the sound to be played as stored in vecSounds.
            bStop - If true, the sound will be stopped before playing it again.
            bNoOverlap - If true, the sound will not overlap with itself if it is already playing. If false, it will play regardless of its current state.
     */
    void PlaySoundAffect(uint32_t soundID, bool bStop = false, bool bNoOverlap = false)
    {
        for(auto& sound : vecSounds)
        {
            if(sound.id == soundID && sound.pMiniAudioSound)
            {
                // If the sound is already playing and no overlap is allowed, stop the sound before playing it again
                if(!bStop) [[likely]]
                {
                    if(bNoOverlap && sound.bIsPlaying) break;
                    sound.pMiniAudioSound->Play(sound.bIsLooping);
                    sound.bIsPlaying = true;
                }
                else
                {
                    sound.pMiniAudioSound->Stop();
                    sound.bIsPlaying = false;
                }
               
                break;
            }
        }

    }

    /*
        Loads the specified sounds into the sound manager.
        Parameters:
            vecLoadSounds - A vector containing the sounds to be loaded.
            bReset - If true, clears existing sounds before loading new ones.
        Returns:
            The number of successfully loaded sounds.
    */
    uint32_t LoadSounds(std::vector<loadSound> vecLoadSounds, bool bReset = false)
    {
        int res = -1;
        if(bReset)
        {
            for(auto& sound : this->vecSounds){
                if(sound.pMiniAudioSound) {
                    extMiniAudio->DestroySound(*sound.pMiniAudioSound);
                    delete sound.pMiniAudioSound;
                }
            }
            this->vecSounds.clear();
            uniqueIDCounter = 0;
            res = 0;
        }
        
        for(const auto& sound : vecLoadSounds)
        {
            olc::ext::Miniaudio::Sound* pNewSound = new olc::ext::Miniaudio::Sound();
            bool bLoaded = extMiniAudio->CreateSoundFromFile(*pNewSound, sound.strPath);
            
            if(bLoaded)
            {
                Sounds newSound {
                    .id                 = uniqueIDCounter++,
                    .bIsBackgroundMusic = sound.bIsBackgroundMusic,
                    .bIsLoaded          = true,
                    .bIsPlaying         = false,
                    .bIsLooping         = sound.bIsLooping,
                    .strName            = sound.strName,
                    .pMiniAudioSound    = pNewSound
                };
        
                this->vecSounds.push_back(newSound);
                res++;
            }
            else
            {
                delete pNewSound;
            }
        }

        return res;
    };
    
    /*
        Retrieves the first ID of a sound by its name.
        Parameters:
            soundName - The name of the sound to search for.
        Returns:
            The ID of the sound if found, UINT32_MAX otherwise.
    */
    uint32_t GetSoundIDByName(const std::string& soundName) const
    {
        auto it = std::find_if(vecSounds.begin(), vecSounds.end(), [&soundName](const Sounds& sound) {
            return sound.strName == soundName;
        });
        
        if(it != vecSounds.end())
        {
            return it->id;  // Return the sound's ID
        }
        
        return UINT32_MAX;  // Return invalid ID if not found
    }

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
            spBackground  = DefaultProperties;
            spFxSound     = DefaultProperties;
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
        {
            auto id = GetSoundIDByName("Cheerful_Annoyance");
            if(id != UINT32_MAX)
            {
                bTottleBackGoundMusic = !bTottleBackGoundMusic;
                PlaySoundAffect(id, !bTottleBackGoundMusic, true);
            }

        }
 			
        // Master volume
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::MINUS).bHeld)
            spMasterSound.volume -= 1.0f * fElapsedTime;
        
        if(ptrPGE->GetKeyboard().GetKey(olc::Key::EQUALS).bHeld)
            spMasterSound.volume += 1.0f * fElapsedTime;
    
        // Lets keep some order to the madness
        spMasterSound.pan    = std::clamp(spMasterSound.pan, 0.0f, 1.0f);
        spMasterSound.pitch  = std::clamp(spMasterSound.pitch, 0.0f, 1.0f);
        spMasterSound.volume = std::clamp(spMasterSound.volume, 0.0f, 1.0f);

        // TODO remove this debug output
        // Master sound properties are applied to all sounds as a base multiplier
        ptrPGE->GetDraw().String({10, 30}, "Master Volume: " + std::to_string(spMasterSound.volume), olc::Colour::WHITE);
        ptrPGE->GetDraw().String({10, 50}, "Background Volume: " + std::to_string(spBackground.volume) + " Output Volume: " + std::to_string(spBackground.volume * spMasterSound.volume), olc::Colour::WHITE);
        ptrPGE->GetDraw().String({10, 70}, "FX Volume: " + std::to_string(spFxSound.volume) + " Output Volume: " + std::to_string(spFxSound.volume * spMasterSound.volume), olc::Colour::WHITE);

        for(auto& sound : vecSounds)
        {
            if(sound.bIsBackgroundMusic && sound.pMiniAudioSound)
                sound.pMiniAudioSound->SetVolume(std::clamp(spBackground.volume * spMasterSound.volume, 0.0f, 1.0f));
            else if(!sound.bIsBackgroundMusic && sound.pMiniAudioSound)
                sound.pMiniAudioSound->SetVolume(std::clamp(spFxSound.volume * spMasterSound.volume, 0.0f, 1.0f));
        }

		return res;
    }


};
