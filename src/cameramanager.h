
#pragma once
#include "olcPixelGameEngine3.h"
#include "olcUTIL3_Camera2D.h"
#include "tiledlevelmanager.h"

class CameraManager
{

private:

    // Player stuff!
    bool bFirstFrame = true;              // Indicates if this is the first frame of the game loop
    int32_t nLastYPos = 0;                 // Stores the last frame's velocity.y pos
    olc::vf2d vVelJumpSpeed = {0, -2.4f}; // Current velocity of the player
    olc::vf2d vJumpCount = {0, 0};        // Keeps track of the jump duration or count
    olc::vf2d vVelJumpMax = {0, -24.0f};
    float fJumpMultiplier = 16.0f;
    bool bJumping = false;
    // Sound IDs for player actions
    uint32_t nJumpSoundID = UINT32_MAX;
    uint32_t nLandSoundID = UINT32_MAX;
    uint32_t nBongSoundID = UINT32_MAX;
    bool isGrounded = false;
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

    bool Initialize(olc::PixelGameEngine* pge, TiledLevelManager* ptrTLM, SoundManager* sound)
    {
        bool res = true;
        this->ptrPGE = pge;
        this->ptrTLM = ptrTLM;
        this->ptrSound = sound;

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


        // Update sounds:
        nJumpSoundID = ptrSound->GetSoundIDByName("jump1");
        nBongSoundID = ptrSound->GetSoundIDByName("bong_001");
        nLandSoundID = ptrSound->GetSoundIDByName("footstep_concrete_001");

        return res;
    }

    void SetCameraTarget(olc::vf2d vTarget)
    {
        camera.SetTarget(vTarget);
    }

    void Update(float fElapsedTime)
    {
		// Set the world transform for the camera, so that all drawing operations
        
        ptrPGE->GetDraw().StringProp({ 10,100 }, "Before Collisions: " + std::to_string(int(vTrackedPoint.x * 100)) + ", " + std::to_string(int(vTrackedPoint.y * 100)), olc::Colour::YELLOW);
		ptrPGE->GetDraw().SetWorldTransform(camera.GetWorldTransform());

        // Update camera logic here
        ManageKeyboardInput(fElapsedTime);

        olc::vf2d vCameraPos = camera.GetViewPosition();

        // TODO: I need to sort out this hack later
        olc::vf2d vfCenterPos = (vTrackedPoint * olc::vf2d(viTileSize)) - olc::vf2d(0.5f, 0.5f);
        
        
        UpdateCollisions(fElapsedTime, &vTrackedPoint, vfCenterPos, 16.0f);

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
    
		// Draw the "player" as a 1x1 cell
		ptrPGE->GetDraw().FilledRect(vTrackedPoint - olc::vf2d(0.5f, 0.5f), { 1.0f, 1.0f }, olc::Colour::BLUE);
        ptrPGE->GetDraw().FilledCircle(vTrackedPoint, 0.5f, olc::Colour::RED); // Draw the tracked point as a red circle

		// Overlay with information
		if (bFreeRoam)
		{
			ptrPGE->GetDraw().FilledRect(camera.GetViewPosition(), camera.GetViewSize(), olc::PixelF(1.0f, 0.0f, 0.0f, 0.5f));			
		}

        		
		// Reset world transform to draw info in screen space
		ptrPGE->GetDraw().WorldReset();
        
        ptrPGE->GetDraw().StringProp({ 10,120 }, "After Collisions: " + std::to_string(int(vTrackedPoint.x * 100)) + ", " + std::to_string(int(vTrackedPoint.y * 100)), olc::Colour::YELLOW);


		if (bFreeRoam)
			ptrPGE->GetDraw().StringProp({ 2, 2 }, "TAB: Free Mode, M-Btn to Pan & Zoom", olc::Colour::YELLOW);
		else
			ptrPGE->GetDraw().StringProp({ 2,2 }, "TAB: Play Mode", olc::Colour::YELLOW);

		

    }

private:
    olc::PixelGameEngine* ptrPGE = nullptr;
    TiledLevelManager* ptrTLM    = nullptr;
    SoundManager* ptrSound       = nullptr;

