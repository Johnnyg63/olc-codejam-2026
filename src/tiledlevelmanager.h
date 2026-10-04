#pragma once
#include "olcUTIL3_Geometry2D.h"
#include "olcPixelGameEngine3.h"
#include "TMXParser.h"
#include "TSXParser.h"
#include <any>
#include <cmath>
#include <numeric>
#include <numbers>
#include <vector>

// TODO: olc::utils ...
namespace olc
{
    /*
        TiledLevelManager is responsible for managing levels created with the Tiled map editor.
        It handles loading TMX and TSX files, storing map and tileset information, and managing
        collision types and custom properties for tiles.
    */
    class TiledLevelManager : public olc::PixelGameEngine
    {

        private:
            bool bisLevelLoaded = false; // Use to stop execution until a level is loaded
            olc::PixelGameEngine* ptrPGE= nullptr; // Pointer to the PixelGameEngine itself,
            // Degrees to radians
            constexpr float DegreesToRadians(float deg) { return deg * std::numbers::pi / 180.0f; }

            // Radians to degrees
            constexpr float RadiansToDegrees(float rad) { return rad * 180.0f / std::numbers::pi; }

            // Calculates the center point of a polygon given its vertices
            constexpr olc::vf2d PolygonCenter(std::vector<olc::vf2d>& points) { return (std::accumulate(points.begin(), points.end(), olc::vf2d{ 0.0f, 0.0f })) / float(points.size()); }

        public:

            Map_TMX map_TMX; // TMXParser Map (TMX Level file example Level1Output.tmx)
            Map_TSX map_TSX; // TSXParser Map (TSX TileSet file example AbstractPlatformer.tsx)

            // Holds the general information about the tiled map
            struct MapInfo
            {
                bool bIsInfinite      = false;  	// Is the layer infinite

                int32_t nWidth        = 0;			// Layer Width
                int32_t nHeight       = 0;			// Layer Height
                int32_t nTileWidth    = 0;			// Layer Width
                int32_t nTileHeight   = 0;			// Layer Height
                int32_t nNextLayerID  = 0;			// The next layer id
                int32_t nNextObjectID = 0;			// The next object id

                std::string strVersion      = "";	// Version
                std::string strTiledVersion = "";	// Tiled Map Version
                std::string strOrientation  = "";	// Orientation
                std::string strRenderorder  = "";	// Order and direction to render (Keep is simple folks use right-down

            };

            // Holds the tiledmap map info
            MapInfo sMapInfo;

            // Holds the location of the tileset used in the map
            struct TileSetLocation
            {
                std::string strFirstgid = "";	// ID
                std::string strScource = "";	// Tiled Map TSX file location
            };

            // Holds the tiledmap TileSet info
            TileSetLocation sTileSetLocation;

            // Holds the detailed information about the tileset used in the map
            struct TileSetInfo
            {
                int32_t nRows      = 0;				    // Number of tile rows in the tile set
                int32_t nColumns   = 0;					// Number of tile columns in the tile set
                int32_t nTileCount = 0;					// Number of tiles in the tile set

                olc::vf2d vfTileSize = { 0.0f, 0.0f };	// Tile Size Width, Height

                std::string strVersion      = "";		// Version
                std::string strTiledVersion = "";		// Tiled Map Version
                std::string strName         = "";		// Class name
                
            };

            TileSetInfo sTileSetInfo;

            // Holds the sprite image information for the tileset
            struct TileSetSpriteImage
            {
                std::string strSource = "";				// Source file
                olc::vf2d vfSize = { 0.0f, 0.0f };		// Size of the sprite image

            };

            TileSetSpriteImage sTileSetSpriteImage;

            // Defines the types of collision shapes available for objects in the map
            enum Collision
            {
                CAPSULE = 0,
                CIRCLE,
                ELLIPSE,
                POINT,
                POLYGON,
                RECT
            };

            // Defines the structure for collision types, including the shape, area, and defining points
            struct CollisionType
            {
                Collision eCollision   = Collision::RECT;	// Collision object type, Default RECT
                float fArea            = 0.0f;				// Area of the collision object, used for sorting collision objects by size desending
                std::vector<olc::vf2d> vecPoints;           // Points defining the shape of the collision object
            };

            struct TypeBool
            {
                std::string name = "";
                bool value       = false;
            };

            // Defines the structure for a Colour (Color) property
            struct TypeColor
            {
                std::string name = "";
                olc::Pixel value = olc::Colour::BLANK;
                // TODO: we probably need to a constructor for initializing the color value to an olc::Colour
            };

            struct TypeInt
            {
                std::string name = "";
                int value        = 0;
            };

            struct TypeFloat
            {
                std::string name = "";
                float value      = 0.0f;
            };

            struct TypeDouble
            {
                std::string name = "";
                double value      = 0.0;
            };

            struct TypeString
            {
                std::string name  = "";
                std::string value = "";
            };

            struct TypeObject
            {
                std::string name = "";
                std::any value;
            };

            struct TypeFile
            {
                std::string name  = "";
                std::string value = "";
                // TODO: Add constructor for initializing the file property std::filestream or similar
            };

            struct TileProperites
            {
                std::vector<TypeBool>	vecBools;
                std::vector<TypeColor>	vecColors;
                std::vector<TypeInt>	vecInts;
                std::vector<TypeFloat>	vecFloats;
                std::vector<TypeDouble>	vecDoubles;
                std::vector<TypeFile>	vecFiles;

                std::vector<TypeString> vecStrings;
                std::vector<TypeObject> vecObjects;
            };

            struct TileObject
            {
                int32_t nTileObjectID    = 0;				// Tile Object ID
                std::string strName      = "NOT_SET";		// Name if passed,default: "NOT_SET"
                std::string strClassType = "NOT_SET";	    // Class type if passed, default: "NOT_SET"
                olc::vf2d vfPosition     = { 0.0f, 0.0f };	// Object Start Poistion X,Y
                olc::vf2d vfSize         = { 0.0f, 0.0f };	// Object Size	Width, Height
                float fRotationDeg       = 0.0f;			// Object Rotation in Degrees
                float fRotationRad       = 0.0f;			// Object Rotation in Radians
                CollisionType sCollisionType;			    // Stores the Collision Type data, RECT, POINT, ELLIPSE, POLYGON, CAPSULE

            };



