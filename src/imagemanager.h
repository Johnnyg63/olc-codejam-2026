#pragma once
#include "olcPixelGameEngine3.h"
#include "olcPGEX3_Miniaudio.h"
#include <algorithm>


struct loadImage{
    std::string strName;             // Name of the Image
    std::string strPath;             // Path to the image file
};


class ImageManager : public olc::PixelGameEngine {

    private:
    uint32_t uniqueIDCounter = 0;       // Counter to generate unique IDs for sounds

    struct ImageProp{
        uint32_t id;                                 // Unique identifier for the image (Automatically assigned)
        bool bIsLoaded = false;                      // Indicates if the image has been loaded
        std::string strName;                         // Name of the image
        olc::Image* pImage;                          // Pointer to the image object
    };

    std::vector<ImageProp> vecImages; // Container to store all loaded images


public:

    ImageManager() {}
    ~ImageManager() {}

    void Initialize(olc::PixelGameEngine* engine) {
        ptrPGE = engine;
    }

    uint32_t LoadImages(std::vector<loadImage> vecLoadImages, bool bReset = false)
    {
        int res = -1;
        if(bReset)
        {
            for(auto& image : this->vecImages){
                if(image.pImage) {
                    delete image.pImage;
                }
            }
            this->vecImages.clear();
            uniqueIDCounter = 0;
            res = 0;
        }
        
        for(const auto& image : vecLoadImages)
        {
            olc::Image pNewImage;
            bool bLoaded = ptrPGE->CreateImageFromFile(pNewImage, image.strPath);
            
            if(bLoaded)
            {
                ImageProp newImage {
                    .id        = uniqueIDCounter++,
                    .bIsLoaded = true,
                    .strName   = image.strName,
                    .pImage    = &pNewImage
                };
        
                this->vecImages.push_back(newImage);
                res++;
            }
          
        }

        return res;
    };

    uint32_t GetImageIDByName(const std::string& imageName) const
    {
        auto it = std::find_if(vecImages.begin(), vecImages.end(), [&imageName](const ImageProp& image) {
            return image.strName == imageName;
        });
        
        if(it != vecImages.end())
        {
            return it->id;  // Return the image's ID
        }
        
        return UINT32_MAX;  // Return invalid ID if not found
    }

    olc::Image* GetImageByID(uint32_t imageID) const
    {
        auto it = std::find_if(vecImages.begin(), vecImages.end(), [&imageID](const ImageProp& image) {
            return image.id == imageID;
        });

        if(it != vecImages.end())
        {
            return it->pImage;  // Return the image pointer
        }

        return nullptr;  // Return nullptr if not found
    }

    void DrawImageByID(uint32_t imageID, olc::vf2d position, olc::vf2d size = olc::vf2d(0,0))
    {
        
        for(auto& image : vecImages)
        {
            if(image.id == imageID && image.pImage && ptrPGE)
            {
                ptrPGE->GetDraw().ImageRect(*image.pImage, position, image.pImage->Size(), olc::Colour::WHITE);
                break;
            }
        }

    }

    private:
        olc::PixelGameEngine* ptrPGE = nullptr;


};


