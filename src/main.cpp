/*
	<TBA>

	<TBA>>

	Licenced under the OLC-3 License
*/


// Define OLC_PGE3_APPLICATION to include the implementation of 
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION
#include "olcPixelGameEngine3.h"

#define OLC_PGEX3_MINIAUDIO
#include "olcPGEX3_Miniaudio.h"

#include "soundmanager.h"
#include "levelmanager.h"
#include "player.h"


// This class overrides the olc::PixelGameEngine base class
// by implementing the OnUserCreate() and OnUserUpdate()
// functions
class JohnnyChange : public olc::PixelGameEngine
{

private:
	LevelManager levelManager;
	Player player;
	SoundManager soundManager;
	// Copyright notice
	std::string strCopyrightNotice_MusicLFiles;
	olc::Image imgCopyright_Emscripten;
	olc::Image imgCopyright_OLC;
	olc::Image imgCopyRight_Kenny;


    // For demonstration controls, with sensible default values
    float pan    = 0.0f;
    float pitch  = 1.0f;
    float volume = 1.0f;
    float distance = 0.0f;
    bool backgroundPlay = false;
    ma_uint64 cursorMillis = 0ull;
    float     cursorFloat  = 0.0f;

public:
	// Extensions 
	olc::ext::Miniaudio::AudioEngine extMiniAudio;


public:
	JohnnyChange()
	{
		sAppName = "Example - Johnny Has To Change";
		if(!InstallSystemExtension(&extMiniAudio))
			throw std::runtime_error("Failed to install olcPGEX3_Miniaudio");
	}

public:
	// Called once at the start, so create things here
	bool OnUserCreate() override
	{
		if (!LoadSounds()) 			{throw std::runtime_error("Failed to load sounds");};
		if (!LoadLevel()) 			{throw std::runtime_error("Failed to load level");};
		if (!LoadPlayer()) 			{throw std::runtime_error("Failed to load player");};
		if (!LoadCopyRightNotice()) {throw std::runtime_error("Failed to load copyright notice");};
        
		return true;
	}

public:

	// Called every frame, so update things here
	bool OnUserUpdate(float fElapsedTime) override
	{
		// Handle screen clearing, change backgrounds etc later
		if (!ClearScreen(fElapsedTime, olc::Colour::BLACK)) {throw std::runtime_error("Failed to clear screen");};

		// Handle audio playback and controls 
		if (!HandleSound(fElapsedTime)) {throw std::runtime_error("Failed to handle sound");};
		
		// Draw the level
		if(!HandleLevel(fElapsedTime)) {throw std::runtime_error("Failed to handle level");};

		// Draw the player
		if(!HandlePlayer(fElapsedTime)) {throw std::runtime_error("Failed to handle player");};
				
		// Handle copyright notices (Keep this last to ensure it overlays everything else)
		if (!HandleCopyRightNotices(fElapsedTime)) {throw std::runtime_error("Failed to handle copyright notices");};

		// Handle exit
		return HandleExit(fElapsedTime);

	}


private:
	// Clears the screen with the specified color (default is black)
	bool ClearScreen(float fElapsedTime, olc::Pixel col = olc::Colour::BLACK)
	{
		olc_IgnoreUnused(fElapsedTime);
		bool res = true;

		draw.Clear(col);
		
		return res;
	}

	
	/*
	Load default sounds into the audio engine, to be moved to sound class later
	*/
	bool LoadSounds()
	{
		bool res = true;
		soundManager.Initialize(this, &extMiniAudio);
		res = soundManager.LoadSounds();
		return res;
	}

	/*
	Hand sounds temp before moving to class
	*/
	bool HandleSound(float fElapsedTime)
	{
		bool res = true;
		SoundProperties soundProperties;
		soundManager.Update(fElapsedTime, soundProperties);
		res = soundManager.HandleSound(fElapsedTime);
		return res;		
	}

	/*
	Load the level data and initialize the level manager
	*/
	bool LoadLevel()
	{
		bool res = true;
		levelManager.Initialize(this);
		return res;
	}

	bool HandleLevel(float fElapsedTime)
	{
		bool res = true;
		levelManager.Update(fElapsedTime);
		levelManager.Draw();
		return res;
	}

	/*
	Load the level data and initialize the level manager
	*/
	bool LoadPlayer()
	{
		bool res = true;
		player.Initialize(this);
		return res;
	}

	bool HandlePlayer(float fElapsedTime)
	{
		bool res = true;
		player.Update(fElapsedTime, levelManager);
		player.Draw(levelManager.currentPhase);
		return res;
	}

	/*
	Load Copyright Notice text and images
	*/
	bool LoadCopyRightNotice()
	{
		bool res = true;
		// Emscripten copyright notice
		res = CreateImageFromFile(imgCopyright_Emscripten, "assets/emscripten_logo.png");
		if(!res) printf("Failed to load assets/emscripten_logo.png\n");
		res = true; //TODO: Remove temp here keep things moving
		
		return res;
	}


	/*
	Handle Copyright Notices
	*/
	bool HandleCopyRightNotices(float fElapsedTime)
	{
		olc_IgnoreUnused(fElapsedTime);
		bool res = true;
		float fCopyrightNoticeX = 10.0f;
		float fCopyrightNoticeY = 320.0f;

		fCopyrightNoticeY = GetScreen().Size().y - (imgCopyright_Emscripten.Size().y * 0.25f);
		draw.Image(imgCopyright_Emscripten, {fCopyrightNoticeX, fCopyrightNoticeY}, {0.25f,0.25f}); 

		return res;
		
	}

	// Handle exit logic
	bool HandleExit(float fElapsedTime)
	{
		olc_IgnoreUnused(fElapsedTime);
#if OLC_HOST == OLC_HOST_EMSCRIPTEN
		return true;
#else		
		return !keyboard.GetKey(olc::Key::ESCAPE).bPressed;
#endif
	}
	
};




// Main entry point for the application
int main()
{
	// Construct demo application
	JohnnyChange demo;

	PGEConfig config;
	config.bVSync = false;
	config.vPixelSize = { 1,1 };
	config.vScreenSize = { 800,450 };
	config.bFullScreen = false;
	
	if (demo.Construct(config))
	{
		// Start the application
		demo.Start();
	}

	return 0;
}