            struct Tile
            {
                bool bHasCollision        = false;		  // Set if tile has collision
                bool bIsFlag              = false;		  // Set if flag decal
                bool bIsGreenBlock        = false;		  // Set if green block decal
                bool bIsRedBlock          = false;		  // Set if red block decal
                bool bVisiable            = true;		  // Set if tile is visible
                int32_t nColour           = 0;			  // Tile colour
                int32_t nTileID           = 0;			  // Tile ID
                std::string strClassType  = "NOT_SET";	  // Class type if passed, default: "NOT_SET"
                std::string strDrawOrder  = "NOT_SET";	  // Draw Order if passed, default: "NOT_SET"
                int32_t nObjectGroupID    = 0;			  // Object Group ID
                std::vector<TileObject>   vecTileObjects; // Vector of TileObject
                TileProperites Properites;				  // Stores vectors of custom properties
        
            };

            std::vector<Tile> vecTiles;

            struct DecalInfo
            {
                bool bIsVisable          = true;			// Is layer is visable
                bool bIsLocked           = true;			// Is layer is locked
                bool bHasCollision       = false;			// Has collision object 
                int16_t nLayerID         = 0;				// Layer id number
                int32_t nDecalID         = 0;			    // Holds the Tile ID to draw this decal
                int32_t nWidth           = 0;				// Layer Width
                int32_t nHeight          = 0;				// Layer Height
                int32_t nTiledID         = 0;				// Tiled Map Editor ID 
                olc::vf2d vfDrawLocation = { 0.0f, 0.0f };	// Locatoin of where to draw
                olc::vf2d vfSourcePos    = { 0.0f, 0.0f };	// Location on Sprite Sheet
                olc::vf2d vfSoureSizePos = { 0.0f, 0.0f };	// Size of Partial Decal
                std::string strName      = "";				// Layer name
                std::string strClass     = "";				// Layer Class name               
                Tile sCollisionTile;						// Stores the tile collision details
            };

            DecalInfo sDecalInfo;

            /*
            * Stores data required for the Sprite Sheet objects to display correctly
            */
            struct ImageInfo
            {
                olc::vf2d vSource = { 0.0f, 0.0f };     // Source poisition to draw drawing from, default: {0.0f, 0.0f)
                olc::vf2d vSize   = { 1.0f, 1.0f };	    // The size of the image to be drawn, default: Full passed in image size, edit this to when using SpriteSheets to the location of the sprite
                olc::vf2d vScale  = { 1.0f, 1.0f };	    // Scaling factor (Decal Only), default: {1.0, 1.0}, when AutoScale is set this value will be automactically updated
                olc::Pixel pxTint = olc::Colour::WHITE;	// Tint colour for background, set to olc::DARK_GREY for a night time effect
            };

            struct ObjectProperites
            {
                bool bAutoScale                = true;				// Automatically scales the background image to fit within the screen size
                bool bShowCollisions           = false;				// Set to true to show collision lines around objects, default: false

                std::string strName            = "LevelX";			// Object Name. Default "LevelX"
                uint16_t nLevelNumber          = 0;					// Object Number, Default 0 i.e. Backupground 1 , LevelManager 2 etc
                float fMaxSquareTolerance      = 2.0f;					// Maximum pixel difference for approximately square (width ≈ height) used for collision detection (Ellipse v Circle), default: 2.0f
                
                olc::vf2d vfPosition           = { 0.0f,0.0f };		// Image POS {x,y} (float), Default {0.0f,0.0f}
                olc::vi2d viWorldSize          = { 140, 24 };		// 2048 64 cells
                olc::vi2d viTileSize           = { 32, 32 };		// Tile Size (Screen Size 35X35, World Size 1 X 1)
                olc::vi2d viSpriteSheetTiles   = { 28, 14 };	    // Stores the total number of tiles x,y in the sprite sheet (Important!): TODO need to check this

                std::string strSpriteSheetPath = "";				// Sprite path, i.e. "images/mysprite.png", Default: ""
                std::string strTiledMapTMXPath = "";				// Sprite path, i.e. "maps/level.tmx", Default: ""
                olc::Image renSpriteSheet;							// Sprite Sheet Image, loaded from the strSpriteSheetPath, used to draw the level
                std::vector<DecalInfo> vecPartialDecalInfo;			// Stores the DecalInfo struct for tiled map graphics
                std::map<int, std::vector<DecalInfo>> mapLayerInfo; // Stores the layers of DecalInfo vectors

            };

            ObjectProperites Properties;

    public:
        TiledLevelManager()
        {
            // Nothing to do here but to wait until we are ready for the level
            bisLevelLoaded = false;
        }
        ~TiledLevelManager(){}

        void Initialize(olc::PixelGameEngine* pge)
        {
            this->ptrPGE = pge; // Set the pointer to the PGE Window, so we can call PGE methods from this class
        }

