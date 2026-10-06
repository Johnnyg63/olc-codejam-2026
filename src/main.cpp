/*
	<TBA>

	<TBA>>

	Licenced under the OLC-3 License
*/


// Define OLC_PGE3_APPLICATION to include the implementation of 
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION
#include "olcUTIL3_Geometry2D.h"
#include "olcPixelGameEngine3.h"

#define OLC_PGEX3_MINIAUDIO
#include "olcPGEX3_Miniaudio.h"

#include "soundmanager.h"
#include "levelmanager_hold.h"
#include "tiledlevelmanager.h"
#include "player.h"
#include "backgroundmanager.h"
#include "cameramanager.h"
#include "imagemanager.h"

// This class overrides the olc::PixelGameEngine base class
// by implementing the OnUserCreate() and OnUserUpdate()
// functions
class JohnnyChange : public olc::PixelGameEngine
{

public:
    Player player;
	LevelManager_hold levelManager;
	TiledLevelManager tiledLevelManager;
	SoundManager soundManager;
	BackgroundManager backgroundManager;
	CameraManager cameraManager;
	ImageManager imageManager;
	// Copyright notice
	std::string strCopyrightNotice_MusicLFiles;
	olc::Image imgCopyright_Emscripten;
	olc::Image imgCopyright_OLC;
	olc::Image imgCopyRight_Kenny;
	olc::Image imgCopyRight_Tiled;
    
    // For demonstration controls, with sensible default values
    float pan    = 0.0f;
    float pitch  = 1.0f;
    float volume = 1.0f;
    float distance = 0.0f;
    bool backgroundPlay = false;
    ma_uint64 cursorMillis = 0ull;
    float     cursorFloat  = 0.0f;


	// Game menu enumeration and current selection
	enum GAME_MENU
	{
		MAIN_MENU = 0,
		GAME_LEVEL,
		CREDITS,
		DEBUG

	};

	GAME_MENU eGameMenu = MAIN_MENU;


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
		//if (!LoadImages()) 			{throw std::runtime_error("Failed to load images");};
		if (!LoadSounds()) 			{throw std::runtime_error("Failed to load sounds");};
        if (!LoadBackground())      {throw std::runtime_error("Failed to load background");};
		if (!LoadLevel()) 			{throw std::runtime_error("Failed to load level");};
        if (!LoadTitledLevel())     {throw std::runtime_error("Failed to load tiled level");};
		if (!LoadCameraManager()) 	{throw std::runtime_error("Failed to load camera manager");};
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
        
        // Handle background drawing and updates
        if (!HandleBackground(fElapsedTime)) {throw std::runtime_error("Failed to handle background");};

		// Handle audio playback and controls 
		if (!HandleSound(fElapsedTime)) {throw std::runtime_error("Failed to handle sound");};
		
		// Draw the level
		//if(!HandleLevel(fElapsedTime)) {throw std::runtime_error("Failed to handle level");};
        
        // Draw the Tiled Level
        if(!HandleTitledLevel(fElapsedTime)) {throw std::runtime_error("Failed to handle tiled level");};

		// Draw the camera manager
		if(!HandleCameraManager(fElapsedTime)) {throw std::runtime_error("Failed to handle camera manager");};
				
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
		bool res = 0;
		soundManager.Initialize(this, &extMiniAudio);
		// Temp code to load sounds without specifying files, to be updated later
		
		std::vector<loadSound> vecLoadSounds; // Temporary container for sounds to load
        vecLoadSounds.push_back({"Cheerful_Annoyance", "assets/sounds/background/Cheerful_Annoyance.mp3", true, true});
        vecLoadSounds.push_back({"bong_001", "assets/sounds/fx/bong_001.mp3", false, false});
        vecLoadSounds.push_back({"jump1", "assets/sounds/fx/jump1.mp3", false, false});
        vecLoadSounds.push_back({"jump2", "assets/sounds/fx/jump2.mp3", false, false});
        vecLoadSounds.push_back({"jump3", "assets/sounds/fx/jump3.mp3", false, false});
        vecLoadSounds.push_back({"jump4", "assets/sounds/fx/jump4.mp3", false, false});
		vecLoadSounds.push_back({"jump5", "assets/sounds/fx/jump5.mp3", false, false});
		vecLoadSounds.push_back({"footstep_concrete_001", "assets/sounds/fx/footstep_concrete_001.mp3", false, false});
		res = soundManager.LoadSounds(vecLoadSounds);

