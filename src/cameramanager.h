
#pragma once
#include "olcPixelGameEngine3.h"
#include "olcUTIL3_Camera2D.h"

class CameraManager
{

private:

    // Player stuff!
    //olc::vf2d velocity = { 0, 0 };
    const float SPEED = 300.0f;
    const float GRAVITY = 1200.0f;
    const float JUMP_FORCE = -500.0f;
    const float WIDTH = 24.0f;
    const float HEIGHT = 36.0f;

    // Sound IDs for player actions
    uint32_t nJumpSoundID = UINT32_MAX;
    uint32_t nLandSoundID = UINT32_MAX;
    uint32_t nBongSoundID = UINT32_MAX;
    bool wasGroundedLastFrame = false;

public:
// World Map Properties

    // Camera utility class
    olc::utils::Camera2D camera;
    
    // For debug and testing purposes, allow free roam of the camera
    bool bFreeRoam = false;
    
    // World map parameters, should be set by level manager on load level
    std::vector<uint32_t> vecWorldMap;   // Our basic grid map!
    olc::vi2d viWorldSize = { 140, 24 }; // 2048 64 cells
    
    // Tile size, should be set by level manager on load level
    olc::vi2d viTileSize = { 32, 32 };
    
    // The point that represents the player, it is "tracked" by the camera
    olc::vf2d vTrackedPoint;
    
    // This is for the player, not now, later :) here so I don't forget
    olc::vi2d viSpriteSheetTiles = { 28, 14 };

public:
    CameraManager() {}
    ~CameraManager() {}

    bool Initialize(olc::PixelGameEngine* pge)
    {
        bool res = true;
        this->ptrPGE = pge;

        vTrackedPoint = { 20.0f, 20.0f }; // Initial position of the tracked point (player)
		camera = olc::utils::Camera2D(ptrPGE->ScreenSize(), viTileSize, vTrackedPoint); // Create the camera with screen size, tile size, and tracked point
		camera.SetTarget(vTrackedPoint);    // Set the point in the world we want the camera to track
		camera.SetMode(olc::utils::Camera2D::Mode::Simple); // Set the camera mode to simple, which follows the tracked point without any additional effects

		camera.SetWorldBoundary({ 0.0f, 0.0f }, viWorldSize); // Set the boundaries of the world for the camera
		camera.EnableWorldBoundary(true); // Enable the world boundary so the camera doesn't show areas outside the world

		// Create "tile map" world with just two tile types
		vecWorldMap.resize(viWorldSize.area());
		for (int i = 0; i < vecWorldMap.size(); i++)
			vecWorldMap[i] = ((rand() % 20) == 1) ? 1 : 0;

        return res;
    }

    void SetCameraTarget(olc::vf2d vTarget)
    {
        camera.SetTarget(vTarget);
    }

    void Update(float fElapsedTime)
    {
        // Update camera logic here
        ManageKeyboardInput(fElapsedTime);
    
		// Set the world transform for the camera, so that all drawing operations
		ptrPGE->GetDraw().SetWorldTransform(camera.GetWorldTransform());

        // // TODO This is our new collision and rendering logic for the tile map
		olc::vi2d vTileOffset = ptrPGE->GetDraw().ScreenToWorld({ 0,0 }).floor();
		olc::vi2d vTileCount = ptrPGE->GetDraw().ScreenToWorld(ptrPGE->ScreenSize()).ceil() - vTileOffset;

		// // Clamp to ensure we stay in bounds of our world map
		olc::vi2d vTileTL = vTileOffset.max({ 0,0 });
		olc::vi2d vTileBR = (vTileOffset + vTileCount).min(viWorldSize);
		olc::vi2d vTile;

		// Then looping through them and drawing them
		//auto batch = ptrPGE->GetDraw().CreateFilledBatch();

		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				// 2D -> 1D index conversion for our world map
				int idx = vTile.y * viWorldSize.x + vTile.x;

				if (vecWorldMap[idx] == 0)
					ptrPGE->GetDraw().Rect(vTile, { 1.0f, 1.0f }, olc::Colour::DARK_GREEN);

				if (vecWorldMap[idx] == 1)
					ptrPGE->GetDraw().Rect(vTile, { 1.0f, 1.0f }, olc::Colour::TANGERINE);
			}

		// Draw the batch of tiles
		// ptrPGE->GetDraw().Batch(batch);

		// Draw the "player" as a 1x1 cell
		ptrPGE->GetDraw().FilledRect(vTrackedPoint - olc::vf2d(0.5f, 0.5f), { 1.0f, 1.0f }, olc::Colour::BLUE);

		// Overlay with information
		if (bFreeRoam)
		{
			ptrPGE->GetDraw().FilledRect(camera.GetViewPosition(), camera.GetViewSize(), olc::PixelF(1.0f, 0.0f, 0.0f, 0.5f));			
		}

		
		// Reset world transform to draw info in screen space
		ptrPGE->GetDraw().WorldReset();

		if (bFreeRoam)
			ptrPGE->GetDraw().StringProp({ 2, 2 }, "TAB: Free Mode, M-Btn to Pan & Zoom", olc::Colour::YELLOW);
		else
			ptrPGE->GetDraw().StringProp({ 2,2 }, "TAB: Play Mode", olc::Colour::YELLOW);

		ptrPGE->GetDraw().StringProp({ 2,12 }, "WASD  : Move", olc::Colour::YELLOW);
		ptrPGE->GetDraw().StringProp({ 2,22 }, "CAMERA: 1) Simple  2) EdgeMove  3) LazyFollow  4) Screens 5) Slides", olc::Colour::YELLOW);
		ptrPGE->GetDraw().StringProp({ 2,42 }, vTileOffset.str(), olc::Colour::YELLOW);

    }

private:
    olc::PixelGameEngine* ptrPGE;

    void ManageKeyboardInput(float fElapsedTime)
    {
        // Handle player "physics" in response to key presses
        olc::vf2d vVel = { 0.0f, 0.0f };
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::W).bHeld) 
        {
            vVel = vVel + olc::vf2d{0, -1};
        }
        
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::S).bHeld)
        {
            vVel = vVel + olc::vf2d{0, +1};
        }
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::A).bHeld) vVel = vVel + olc::vf2d{-1, 0};
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::D).bHeld) vVel = vVel + olc::vf2d{+1, 0};
		vTrackedPoint += vVel * 8.0f * fElapsedTime;
        //vTrackedPoint = vNewTrackedPoint;

		// Switch between "free roam" and "play" mode with TAB key
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::TAB).bPressed)
		{
			bFreeRoam = !bFreeRoam;
		}

		// Update the camera, if teh tracked object remains visible, 
		// true is returned
		bool bOnScreen = false;

		if (bFreeRoam)
		{
			// In free roam mode, we ignore the tracked point and instead 
			// allow the user to pan and zoom the camera with the mouse
			camera.HandlePanAndZoom(ptrPGE->GetMouse());
			// Update camera, but dont actually change the world transform
			bOnScreen = camera.Update(fElapsedTime, false);
		}
		else
			// In play mode, we update the camera as normal, which will cause it to
			// follow the tracked point according to the camera mode
			bOnScreen = camera.Update(fElapsedTime);

    }

};