        bool LoadLevel(std::string strSpriteSheetPath, std::string strTiledMapTMXPath, uint16_t nLevel)
        {
            
            bool res = false;
            // Load the Sprite Sheet
            res = ptrPGE->CreateImageFromFile(Properties.renSpriteSheet, strSpriteSheetPath);
            
            Properties.strTiledMapTMXPath = strTiledMapTMXPath; // TODO, should we get the fillpath?

            // Load the TMX file
            TMXParser tmxParser = TMXParser(Properties.strTiledMapTMXPath);
            map_TMX = tmxParser.GetData();

            // TileSet Location
            for (auto& tileSetInfo : map_TMX.TilesetData.data)
            {
                if (tileSetInfo.first == "firstgid") { sTileSetLocation.strFirstgid = tileSetInfo.second; continue; }
                if (tileSetInfo.first == "source")
                {
                    sTileSetLocation.strScource = tileSetInfo.second;

                    // Load the tsx file for collections etc
                    if (sTileSetLocation.strScource != "")
                    {
                        // We need to check if the file exist
                        std::filesystem::path filePath(sTileSetLocation.strScource);

                        if (!std::filesystem::exists(filePath)) {

                            std::filesystem::path fileName = filePath.filename();
                            std::filesystem::path TMXFilePath(Properties.strTiledMapTMXPath);
                            std::filesystem::path strvTMXFilePath = TMXFilePath.replace_filename(fileName);

                            if (std::filesystem::exists(strvTMXFilePath))
                                sTileSetLocation.strScource = strvTMXFilePath.string();

                        }

                        TSXParser tsxParser = TSXParser(sTileSetLocation.strScource);
                        map_TSX = tsxParser.GetData();
                    }
                }
            }

            // Get the tileset data
            for (auto& tileSetInfo : map_TSX.TilesetData.data)
            {
                if (tileSetInfo.first == "columns")		 { sTileSetInfo.nColumns        = std::stoi(tileSetInfo.second); continue; }
                if (tileSetInfo.first == "name")		 { sTileSetInfo.strName         = tileSetInfo.second;            continue; }
                if (tileSetInfo.first == "tilecount")	 { sTileSetInfo.nTileCount      = std::stoi(tileSetInfo.second); continue; }
                if (tileSetInfo.first == "tiledversion") { sTileSetInfo.strTiledVersion = tileSetInfo.second;            continue; }
                if (tileSetInfo.first == "tileheight")	 { sTileSetInfo.vfTileSize.y    = std::stof(tileSetInfo.second); continue; }
                if (tileSetInfo.first == "tilewidth")	 { sTileSetInfo.vfTileSize.x    = std::stof(tileSetInfo.second); continue; }
                if (tileSetInfo.first == "version")		 { sTileSetInfo.strVersion      = tileSetInfo.second;            continue; }

            }

            // We can calculate the number of rows in the tile set by dividing the total tile count by the number of columns
            if (sTileSetInfo.nColumns > 0 && sTileSetInfo.nTileCount > 0)
            {
                sTileSetInfo.nRows = sTileSetInfo.nTileCount / sTileSetInfo.nColumns;
            }

            // Get the Sprite image used for the tile set
            for (auto& imageInfo : map_TSX.ImageData.data)
            {
                if (imageInfo.first == "source") { sTileSetSpriteImage.strSource = imageInfo.second;            continue; }
                if (imageInfo.first == "width")  { sTileSetSpriteImage.vfSize.x  = std::stof(imageInfo.second); continue; }
                if (imageInfo.first == "height") { sTileSetSpriteImage.vfSize.y  = std::stof(imageInfo.second); continue; }

            }

            // Get tile information
            for (auto& tileInfo : map_TSX.vecTiles)
            {
                // 1: Create a new struct
                Tile sTile;

                // Get the tile Data
                //  <tile id="236">
                for (auto& tileData : tileInfo.sTileData.data)
                {
                    // Right need to explain this, the TMX CSV data tile ID will always be + 1 greater than 
                    // The tileID held in the TSX data. There is a lot of reasons for this, but for our code to work correctly
                    // we need to ensure our sTile.ID matchs that of which is in the TMX file. 
                    if (tileData.first == "id")   { sTile.nTileID      = (std::stoi(tileData.second) + 1);  continue; }
                    if (tileData.first == "type") { sTile.strClassType = tileData.second;                   continue; }
                }

                // Get the Object Group data
                // <objectgroup draworder="index" id="2">
                for (auto& objectGroupData : tileInfo.sObjectGroupData.data)
                {
                    if (objectGroupData.first == "draworder") { sTile.strDrawOrder   = objectGroupData.second;              continue; }
                    if (objectGroupData.first == "id")        { sTile.nObjectGroupID = std::stoi(objectGroupData.second);   continue; }
                }

                /*
                * <properties>
                *	<property name="IsLadder" type="bool" value="true"/>
                * </properties>
                */
                for (auto& property : tileInfo.vecProperties)
                {
                    std::string sName = "";
                    std::string sType = "";
                    std::string sValue = "";
                    for (auto& data : property.data)
                    {
                        if (data.first == "name")  { sName  = data.second; continue; }
                        if (data.first == "type")  { sType  = data.second; continue; }
                        if (data.first == "value") { sValue = data.second; continue; }
                    }

                    if ((sType == "bool") && (sName == "bHasCollision"))
                    {
                        sTile.bHasCollision = (sValue == "true") ? true : false;
                    }
                    if ((sType == "bool") && (sName == "bIsFlag"))
                    {
                        sTile.bIsFlag = (sValue == "true") ? true : false;
                    }

                    if ((sType == "bool") && (sName == "bIsGreenBlock"))
                    {
                        sTile.bIsGreenBlock = (sValue == "true") ? true : false;
                    }

                    if ((sType == "bool") && (sName == "bIsRedBlock"))
                    {
                        sTile.bIsRedBlock = (sValue == "true") ? true : false;
                    }

                    if ((sType == "bool") && (sName == "bVisiable"))
                    {
                        sTile.bVisiable = (sValue == "true") ? true : false;
                    }

                    if ((sType == "int") && (sName == "nColour"))
                    {
                        sTile.nColour = std::stoi(sValue);
                    }

                    // Ok Create the Property
                    if (sType == "bool")
                    {
                        TypeBool sTypeBool;
                        sTypeBool.name = sName;
                        sTypeBool.value = (sValue == "true") ? true : false; // Implicit conversion from string to bool cause I can...
                        sTile.Properites.vecBools.push_back(sTypeBool);
                        continue;
                    }

                    if (sType == "color")
                    {
                        TypeColor sTypeColor;
                        sTypeColor.name = sName;

                        // remove the leading # if it exist
                        char firstChar = sValue.at(0);
                        if (firstChar == '#')
                            sValue.erase(0, 1);


                        sTypeColor.value = olc::Pixel(std::stoul(sValue, nullptr, 16));
                        sTile.Properites.vecColors.push_back(sTypeColor);
                        continue;
                    }

                    if (sType == "file")
                    {
                        TypeFile sTypeFile;
                        sTypeFile.name  = sName;
                        sTypeFile.value = sValue;
                        sTile.Properites.vecFiles.push_back(sTypeFile);
                        continue;
                    }

                    if (sType == "int")
                    {
                        TypeInt sTypeInt;
                        sTypeInt.name = sName;
                        sTypeInt.value = std::stoi(sValue);
                        sTile.Properites.vecInts.push_back(sTypeInt);
                        continue;
                    }

                    if (sType == "float")
                    {
                        TypeFloat sTypeFloat;
                        sTypeFloat.name = sName;
                        sTypeFloat.value = std::stof(sValue);
                        sTile.Properites.vecFloats.push_back(sTypeFloat);
                        continue;
                    }

                    if (sType == "double")
                    {
                        TypeDouble sTypeDouble;
                        sTypeDouble.name = sName;
                        sTypeDouble.value = std::stod(sValue);
                        sTile.Properites.vecDoubles.push_back(sTypeDouble);
                        continue;
                    }


                    if (sType == "object")
                    {
                        TypeObject sTypeObject;
                        sTypeObject.name = sName;
                        sTypeObject.value = sValue;
                        sTile.Properites.vecObjects.push_back(sTypeObject);
                        continue;
                    }

                    // If it is not any of the above it is a string or unknown type we just record it as a string
                    // and let the developer decide what to do with it
                    TypeString sTypeString;
                    sTypeString.name = sName;
                    sTypeString.value = sValue;
                    sTile.Properites.vecStrings.push_back(sTypeString);

                }




                // Get the Object data:
                /*
                *   <object id="1" name="Left_Triangle" type="clsLeftTriangle" x="0.176258" y="9.51793">
                *		<polygon points="0,0 4.51712,-9.51793 6.69781,0.352516"/>
                *   </object>
                *	<object id="2" name="Right_Triangle" type="clsRightTriangle" x="27.4963" y="9.87045">
                *		<polygon points="0,0 4.44713,-9.69419 7.22658,-0.528774"/>
                *   </object>
                *   <object id="3" x="13.5719" y="0.705032" width="7.22658" height="7.93161"/>
                */
                for (auto& sObjectDataInfo : tileInfo.vecObjectDataInfo)
                {
                    // Defaults
                    TileObject sTileObject;
                    sTileObject.sCollisionType.eCollision = Collision::RECT;

                    // <object id="1" name="Left_Triangle" type="clsLeftTriangle" x="0.176258" y="9.51793">
                    for (auto& objectData : sObjectDataInfo.sObjectData.data)
                    {
                        if (objectData.first == "height") { sTileObject.vfSize.y      = std::stof(objectData.second);   continue; }
                        if (objectData.first == "id")     { sTileObject.nTileObjectID = std::stoi(objectData.second);   continue; }
                        if (objectData.first == "name")   { sTileObject.strName       = objectData.second;              continue; }

                        if (objectData.first == "rotation")
                        {
                            sTileObject.fRotationDeg = std::stof(objectData.second);
                            // Get our radians 
                            sTileObject.fRotationRad = DegreesToRadians(sTileObject.fRotationDeg);
                            continue;
                        }
                        if (objectData.first == "type")  { sTileObject.strClassType = objectData.second;            continue; }
                        if (objectData.first == "width") { sTileObject.vfSize.x     = std::stof(objectData.second); continue; }
                        if (objectData.first == "x")     { sTileObject.vfPosition.x = std::stof(objectData.second); continue; }
                        if (objectData.first == "y")     { sTileObject.vfPosition.y = std::stof(objectData.second); continue; }

                    }

                    if (sObjectDataInfo.sTypeData.tag == "ellipse") sTileObject.sCollisionType.eCollision = Collision::ELLIPSE;
                    if (sObjectDataInfo.sTypeData.tag == "point")   sTileObject.sCollisionType.eCollision = Collision::POINT;
                    if (sObjectDataInfo.sTypeData.tag == "polygon") sTileObject.sCollisionType.eCollision = Collision::POLYGON;
                    if (sObjectDataInfo.sTypeData.tag == "rect")    sTileObject.sCollisionType.eCollision = Collision::RECT;
                    if (sObjectDataInfo.sTypeData.tag == "capsule") sTileObject.sCollisionType.eCollision = Collision::CAPSULE;


                    // <polygon points = "0,0 4.51712,-9.51793 6.69781,0.352516" / >
                    for (auto& typeData : sObjectDataInfo.sTypeData.data)
                    {
                        /*
                        * Lets parse out our points to string --> olc::vf2d
                        * example: -0.666667,32.6667 (x, y)
                        * As we are dealing with data we use typeData.second as this is where the data is stored
                        */
                        std::string strX =  typeData.second.substr(0, typeData.second.find(","));
                        std::string strY =  typeData.second.substr(typeData.second.find(",") + 1, std::string::npos);
                        olc::vf2d vfPoint = { 0.0f, 0.0f };
                        vfPoint.x =			std::stof(strX);
                        vfPoint.y =			std::stof(strY);
                        sTileObject.sCollisionType.vecPoints.push_back(vfPoint);

                    }

                    switch (sTileObject.sCollisionType.eCollision)
                    {
                    case Collision::ELLIPSE:
                    {
                        // The user needs to make sure the width and height are the same, 
                        // we need to check if they are close enough and if so we can treat it as a circle
                        if (std::abs(sTileObject.vfSize.x - sTileObject.vfSize.y) < Properties.fMaxSquareTolerance) // Approximately square (width ≈ height) default 2.0f pixels
                        {
                            sTileObject.sCollisionType.eCollision = Collision::CIRCLE;
                            // ok now we need to make vfSize.x and y = smallest of the two, as we want to treat it as a circle
                            float fMinSize = std::min(sTileObject.vfSize.x, sTileObject.vfSize.y);
                            sTileObject.vfSize = { fMinSize, fMinSize };
                            sTileObject.sCollisionType.fArea = std::numbers::pi_v<float> *(fMinSize / 2.0f) * (fMinSize / 2.0f); // Area of a circle A = πr^2 where r = diameter / 2
                        }
                        else
                        {
                            // Ellipse, it is best just change it to polygon, makes everything later easier
                            // We can approximate the ellipse with a polygon, the more points we use the better the approximation,
                            const float fMinPoints = 16.0f;
                            // Get the rx / ry of the ellipse, and centre of ellipse
                            olc::vf2d rXrY = sTileObject.vfSize * 0.5f;
                            olc::vf2d vfCenter = sTileObject.vfPosition + sTileObject.vfSize * 0.5f;

                            // Get rough circumference of the ellipse:  C ≈ π * sqrt( 2 * (rx² + ry²) )
                            float fCir = std::numbers::pi_v<float> *std::sqrt(2.0f * (rXrY.x * rXrY.x + rXrY.y * rXrY.y));

                            // Get the maximum number of points based on the circumference (2 π * r) we want to have a point every 2 - 3 pixels approx,
                            float fMaxPoints = fCir / (std::numbers::pi_v<float> *2.0f);

                            // Ensure we have at least the minimum number of points
                            if (fMinPoints > fMaxPoints) fMaxPoints = fMinPoints;

                            float fStep = fCir / fMaxPoints;	// Get the step size based on the circumference

                            sTileObject.sCollisionType.vecPoints.clear();
                            sTileObject.sCollisionType.vecPoints.reserve((size_t)std::ceil(fMaxPoints));
                            sTileObject.sCollisionType.eCollision = Collision::POLYGON;

                            // ds/dt for ellipse: sqrt( (rx*sin(t))^2 + (ry*cos(t))^2 )
                            auto dsdt = [](float rx, float ry, float t)
                                {
                                    float dx = -rx * std::sin(t);
                                    float dy = ry * std::cos(t);
                                    return std::sqrt(dx * dx + dy * dy);
                                };


                            float accumulated = 0.0f;
                            float fpa = 0.0f, fpx = 0.0f, fpy = 0.0f;
                            for (int i = 0; i < (int32_t)fMaxPoints; i++)
                            {
                                float target = i * fStep;

                                // Move forward in small dt steps, until arc length equals target
                                while (accumulated < target)
                                {
                                    constexpr float dt = 0.001f;
                                    accumulated += dsdt(rXrY.x, rXrY.y, fpa) * dt;
                                    fpa += dt;
                                }

                                fpx = vfCenter.x + rXrY.x * std::cos(fpa);
                                fpy = vfCenter.y + rXrY.y * std::sin(fpa);

                                sTileObject.sCollisionType.vecPoints.push_back({ fpx, fpy });
                            }

                            // We set vfPosition to {0.0f, 0.0f} as we have already applied the position to our points
                            sTileObject.vfPosition = { 0.0f, 0.0f };
                        }
                        break;
                    }
                    case Collision::POINT: { break; }
                    case Collision::CAPSULE:
                    {
                        // The user needs to make sure the width and height are the same, 
                        // we need to check if they are close enough and if so we can treat it as a circle
                        if (std::abs(sTileObject.vfSize.x - sTileObject.vfSize.y) < Properties.fMaxSquareTolerance) // Approximately square (width ≈ height) default 2.0f pixels
                        {
                            sTileObject.sCollisionType.eCollision = Collision::CIRCLE;
                            // ok now we need to make vfSize.x and y = smallest of the two, as we want to treat it as a circle
                            float fMinSize = std::min(sTileObject.vfSize.x, sTileObject.vfSize.y);
                            sTileObject.vfSize = { fMinSize, fMinSize };
                            sTileObject.sCollisionType.fArea = std::numbers::pi_v<float> *(fMinSize / 2.0f) * (fMinSize / 2.0f); // Area of a circle A = πr^2 where r = diameter / 2
                        }
                        else
                        {

                            // We need to calculate the points for the capsule shape based on the position and size of the object
                            sTileObject.sCollisionType.eCollision = Collision::POLYGON;

                            int nSegments = 16;
                            sTileObject.sCollisionType.vecPoints.clear();
                            sTileObject.sCollisionType.vecPoints.reserve(4 + nSegments);
                            
                            // we need to check if we are x or y dominant to know which way our capsule is facing
                            bool bYDominant = sTileObject.vfSize.y > sTileObject.vfSize.x;

                            if (bYDominant)
                            {
                                // we need to rotate our points by 90 degrees to get the correct orientation of the capsule
                                // we need to swap width and height for our calculations as we are treating it as x dominant and then we will rotate the points later
                                std::swap(sTileObject.vfSize.x, sTileObject.vfSize.y);

                            }


                            // get the centre of the capsule
                            olc::vf2d vfCenter = sTileObject.vfSize * 0.5f;

                            // get the radius of the circles (half the height of the capsule)
                            float fRadius = std::min(sTileObject.vfSize.x, sTileObject.vfSize.y) / 2.0f;

                            // get our circumference of the circles: C = 2 π r
                            float fCir = 2.0f * std::numbers::pi_v<float> *fRadius;

                            // get the maximum number of points based on the circumference (2 π * r) we want to have a point every 2 - 3 pixels approx,
                            float fMaxPoints = fCir / (std::numbers::pi_v<float> *2.0f);

                            // Ensure we have at least the minimum number of points
                            if (fMaxPoints > (float)nSegments) nSegments = std::floor(fMaxPoints);

                            // get the size of the inner rectangle (size of capsule - diameter of the circles)
                            olc::vf2d vfInnerSize = olc::vf2d{ sTileObject.vfSize.x - fRadius * 2.0f, sTileObject.vfSize.y }; // : olc::vf2d{ sTileObject.vfSize.x, sTileObject.vfSize.y - fRadius * 2.0f };

                            // get the four corners of the inner rectangle
                            olc::vf2d vfTopLeft     = vfCenter - olc::vf2d{ vfInnerSize.x / 2.0f, vfInnerSize.y / 2.0f };
                            olc::vf2d vfTopRight    = vfTopLeft + olc::vf2d{ vfInnerSize.x, 0.0f };
                            olc::vf2d vfBottomLeft  = vfTopLeft + olc::vf2d{ 0.0f, vfInnerSize.y };
                            olc::vf2d vfBottomRight = vfTopLeft + vfInnerSize;

                            // get the points for the semicircles on either end
                            float dtheta = 2.0f * std::numbers::pi_v<float> / nSegments;

                            // Left semicircle
                            vfCenter = vfTopLeft + olc::vf2d{ 0.0f, fRadius };
                            float fpx, fpy, fpa;
                            for (int i = 0; i <= nSegments; i++)
                            {
                                fpa = i * dtheta;
                                fpx = vfCenter.x + fRadius * std::cos(fpa);
                                fpy = vfCenter.y + fRadius * std::sin(fpa);
                                sTileObject.sCollisionType.vecPoints.push_back({ fpx, fpy });
                            }

                                
                            // Right semicircle
                            vfCenter = vfTopRight + olc::vf2d{ 0.0f, fRadius };
                            for (int i = 0; i < nSegments; i++)
                            {
                                fpa = i * dtheta;
                                fpx = vfCenter.x + fRadius * std::cos(fpa);
                                fpy = vfCenter.y + fRadius * std::sin(fpa);
                                sTileObject.sCollisionType.vecPoints.push_back({ fpx, fpy });
                            }

                            // a little hack to ensure the secound circle i
                            fpx = vfCenter.x + fRadius * std::cos(0);;
                            fpy = vfCenter.y + fRadius * std::sin(0);
                            sTileObject.sCollisionType.vecPoints.push_back({ fpx, fpy });
                            vfCenter = sTileObject.vfSize * 0.5f;
                            sTileObject.sCollisionType.vecPoints.push_back(vfCenter);

                            // Draw our rect
                            sTileObject.sCollisionType.vecPoints.push_back(vfTopLeft);
                            sTileObject.sCollisionType.vecPoints.push_back(vfTopRight);
                            sTileObject.sCollisionType.vecPoints.push_back(vfBottomRight);
                            sTileObject.sCollisionType.vecPoints.push_back(vfBottomLeft);
                            sTileObject.sCollisionType.vecPoints.push_back(vfTopLeft);

                            // If we are y dominant we need to rotate our points back by 90 degrees to get the correct orientation of the capsule
                            if (bYDominant)
                            {
                                for (auto& point : sTileObject.sCollisionType.vecPoints)
                                {
                                    float x = point.x;
                                    point.x = point.y;
                                    point.y = x;
                                }
                            }

                        }


                        

                    }
                    case Collision::RECT:
                        // For these collision types we can just push the object into our tile struct
                        
                        sTileObject.sCollisionType.fArea = sTileObject.vfSize.x * sTileObject.vfSize.y; // Area of a rectangle A = width * height

                        if (sTileObject.fRotationRad != 0.0f)
                        {
                            // For a rect we need to convert to triangles (polgyon) as it makes out collision code easier to work with
                            // Get the 4 points of the rect
                            olc::vf2d vfTopLeft     = sTileObject.vfPosition;
                            olc::vf2d vfTopRight    = sTileObject.vfPosition + olc::vf2d{ sTileObject.vfSize.x, 0.0f };
                            olc::vf2d vfBottomLeft  = sTileObject.vfPosition + olc::vf2d{ 0.0f, sTileObject.vfSize.y };
                            olc::vf2d vfBottomRight = sTileObject.vfPosition + sTileObject.vfSize;

                            // Now we need to convert into Polgon
                            sTileObject.sCollisionType.eCollision = Collision::POLYGON;

                            sTileObject.sCollisionType.vecPoints.push_back(vfTopLeft);
                            sTileObject.sCollisionType.vecPoints.push_back(vfTopRight);
                            sTileObject.sCollisionType.vecPoints.push_back(vfBottomRight);
                            sTileObject.sCollisionType.vecPoints.push_back(vfBottomLeft);
                            sTileObject.sCollisionType.vecPoints.push_back(vfTopLeft);
                        }
                        break;
                    case Collision::POLYGON:
                    default:
                        break;
                    }
                    
                    if (sTileObject.sCollisionType.eCollision == Collision::POLYGON)
                    {
                        // If we have a polygon we need to calculate the area using the shoelace formula: A = 0.5 * |Σ(x_i*y_{i+1} - x_{i+1}*y_i)|
                        for (size_t i = 0; i < sTileObject.sCollisionType.vecPoints.size() - 1; i++)
                        {
                            sTileObject.sCollisionType.fArea += std::abs(0.5f * (sTileObject.sCollisionType.vecPoints[i].x * sTileObject.sCollisionType.vecPoints[i + 1].y - sTileObject.sCollisionType.vecPoints[i + 1].x * sTileObject.sCollisionType.vecPoints[i].y));
                        }
                    }
                    
                    sTile.vecTileObjects.push_back(sTileObject);


                } // END: for (auto& sObjectDataInfo : tileInfo.vecObjectDataInfo)

                // ok Before we can finsh we need to sort our collision points by area descending, to ensure a smooth transition between collision sizes.
                std::sort(sTile.vecTileObjects.begin(), sTile.vecTileObjects.end(), [](const TileObject& a, const TileObject& b)
                    {
                        return a.sCollisionType.fArea > b.sCollisionType.fArea;
                    });

                vecTiles.push_back(sTile); // Push back the tile info for this decal

            }

            // Get TMX Map information
            for (auto& mapInfo : map_TMX.MapData.data)
            {
                // Note Order is important as it follows the TMX map structure: height, infinite, nextlayerid, etc, therefore increase performance...

                if (mapInfo.first == "height")       { sMapInfo.nHeight         = std::stoi(mapInfo.second); continue; }
                if (mapInfo.first == "infinite")     { sMapInfo.bIsInfinite     = (std::stoi(mapInfo.second) > 0) ? true : false; continue; }
                if (mapInfo.first == "nextlayerid")  { sMapInfo.nNextLayerID    = std::stoi(mapInfo.second);  continue; }
                if (mapInfo.first == "nextobjectid") { sMapInfo.nNextObjectID   = std::stoi(mapInfo.second);  continue; }
                if (mapInfo.first == "orientation")  { sMapInfo.strOrientation  = mapInfo.second;             continue; }
                if (mapInfo.first == "renderorder")  { sMapInfo.strRenderorder  = mapInfo.second;             continue; }
                if (mapInfo.first == "tiledversion") { sMapInfo.strTiledVersion = mapInfo.second;             continue; }
                if (mapInfo.first == "tileheight")   { sMapInfo.nTileHeight     = std::stoi(mapInfo.second);  continue; }
                if (mapInfo.first == "tilewidth")    { sMapInfo.nTileWidth      = std::stoi(mapInfo.second);  continue; }
                if (mapInfo.first == "version")      { sMapInfo.strVersion       = mapInfo.second;            continue; }
                if (mapInfo.first == "width")        { sMapInfo.nWidth           = std::stoi(mapInfo.second); continue;}
                
            }

            // lets clear things up
            Properties.vecPartialDecalInfo.clear();
            int16_t nLayerCount = 0;
            int32_t x           = 0;
            int32_t y           = 0;

            // Important this is needed to ensure the DrawPartialDecal correctly finds the location with the spritesheet
            int32_t nSpriteSheetTileCount = Properties.renSpriteSheet.Size().x / sMapInfo.nTileWidth;

            int32_t nIDs = 0;

            // Get the layer information and create our DecalInfo struct for each tile in the layer
            for (auto& layer : map_TMX.LayerData)
            {
                auto vecPartialDecalInfo = std::vector<DecalInfo>();
                auto rowYtiles = layer.tiles;

                nIDs = 0; // Reset the Tile ID per layer
                DecalInfo sDecalInfoLayerDefaults; // We can use this to set default values for the layer,

                for (auto& tag : layer.tag.data)
                {
                    // Order is important
                    if (tag.first == "class")   { sDecalInfoLayerDefaults.strName    = tag.second;            continue; }
                    if (tag.first == "height")  { sDecalInfoLayerDefaults.nHeight    = std::stoi(tag.second); continue; }
                    if (tag.first == "id")      { sDecalInfoLayerDefaults.nLayerID   = std::stoi(tag.second); continue; }
                    if (tag.first == "locked")  { sDecalInfoLayerDefaults.bIsLocked  = (std::stoi(tag.second) > 0) ? true : false; continue; }
                    if (tag.first == "name")    { sDecalInfoLayerDefaults.strName    = tag.second;            continue; }
                    if (tag.first == "visable") { sDecalInfoLayerDefaults.bIsVisable = (std::stoi(tag.second) > 0) ? true : false; continue; }
                    if (tag.first == "width")   { sDecalInfoLayerDefaults.nWidth     = std::stoi(tag.second); continue; }
                    
                }


                for (auto& tiles : rowYtiles)
                {
                    x = 0;
                    for (auto& tile : tiles)
                    {
                        DecalInfo sDecalInfo;
                        sDecalInfo.nDecalID = nIDs;

                        int tileId = tile;

                        float spriteX = float(x * sMapInfo.nTileWidth);
                        float spriteY = float(y * sMapInfo.nTileHeight);

                        sDecalInfo.nTiledID = tileId;
                        sDecalInfo.vfDrawLocation = { spriteX , spriteY };
                        sDecalInfo.vfSoureSizePos = { (float)sMapInfo.nTileWidth, (float)sMapInfo.nTileHeight };

                        // Layer defaults, we can override these with the tile info if we want to
                        sDecalInfo.strName    = sDecalInfoLayerDefaults.strName;
                        sDecalInfo.nHeight    = sDecalInfoLayerDefaults.nHeight;
                        sDecalInfo.nLayerID   = sDecalInfoLayerDefaults.nLayerID;
                        sDecalInfo.bIsLocked  = sDecalInfoLayerDefaults.bIsLocked;
                        sDecalInfo.nWidth     = sDecalInfoLayerDefaults.nWidth;
                        sDecalInfo.bIsVisable = sDecalInfoLayerDefaults.bIsVisable;

                        if (tileId > 0)
                        {
                            // Draw something
                            int tileX = (tileId - 1) % nSpriteSheetTileCount; // Number of X tiles Johnngy!!!!... number of tiles on the SpriteSheet!
                            int tileY = (tileId - 1) / nSpriteSheetTileCount;

                            float sourceX = tileX * sTileSetInfo.vfTileSize.x; // sMapInfo.nTileWidth;
                            float sourceY = tileY * sTileSetInfo.vfTileSize.y; // sMapInfo.nTileHeight;

                            for (auto& sTile : vecTiles)
                            {
                                if (sDecalInfo.nTiledID == sTile.nTileID)
                                {
                                    sDecalInfo.bHasCollision = true;
                                    sDecalInfo.sCollisionTile = sTile;
                                    break;
                                }
                            }

                            sDecalInfo.vfSourcePos = { sourceX , sourceY };

                        }

                        vecPartialDecalInfo.push_back(sDecalInfo);

                        x++;
                        nIDs++;
                    }

                    y++;
                }

                Properties.mapLayerInfo.insert({ nLayerCount, vecPartialDecalInfo });
                nLayerCount++;

            }

            bisLevelLoaded = res; // the level is only loaded when no issues occured
            return res;
        }

