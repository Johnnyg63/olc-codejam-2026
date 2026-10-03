
#pragma once
#include "olcPixelGameEngine3.h"
#include "olcUTIL3_Camera2D.h"

class CameraManager
{

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

    void Initialize(olc::PixelGameEngine* pge)
    {
        bool res = true;
        this->ptrPGE = pge;

        vTrackedPoint = { 20.0f, 20.0f }; // Initial position of the tracked point (player)
		camera = olc::utils::Camera2D(ScreenSize(), vTileSize, vTrackedPoint); // Create the camera with screen size, tile size, and tracked point
		camera.SetTarget(vTrackedPoint);    // Set the point in the world we want the camera to track
		camera.SetMode(olc::utils::Camera2D::Mode::Simple); // Set the camera mode to simple, which follows the tracked point without any additional effects

		camera.SetWorldBoundary({ 0.0f, 0.0f }, vWorldSize); // Set the boundaries of the world for the camera
		camera.EnableWorldBoundary(true); // Enable the world boundary so the camera doesn't show areas outside the world

		// Create "tile map" world with just two tile types
		vecWorldMap.resize(vWorldSize.area());
		for (int i = 0; i < vecWorldMap.size(); i++)
			vecWorldMap[i] = ((rand() % 20) == 1) ? 1 : 0;

        return res;
    }

    bool SetCameraTarget(const olc::vf2d& vTarget)
    {
        camera.SetTarget(vTarget);
        return false;
    }

    void Update(float fElapsedTime)
    {
        // Update camera logic here
       // Handle player "physics" in response to key presses
		olc::vf2d vVel = { 0.0f, 0.0f };
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::W).bHeld) vVel = vVel + olc::vf2d{0, -1};
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::S).bHeld) vVel = vVel + olc::vf2d{0, +1};
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::A).bHeld) vVel = vVel + olc::vf2d{-1, 0};
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::D).bHeld) vVel = vVel + olc::vf2d{+1, 0};
		vTrackedPoint += vVel * 8.0f * fElapsedTime;

		// Switch between "free roam" and "play" mode with TAB key
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::TAB).bPressed)
		{
			bFreeRoam = !bFreeRoam;
		}

		// Switch camera mode in operation
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::K1).bPressed)
			camera.SetMode(olc::utils::Camera2D::Mode::Simple);
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::K2).bPressed)
			camera.SetMode(olc::utils::Camera2D::Mode::EdgeMove);
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::K3).bPressed)
			camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::K4).bPressed)
			camera.SetMode(olc::utils::Camera2D::Mode::FixedScreens);
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::K5).bPressed)
			camera.SetMode(olc::utils::Camera2D::Mode::SlideScreens);

		// Update the camera, if teh tracked object remains visible, 
		// true is returned
		bool bOnScreen = false;

		if (bFreeRoam)
		{
			// In free roam mode, we ignore the tracked point and instead 
			// allow the user to pan and zoom the camera with the mouse
			camera.HandlePanAndZoom(mouse);
			// Update camera, but dont actually change the world transform
			bOnScreen = camera.Update(fElapsedTime, false);
		}
		else
			// In play mode, we update the camera as normal, which will cause it to
			// follow the tracked point according to the camera mode
			bOnScreen = camera.Update(fElapsedTime);

		// Set the world transform for the camera, so that all drawing operations
		ptrPGE->GetDraw().SetWorldTransform(camera.GetWorldTransform());

		// Render "tile map", by getting visible tiles
		 
		// If we never change scale we can just use the view parameters
		// directly...
		//olc::vi2d vTileCount = camera.GetViewSize().ceil() + 1;
		//olc::vf2d vTileOffset = camera.GetViewPosition().floor();

		// ... but if we allow free zooming, then we need to convert 
		// screen coordinates to world coordinates to get the correct 
		// tile offsets and counts
		olc::vi2d vTileOffset = ptrPGE->GetDraw().ScreenToWorld({ 0,0 }).floor();
		olc::vi2d vTileCount = ptrPGE->GetDraw().ScreenToWorld(ScreenSize()).ceil() - vTileOffset;

		// Clamp to ensure we stay in bounds of our world map
		olc::vi2d vTileTL = vTileOffset.max({ 0,0 });
		olc::vi2d vTileBR = (vTileOffset + vTileCount).min(vWorldSize);
		olc::vi2d vTile;

		// Then looping through them and drawing them
		auto batch = ptrPGE->GetDraw().CreateFilledBatch();

		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				// 2D -> 1D index conversion for our world map
				int idx = vTile.y * vWorldSize.x + vTile.x;

				if (vecWorldMap[idx] == 0)
					ptrPGE->GetDraw().FilledRect(batch, vTile, { 1.0f, 1.0f }, olc::Colour::DARK_GREEN);

				if (vecWorldMap[idx] == 1)
					ptrPGE->GetDraw().FilledRect(batch, vTile, { 1.0f, 1.0f }, olc::Colour::TANGERINE);
			}

		// Draw the batch of tiles
		ptrPGE->GetDraw().Batch(batch);

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

		return true;
    }

private:
    olc::PixelGameEngine* ptrPGE;
    olc::utils::Camera2D camera;
};