		vecLoadSounds.clear(); // Clear the temporary container after loading sounds
		return res > 0; // Return true if at least one sound was successfully loaded
	}

	/*
	Hand sounds temp before moving to class
	*/
	bool HandleSound(float fElapsedTime)
	{
		bool res = true;
		SoundProperties soundProperties;
		// TODO :Move to new location
		soundManager.Update(fElapsedTime);
		res = soundManager.HandleSound(fElapsedTime);

		return res;	
	}

	bool LoadImages()
	{
		bool res = true;
		imageManager.Initialize(this);
		std::vector<loadImage> vecLoadImages;
		// /Users/mickymacm4/Documents/olc-codejam-2026/assets/images/playerblue
		vecLoadImages.push_back({"dead", "assets/images/playerblue/playerBlue_dead.png"});
		vecLoadImages.push_back({"duck", "assets/images/playerblue/playerBlue_duck.png"});
		vecLoadImages.push_back({"fall", "assets/images/playerblue/playerBlue_fall.png"});
		vecLoadImages.push_back({"hit", "assets/images/playerblue/playerBlue_hit.png"});
		vecLoadImages.push_back({"roll", "assets/images/playerblue/playerBlue_roll.png"});
		vecLoadImages.push_back({"stand", "assets/images/playerblue/playerBlue_stand.png"});
		vecLoadImages.push_back({"swim1", "assets/images/playerblue/playerBlue_swim1.png"});
		vecLoadImages.push_back({"swim2", "assets/images/playerblue/playerBlue_swim2.png"});
		vecLoadImages.push_back({"switch1", "assets/images/playerblue/playerBlue_switch1.png"});
		vecLoadImages.push_back({"switch2", "assets/images/playerblue/playerBlue_switch2.png"});
		vecLoadImages.push_back({"up1", "assets/images/playerblue/playerBlue_up1.png"});
		vecLoadImages.push_back({"up2", "assets/images/playerblue/playerBlue_up2.png"});
		vecLoadImages.push_back({"up3", "assets/images/playerblue/playerBlue_up3.png"});
		vecLoadImages.push_back({"walk1", "assets/images/playerblue/playerBlue_walk1.png"});
		vecLoadImages.push_back({"walk2", "assets/images/playerblue/playerBlue_walk2.png"});	
		vecLoadImages.push_back({"walk3", "assets/images/playerblue/playerBlue_walk3.png"});
		vecLoadImages.push_back({"walk4", "assets/images/playerblue/playerBlue_walk4.png"});
		vecLoadImages.push_back({"walk5", "assets/images/playerblue/playerBlue_walk5.png"});
		res = imageManager.LoadImages(vecLoadImages);
		return res;
	}


	/*
	Load and initialize the background manager
	*/
	bool LoadBackground()
	{
		bool res = true;
		backgroundManager.Initialize(this);
		res = backgroundManager.LoadBackground("assets/images/backgrounds/skybox-night.png");
		return res;
	}

	/*
	Handle the background manager
	*/
	bool HandleBackground(float fElapsedTime)
	{
		bool res = true;
		backgroundManager.Update(fElapsedTime);
		backgroundManager.Draw();
		return res;
	}

	bool LoadTitledLevel()
	{
		bool res = true;
		tiledLevelManager.Initialize(this);

		res = tiledLevelManager.LoadLevel("assets/images/tiledsheets/level_tilesheet.png", "assets/tiledprojects/Level1Output.tmx", 1);
		return res;
	}

	bool HandleTitledLevel(float fElapsedTime)
	{
		bool res = true;
		draw.SetWorldTransform(cameraManager.camera.GetWorldTransform());
		tiledLevelManager.DisplayLevel(fElapsedTime);
		draw.WorldReset();
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

	bool LoadCameraManager()
	{
		bool res = true;
		cameraManager.Initialize(this, &tiledLevelManager, &soundManager, &imageManager);

		return res;
	}


	bool HandleCameraManager(float fElapsedTime)
	{
		bool res = true;
		cameraManager.Update(fElapsedTime);
		return res;
	}

	/*
	Load the level data and initialize the level manager
	*/
	bool LoadPlayer()
	{
		bool res = true;
		player.Initialize(this, soundManager);
		return res;
	}

	bool HandlePlayer(float fElapsedTime)
	{
		bool res = true;
		player.SetPlayerPosition(cameraManager.vTrackedPoint);
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
		res = CreateImageFromFile(imgCopyright_Emscripten, "assets/images/emscripten_logo.png");
		res = CreateImageFromFile(imgCopyright_OLC, "assets/images/olc_logo.png");
		res = CreateImageFromFile(imgCopyRight_Kenny, "assets/images/kenny_logo.png");
		res = CreateImageFromFile(imgCopyRight_Tiled, "assets/images/tiled_logo.png");

		/*
		olc::Image imgCopyright_Emscripten;
	olc::Image imgCopyright_OLC;
	olc::Image imgCopyRight_Kenny;
		*/


		if(!res) printf("Failed to load assets/images/emscripten_logo.png\n");
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

		fCopyrightNoticeY = GetScreen().Size().y - (imgCopyright_OLC.Size().y * 0.25f);
		fCopyrightNoticeX += imgCopyright_Emscripten.Size().x + 10.0f;
		draw.Image(imgCopyright_OLC, {fCopyrightNoticeX, fCopyrightNoticeY}, {0.25f,0.25f}); 

		fCopyrightNoticeX += imgCopyright_OLC.Size().x + 10.0f;
		fCopyrightNoticeY = GetScreen().Size().y - (imgCopyRight_Kenny.Size().y * 0.25f);
		draw.Image(imgCopyRight_Kenny, {fCopyrightNoticeX, fCopyrightNoticeY}, {0.25f,0.25f}); 

		fCopyrightNoticeX += imgCopyRight_Kenny.Size().x + 10.0f;
		fCopyrightNoticeX += imgCopyRight_Tiled.Size().x + 10.0f;
		fCopyrightNoticeY = GetScreen().Size().y - (imgCopyRight_Tiled.Size().y * 0.25f);
		draw.Image(imgCopyRight_Tiled, {fCopyrightNoticeX, fCopyrightNoticeY}, {0.25f,0.25f}); 
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
	config.bVSync = true;
	config.vPixelSize = { 1,1 };
	config.vScreenSize = { 1280, 768 };
	config.bFullScreen = false;
	
	if (demo.Construct(config))
	{
		// Start the application
		demo.Start();
	}

	return 0;
}