        /**
         * Rotates a point around a center by a given angle in radians.
         * @param vfCenterPos The center position to rotate around.
         * @param fRadians The angle in radians to rotate.
         * @param vfPoint The point to rotate.
         * @return The rotated point.
         */
        olc::vf2d RotatePoint(olc::vf2d vfCenterPos, float fRadians, olc::vf2d vfPoint)
        {
            float tempX = vfPoint.x - vfCenterPos.x;
            float tempY = vfPoint.y - vfCenterPos.y;
            vfPoint.x = vfCenterPos.x + (tempX * cos(fRadians) - tempY * sin(fRadians));
            vfPoint.y = vfCenterPos.y + (tempX * sin(fRadians) + tempY * cos(fRadians));
            return vfPoint;
        }

        void ClearLevel()
        {
            // TODO, tidy up any additional resources if necessary
            Properties.renSpriteSheet.GetPixels().clear();

        }

        /**
         * Displays the level on the screen.
         * @param fElapsedTime The elapsed time since the last frame.
         */
        void DisplayLevel(float fElapsedTime)
        {
            
            // Displays the level
            // tile offsets and counts
            olc::vi2d vTileOffset = ptrPGE->GetDraw().ScreenToWorld({0,0}).floor();
            olc::vi2d vTileCount  = ptrPGE->GetDraw().ScreenToWorld(ptrPGE->ScreenSize()).ceil() - vTileOffset;

            // Clamp to ensure we stay in bounds of our world map
            olc::vi2d vTileTL = vTileOffset.max({ 0,0 });
            olc::vi2d vTileBR = (vTileOffset + vTileCount).min(Properties.viWorldSize);
            olc::vi2d vTile;

            // Layer stuff
            using namespace olc::utils::geom2d;
            int32_t idx         = 0;
            int32_t nLayerCount = 0;
            olc::vf2d vfDirection = { 0.0f, 0.0f };
            DecalInfo decalInfo;
        

            // Screen Tile Position
            olc::vf2d vfScreenTilePos  = { 0.0f, 0.0f };
            olc::vf2d vfScreenTileSize = { 0.0f, 0.0f };

            // Collision resizing
            olc::vf2d vfCollsionSize = { 0.0f, 0.0f };
            olc::vf2d vfOffSet       = { 0.0f, 0.0f };

            std::vector <olc::vf2d> vfPolyPoints;
            std::vector <olc::vf2d> vfEmptyPoints;

            // Then looping through them and drawing them
            olc::ImageBatch imgBatch = ptrPGE->GetDraw().CreateImageBatch(Properties.renSpriteSheet);
            olc::LineBatch lineBatch = ptrPGE->GetDraw().CreateLineBatch();

            // Then looping through them and drawing them (TODO: We need to optimize this for large levels, SIMD or threading might help)
            for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
                for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
                {
                    idx = vTile.y * Properties.viWorldSize.x + vTile.x;
                    nLayerCount = 0;

                    for (auto& layer : Properties.mapLayerInfo)
                    {
                        decalInfo = layer.second[idx];
                        if (decalInfo.nTiledID == 0) continue; // If the tile does nothing just move on

                        if (decalInfo.bHasCollision  && Properties.bShowCollisions)
                        {
                            // NOTE: We are in world space so we need to get realworld....
                            for (auto& tileObject : decalInfo.sCollisionTile.vecTileObjects)
                            {
                                vfOffSet = tileObject.vfPosition / Properties.viTileSize;
                                vfCollsionSize = tileObject.vfSize / Properties.viTileSize;

                                switch (tileObject.sCollisionType.eCollision)
                                {
                                
                                    case Collision::CAPSULE:
                                    {
                                        ptrPGE->GetDraw().RoundedRect(lineBatch, olc::vf2d{ vTile + vfOffSet }, vfCollsionSize, 
                                                                        std::min(vfCollsionSize.x / 2, vfCollsionSize.y / 2), olc::Colour::CYAN);
                                        break;
                                    }
                                    case Collision::CIRCLE:
                                    {
                                        
                                        olc::vf2d vfCenter = vTile + vfOffSet + vfCollsionSize / 2.0f;
                                        if (tileObject.fRotationRad != 0.0f)
                                        {
                                            vfCenter = RotatePoint(vTile + vfOffSet, tileObject.fRotationRad, vfCenter);
                                        }
                                        ptrPGE->GetDraw().Circle(lineBatch, vfCenter, vfCollsionSize.x / 2.0f, olc::Colour::YELLOW);
                                        break;
                                    }
                                    case Collision::ELLIPSE:
                                    {
                                        olc::vf2d vfCenter = vTile + vfOffSet + vfCollsionSize / 2.0f;
                                        if (tileObject.fRotationRad != 0.0f)
                                        {
                                            vfCenter = RotatePoint(vTile + vfOffSet, tileObject.fRotationRad, vfCenter);
                                        }
                                        ptrPGE->GetDraw().Ellipse(lineBatch, vfCenter, vfCollsionSize.x / 2.0f, vfCollsionSize.y / 2.0f, olc::Colour::TANGERINE);
                                        break;
                                    }
                                    case Collision::POINT:
                                    {
                                        // this is a point collision, we can draw it as a small rect for now
                                        ptrPGE->GetDraw().Rect(lineBatch, olc::vf2d{ vTile + vfOffSet }, { 1.0f, 1.0f }, olc::Colour::BLACK);
                                        break;
                                    }
                                    case Collision::POLYGON:
                                    {

                                        for (auto& vfPoint : tileObject.sCollisionType.vecPoints)
                                        {
                                            // check for any rotation and rotate the points if needed
                                            auto vfRotatedPoint = tileObject.vfPosition;
                                            auto vfPosition = tileObject.vfPosition;
                                            if (tileObject.fRotationRad != 0.0f)
                                            {
                                                vfPoint = RotatePoint(vfRotatedPoint, tileObject.fRotationRad, vfPoint);
                                                vfPosition = { 0.0f, 0.0f };
                                            }
                                            olc::vf2d vfWorldPoint = (vfPoint + vfPosition) / Properties.viTileSize;
                                            olc::vf2d vfPointnew = vTile + vfWorldPoint;
                                            vfPolyPoints.push_back(vfPointnew);
                                            // TODO: what was this colour for again?
                                            olc::vf2d vfColour = { 0.0f, 0.0f };
                                            vfEmptyPoints.push_back(vfColour);
                                        }

                                        auto vfCenter = PolygonCenter(vfPolyPoints);
                                        
                                        // Draw triangle fan for the polygon from center to points
                                        for (size_t i = 0; i < vfPolyPoints.size(); i++)
                                        {
                                            ptrPGE->GetDraw().Triangle(lineBatch, 
                                                                        vfCenter, 
                                                                        vfPolyPoints[i], 
                                                                        vfPolyPoints[(i + 1) % vfPolyPoints.size()],
                                                                        olc::Colour::DARK_GREEN);
                                        }

                                        vfPolyPoints.clear();
                                        vfEmptyPoints.clear();

                                        break;
                                    }
                                    case Collision::RECT:
                                    {
                                        ptrPGE->GetDraw().Rect(lineBatch, olc::vf2d{ vTile + vfOffSet }, vfCollsionSize, olc::Colour::RED);
                                        break;
                                    }
                        
                                    default:
                                    { 
                                        break;
                                    }
                                        
                                }
                            }

                        }

                        // TODO we need to update this 
                        switch (decalInfo.nLayerID)
                        {
                        case 2:
                        case 3:
                        {
                           
                            break;
                        }
                        default:
                            break;
                        }
                        // this is our drawing layer
                        vfScreenTilePos  = ptrPGE->GetDraw().WorldToScreen(vTile);
                        vfScreenTileSize = ptrPGE->GetDraw().ScreenToWorld(decalInfo.vfSoureSizePos);
                        if(decalInfo.sCollisionTile.bIsGreenBlock == false && decalInfo.sCollisionTile.bIsRedBlock == false)
                        {
                            ptrPGE->GetDraw().ImageRect(imgBatch, Properties.renSpriteSheet.region(decalInfo.vfSourcePos, decalInfo.vfSoureSizePos), olc::vf2d(vTile), { 1.0f, 1.0f }, olc::Colour::WHITE);
                        }
                        
                        nLayerCount++;

                    }

                    

                } // End for Loop vtiles

            ptrPGE->GetDraw().Batch(imgBatch);
            ptrPGE->GetDraw().Batch(lineBatch);

        }


    }; // End class LevelManager

}; // End namespace olc