    void ManageKeyboardInput(float fElapsedTime)
    {
        // Handle player "physics" in response to key presses
        olc::vf2d vVel = { 0.0f, 0.0f };

        if(bFreeRoam)
        {
            if (ptrPGE->GetKeyboard().GetKey(olc::Key::W).bHeld)
            {
                vVel = vVel + olc::vf2d{0, -1};
            }
            
            if (ptrPGE->GetKeyboard().GetKey(olc::Key::S).bHeld)
            {
                vVel = vVel + olc::vf2d{0, +1};
            }
        }
        else
        {
            // Manage gravity in play mode
            if(!bJumping) // Disable Gravity when jumping, so player can move left and right while in the air
                vVel = vVel + olc::vf2d{0, +1}; // Apply gravity in play mode
        }
        
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::A).bHeld
            || ptrPGE->GetKeyboard().GetKey(olc::Key::LEFT).bHeld)
        {
            vVel = vVel + olc::vf2d{-1, 0};
        }
        if (ptrPGE->GetKeyboard().GetKey(olc::Key::D).bHeld
            || ptrPGE->GetKeyboard().GetKey(olc::Key::RIGHT).bHeld)
        {
                vVel = vVel + olc::vf2d{+1, 0};
        }
        
        // Manage jumping, but only if the player is on the ground (not falling)
        if(!bJumping)
        {
            if (ptrPGE->GetKeyboard().GetKey(olc::Key::SPACE).bPressed) 
            {
                vJumpCount = {0, 0}; // Reset jump count when jump starts
                bJumping = true;
                PlayPlayerSound(nJumpSoundID); // Play jump sound effect
            }

        }
        else
        {
            vJumpCount += vVelJumpSpeed * fJumpMultiplier* fElapsedTime;
            if(vJumpCount.y < vVelJumpMax.y)
            {
                bJumping = false;
                vJumpCount = {0, 0};
            }
            else {
                vVel = vVel + vVelJumpSpeed;
            }
        }
        
        
        vTrackedPoint += vVel * 8.0f * fElapsedTime;

        if(bFirstFrame)
        {
            nLastYPos = int32_t(vTrackedPoint.y * 100.0f);
            bFirstFrame = false;
        }
        else
        {
            // we need to check if y velocity has changed significantly to detect landing
            if(int(vTrackedPoint.y * 100.0f) == nLastYPos && bJumping)
            {
                PlayPlayerSound(nLandSoundID); // Play landing sound effect
            }
            nLastYPos = int32_t(vTrackedPoint.y * 100.0f);
        }

		// Switch between "free roam" and "play" mode with TAB key
		if (ptrPGE->GetKeyboard().GetKey(olc::Key::TAB).bPressed)
		{
			bFreeRoam = !bFreeRoam;
		}

		

    }

	olc::vf2d RotatePoint(olc::vf2d vfCenterPos, float fRadians, olc::vf2d vfPoint)
	{
		float tempX = vfPoint.x - vfCenterPos.x;
		float tempY = vfPoint.y - vfCenterPos.y;
		vfPoint.x = vfCenterPos.x + (tempX * cos(fRadians) - tempY * sin(fRadians));
		vfPoint.y = vfCenterPos.y + (tempX * sin(fRadians) + tempY * cos(fRadians));
		return vfPoint;
	}

    
    void UpdateCollisions(float fElapsedTime, olc::vf2d* pvfPositionPos, olc::vf2d vfCenterPos,
                                            float fRadius,
                                                bool pbEnableGravity = false, bool pbOnLadder = false)
        {
            // Enable Transformed view to make world offsetting simple
            //ptrPGE->GetDraw().SetWorldTransform(camera.GetWorldTransform());
            // Restrict to only to the tiles on the screen
            olc::vi2d vTileOffset = ptrPGE->GetDraw().ScreenToWorld({ 0,0 }).floor();
            olc::vi2d vTileCount = ptrPGE->GetDraw().ScreenToWorld(ptrPGE->ScreenSize()).ceil() - vTileOffset;

            // Clamp to ensure we stay in bounds of our world map
            olc::vi2d vTileTL = vTileOffset.max({ 0,0 });
            olc::vi2d vTileBR = (vTileOffset + vTileCount).min(viWorldSize);
            olc::vi2d vTile;

            // Layer stuff
            int32_t idx = 0;
            olc::TiledLevelManager::DecalInfo decalInfo;
            int32_t nLayer = 0;
            using namespace olc::utils::geom2d;

            // Collision stuff
            olc::vf2d vfDirection = { 0.0f, 0.0f };
            olc::vf2d vfClosest = { 0.0f, 0.0f };
            olc::vf2d vfDistance = { 0.0f, 0.0f };
            float fDistance = 0.0f;
            float fOverlap = 0.0f;

            // TODO: Add ladders, moving platforms, and other special tiles
                        
            // Rect collision stuff
            rect<float> worldTile;
            worldTile.pos.x = 0.0f;
            worldTile.pos.y = 0.0f;
            worldTile.size = olc::vf2d(viTileSize);

            // Polygon stuff
            olc::vf2d vfPoints[2];
            std::vector <olc::vf2d> vfPolyPoints;
            olc::vf2d vfNewClosest = { 0.0f, 0.0f };
            bool bIsFirstClosest = true;

            bool bOverLaps = false; // Is set when a circle overlaps a Rect/Triangle

            // Updates the player object position based on collisions with the world tiles,
            auto updatePos = [&]()
            {
                bool bCollided = false;
                if (decalInfo.sCollisionTile.bIsLadder == true)
                {
                    // we need to turn off gravity
                    pbEnableGravity = false;
                    pbOnLadder = true;
                    bCollided = true;
                    return bCollided;

                }

                vfDistance = vfCenterPos - vfClosest;

                fDistance = std::sqrt(vfDistance.x * vfDistance.x + vfDistance.y * vfDistance.y);
                fOverlap = fRadius - fDistance;

                if (fDistance != 0)
                {
                    // Move our player out of collision
                    vfCenterPos += (vfDistance / fDistance) * fOverlap;
                    vfDirection += (vfDistance / fDistance) * fOverlap;
                    bCollided = true;
                 
                }
                else
                {
                    // Handle the case where the circle's center is exactly on the rectangle's edge
                    if (vfDistance.x == 0) {
                        vfCenterPos.y += (vfCenterPos.y > worldTile.pos.y + worldTile.size.y / 2) ? fOverlap : -fOverlap;
                        vfDirection.y += (vfCenterPos.y > worldTile.pos.y + worldTile.size.y / 2) ? fOverlap : -fOverlap;
                    }
                    else {
                        vfCenterPos.x += (vfCenterPos.x > worldTile.pos.x + worldTile.size.x / 2) ? fOverlap : -fOverlap;
                        vfDirection.x += (vfCenterPos.x > worldTile.pos.x + worldTile.size.x / 2) ? fOverlap : -fOverlap;
                    }
                }

                /*
                * Note we add *a to declare we want to update the value
                * Javidx9 has a great video explaining pointers here : https://www.youtube.com/watch?v=iChalAKXffs)
                */
                *pvfPositionPos += vfDirection * fElapsedTime;
                return bCollided;
            };

            for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
                for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
                {
                    idx = vTile.y * viWorldSize.x + vTile.x;
                    /*
                    * Note we add *a to declare we want to access the value
                    * Javidx9 has a great video explaining pointers here : https://www.youtube.com/watch?v=iChalAKXffs
                    */
                    for (auto& layer : ptrTLM->Properties.mapLayerInfo)
                    {
                        bOverLaps = false;    // Reset our overlap
                        bIsFirstClosest = true;
                        decalInfo = layer.second[idx];    // We only care about the data (layer.data)

                        if (decalInfo.nTiledID == 0) continue;                      // If the tile does nothing just move on
                        
                        if (decalInfo.bHasCollision)
                        {
                            // Check for collision here - keep in world space
                            worldTile.pos = olc::vf2d(vTile.x * viTileSize.x, vTile.y * viTileSize.y);

                            for (auto& tileObject : decalInfo.sCollisionTile.vecTileObjects)
                            {
                                switch (tileObject.sCollisionType.eCollision)
                                {
                                    case TiledLevelManager::Collision::ELLIPSE:
                                    case TiledLevelManager::Collision::CAPSULE:
                                    {
                                        break;
                                    }
                                    case TiledLevelManager::Collision::CIRCLE:
                                    {
                                        // Get the closest point on the circle and a circle
                                        olc::vf2d vfTilePos = worldTile.pos + tileObject.vfPosition;
                                        worldTile.size = tileObject.vfSize;

                                        olc::vf2d vfCenter = vfTilePos + worldTile.size / 2.0f;
                                        float fCRadius = worldTile.size.x / 2.0f;

                                        bOverLaps = overlaps(circle<float>{vfCenter, fCRadius}, circle<float>{vfCenterPos, fRadius});
                                        if (bOverLaps)
                                        {
                                            // Get the closest point between a circle and a circle
                                            vfClosest = closest(circle<float>{vfCenter, fCRadius}, circle<float>{vfCenterPos, fRadius});
                                            bOverLaps = updatePos();
                                        }

                                        break;
                                    }
                                    case TiledLevelManager::Collision::POINT:
                                    {
                                        break;
                                    }
                                    case TiledLevelManager::Collision::POLYGON:
                                    {
                                        // Important we need to ensure our offset etc are applied, may need to be move to level manager
                                        for (auto& vfPoint : tileObject.sCollisionType.vecPoints)
                                        {
                                            auto vfRotatedPoint = tileObject.vfPosition;
                                            auto vfPosition = tileObject.vfPosition;
                                            if (tileObject.fRotationRad != 0.0f)
                                            {
                                                vfPoint = RotatePoint(vfRotatedPoint, tileObject.fRotationRad, vfPoint);
                                                vfPosition = { 0.0f, 0.0f };
                                            }
                                            olc::vf2d vfPointnew = worldTile.pos + (vfPoint + vfPosition); //vTile + vfWorldPoint;
                                            vfPolyPoints.push_back(vfPointnew);
                                        }

                                        // Get the approx centre of the polygon
                                        auto vfCenter = (std::accumulate(vfPolyPoints.begin(), vfPolyPoints.end(), olc::vf2d{ 0.0f, 0.0f })) / float(vfPolyPoints.size());

                                        for (int i = 0; i < vfPolyPoints.size(); i++)
                                        {
                                            bOverLaps = overlaps(triangle<float>{vfCenter, vfPolyPoints[i], vfPolyPoints[(i + 1) % vfPolyPoints.size()]}, circle<float>{vfCenterPos, fRadius});

                                            // If we over lap lets find the closest first and move back from there
                                            if (bOverLaps)
                                            {
                                                vfNewClosest = closest(triangle<float>{vfCenter, vfPolyPoints[i], vfPolyPoints[(i + 1) % vfPolyPoints.size()]}, circle<float>{vfCenterPos, fRadius});

                                                // use to ensure vfClosest is set the first overlap
                                                if (bIsFirstClosest)
                                                {
                                                    vfClosest = vfNewClosest;
                                                    bIsFirstClosest = false;
                                                }

                                                // Find the closet distance
                                                if (vfClosest > vfNewClosest) vfClosest = vfNewClosest;
                                                bOverLaps = updatePos();
                                            }

                                        }

                                        vfPolyPoints.clear(); // Clear our points for the next loop
                                        break;
                                    }
                                    case TiledLevelManager::Collision::RECT:
                                    {
                                        olc::vf2d vfTilePos = worldTile.pos + tileObject.vfPosition;
                                        worldTile.size = tileObject.vfSize;
                                        bOverLaps = overlaps(circle<float>{vfCenterPos, fRadius}, rect<float>{vfTilePos, worldTile.size});
                                        if (bOverLaps)
                                        {
                                            // Get the closest point between a circle and a rectangle
                                            vfClosest = closest(rect<float>{vfTilePos, worldTile.size}, circle<float>{vfCenterPos, fRadius});
                                            bOverLaps = updatePos();
                                        }

                                        break;
                                    }
                                    default:
                                    {
                                        break;
                                    }

                                } // switch (tileObject.sCollisionType.eCollision)

                            } // for (auto& layer : *Properties.ptrmapLayerInfo)

                        }  // if (decalInfo.bHasCollision)

                        nLayer++;

                    }

                }


                // Finally we need to reset our world transform to the default
                //ptrPGE->GetDraw().WorldReset();
        }

        

	
    void PlayPlayerSound(uint32_t soundID) {
        
        if(soundID != UINT32_MAX && ptrSound != nullptr)
            ptrSound->PlaySoundAffect(soundID);
    }

    
    
    
};
