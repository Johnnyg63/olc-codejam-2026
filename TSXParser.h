#pragma once

#include <fstream>
#include <strstream>
#include <execution>
#include <map>
#include <iostream>

using namespace olc;

/**
 * XMLTag_TSX represents a single XML tag in a TSX file.
 * It contains the tag name and a map of key-value pairs for the tag's attributes.
 */
struct XMLTag_TSX
{
	std::string tag;							// The name of the XML tag.
	std::map<std::string, std::string> data;	// A map of key-value pairs for the tag's attributes.

	/*
	* Formats the tag data into a human-readable string.
	* @param tiles The map of key-value pairs to format.
	* @return A formatted string representing the tag data.
	*/
	const std::string FormatTagData(std::map<std::string, std::string>tiles)
	{
		std::string displayStr = "";

		for (std::map<std::string, std::string>::iterator it = tiles.begin(); it != tiles.end(); it++)
		{
			displayStr += "  " + it->first + ": " + it->second + "\n";
		}

		return displayStr;
	}

	/**
	 * Overloads the << operator to print the XMLTag_TSX object.
	 * @param os The output stream.
	 * @param rhs The XMLTag_TSX object to print.
	 * @return The output stream.
	 */
	friend std::ostream& operator << (std::ostream& os, XMLTag_TSX& rhs)
	{
		os << rhs.tag << "\n" << rhs.FormatTagData(rhs.data) << "\n";

		return os;
	}

	// Retrieves the integer value associated with the specified data tag.
	int GetInteger(std::string dataTag)
	{
		return std::stoi(data[dataTag]);
	}

	// Retrieves the double value associated with the specified data tag.
	double GetDouble(std::string dataTag)
	{
		return std::stod(data[dataTag]);
	}

	// Retrieves the boolean value associated with the specified data tag.
	bool GetBool(std::string dataTag)
	{
		if (data[dataTag] == "0")
		{
			return false;
		}
		else
		{
			return true;
		}

	}

};

/**
 * ObjectDataInfo represents the data for an object in a TSX file.
 * It contains the XML tag for the object data and the type data.
 */
struct ObjectDataInfo
{
	XMLTag_TSX sObjectData;	// The XML tag for the object data.
	XMLTag_TSX sTypeData;	// The XML tag for the type data.
};


/**
 * Tile represents a single tile in a TSX file.
 * It contains the tile data, any custom properties, and object group data.
 */
struct Tile
{
	XMLTag_TSX sTileData;						   // The XML tag for the tile data.
	std::vector<XMLTag_TSX> vecProperties; 		   // Stores any custom properties
	XMLTag_TSX sObjectGroupData;				   // The XML tag for the object group data.
	std::vector<ObjectDataInfo> vecObjectDataInfo; // The vector storing object data information.

};


/**
 * Map_TSX represents the map data in a TSX file.
 * It contains the image data, tileset data, and a vector of tiles.
 */
struct Map_TSX
{
	XMLTag_TSX ImageData;	    // The XML tag for the image data.
	XMLTag_TSX TilesetData;     // The XML tag for the tileset data.
	std::vector<Tile> vecTiles; // The vector storing all the tiles.

};

/*
 * TSXParser is responsible for parsing TSX files and extracting map data.
 */
class TSXParser {

public:

	/**
	 * Retrieves the parsed map data.
	 * @return The Map_TSX object containing the parsed map data.
	 */
	Map_TSX GetData()
	{
		return parsedMapInfo;
	}

	/*
	* Resets the parser Map Info class objects
	*/
	void ResetMapInfo()
	{
		parsedMapInfo.ImageData.data.clear();
		parsedMapInfo.ImageData.tag = "";
		parsedMapInfo.TilesetData.data.clear();
		parsedMapInfo.TilesetData.tag = "";
		parsedMapInfo.vecTiles.clear();
	}

private:

	Map_TSX parsedMapInfo;		// The parsed map data.
	Tile sTile;					// The current tile being parsed.

	bool bIsFirstPass = true;	// Flag indicating if this is the first pass of parsing.

#if NDEBUG
	bool bInDebugMode = false; // Set to false to disable debug logs
#else
	bool bInDebugMode = false; // Set to true to enable debug logs
#endif
	
	/**
	 * Parses an individual XML tag from the TSX file.
	 * @param tag The XML tag as a string.
	 */
	void ParseTag(std::string tag)
	{
		XMLTag_TSX newTag; // The new XML tag being parsed.

		// First character is a '<' so we discard it.
		tag.erase(0, 1);

		// Last characters in the tag '>'. 
		tag.erase(tag.length() - 1, 1);


		// We have an edge case, where the last characters can be > or /> 
		// We need to manage this with a conditional statement
		char lastChar = tag.at(tag.length() - 1);
		if (lastChar == '/')
		{
			//last characters in the tag '/'. 
			tag.erase(tag.length() - 1, 1);
		}


		// Now parse by spaces.
		std::stringstream strStream(tag); //Turn it into a string stream to now parse into individual whitespaces.
		std::string strTempString;        // Temporary storage for each parsed segment of the tag.		
		strTempString.reserve(256);      // Reserve some space to avoid frequent reallocations.

		while (strStream.good())
		{
			strTempString.clear();
			strStream >> strTempString;

			if (newTag.tag.length() == 0)
			{
				//Tag's empty, so first line is the tag.
				newTag.tag = strTempString;
				if (bInDebugMode)
					std::cout << "Tag: " << newTag.tag << "\n";
			}
			else
			{
				// TODO: Edge case there will be tags that are not in the format of key-->value, example:  <point/>
				// We need to manage this
				std::string key = strTempString.substr(0, strTempString.find("="));
				std::string value = strTempString.substr(strTempString.find("=") + 1, std::string::npos);

				//Strip Quotation marks, if they exist. 
				if (value.substr(0, 1) == "\"") value = value.substr(1, std::string::npos);
				if (value.substr(value.length() - 1, 1) == "\"") value = value.substr(0, value.length() - 1);

				newTag.data[key] = value;
				if (bInDebugMode)
					std::cout << "  " << key << ":" << newTag.data[key] << "\n";

			} // END if(newTag.tag.length() == 0...

		} // END While(sd.good())

		// Add the newly parsed tag to the list of all tags.

		if (newTag.tag == "tileset"){
			parsedMapInfo.TilesetData = newTag;
		}
		else if (newTag.tag == "image")
		{
			parsedMapInfo.ImageData = newTag;
		}
		else if (newTag.tag == "tile")
		{
			if (!bIsFirstPass) UpdateVectorIfRequired();

			bIsFirstPass = false;
			sTile.sTileData = newTag;
		}
		else if (newTag.tag == "properties")
		{
			//sTile.sPropertyData = newTag;
		}
		else if (newTag.tag == "property")
		{
			sTile.vecProperties.push_back(newTag);
		}
		else if (newTag.tag == "objectgroup")
		{
			sTile.sObjectGroupData = newTag;
		}
		else if (newTag.tag == "object")
		{
			//sTile.sObjectData = newTag;
			ObjectDataInfo sObjectDataInfo;
			sObjectDataInfo.sObjectData = newTag;
			sObjectDataInfo.sTypeData.tag = "rect";
			sObjectDataInfo.sTypeData.data.clear();
			sTile.vecObjectDataInfo.push_back(sObjectDataInfo);
		}
		else if (newTag.tag == "point")
		{
			// The latest object data will always be the last in the list
			auto& sObjectDataInfo = sTile.vecObjectDataInfo[sTile.vecObjectDataInfo.size() - 1];
			sObjectDataInfo.sTypeData = newTag;
		}
		else if (newTag.tag == "ellipse")
		{
			// The latest object data will always be the last in the list
			auto& sObjectDataInfo = sTile.vecObjectDataInfo[sTile.vecObjectDataInfo.size() - 1];
			sObjectDataInfo.sTypeData = newTag;

		}
		else if (newTag.tag == "polygon")
		{
			// The latest object data will always be the last in the list
			auto& sObjectDataInfo = sTile.vecObjectDataInfo[sTile.vecObjectDataInfo.size() - 1];
			sObjectDataInfo.sTypeData = newTag;
		}
		else if (newTag.tag == "capsule")
		{
			// The latest object data will always be the last in the list
			auto& sObjectDataInfo = sTile.vecObjectDataInfo[sTile.vecObjectDataInfo.size() - 1];
			sObjectDataInfo.sTypeData = newTag;
		}
		else if (newTag.tag == "/object")
		{
			//UpdateVectorIfRequired();

		}
		else if (newTag.tag == "/objectgroup")
		{
			UpdateVectorIfRequired();
		}
		else if (newTag.tag == "/tile")
		{
			UpdateVectorIfRequired();
		}
		else
		{
			if (bInDebugMode)
				std::cout << "Unsupported tag format! Ignoring." << "\n";
		}

		if (bInDebugMode)
			std::cout << "\n" << "=============\n";

	}

	/*
	* We only update the tile vectors if there are no duplicates
	*/
	void UpdateVectorIfRequired()
	{
		Tile newTile = { sTile }; // Copy off our tile

		if (std::none_of(
			parsedMapInfo.vecTiles.begin(),
			parsedMapInfo.vecTiles.end(),
			[newTile](Tile tile) {return tile.sTileData.data == newTile.sTileData.data;}))
		{
			parsedMapInfo.vecTiles.push_back(newTile);
		}

		ResetsTile();
	}

	/*
	* Resets the sTile to defaults
	*/
	void ResetsTile()
	{
		sTile.sObjectGroupData.data.clear();
		sTile.sObjectGroupData.tag.clear();

		for (auto sObjectDataInfo : sTile.vecObjectDataInfo)
		{
			sObjectDataInfo.sObjectData.data.clear();
			sObjectDataInfo.sObjectData.tag.clear();
			sObjectDataInfo.sTypeData.data.clear();
			sObjectDataInfo.sTypeData.tag.clear();
		}

		sTile.vecObjectDataInfo.clear();
		sTile.vecProperties.clear();
	}

	

public:

	/**
	* Constructor for the TSXParser class.
	* @param file The path to the TSX file to be parsed.
	*/
	TSXParser(std::string file)
	{
		/*
		std::cout << "Parsing TSX file: " << file << "\n";
		auto start = std::chrono::high_resolution_clock::now();*/

		// Before we begin lets reset the class objects encase we are loading a new file
		ResetMapInfo();

		// Read the entire file content into a string for parsing.
		std::ifstream f(file, std::ios::in | std::ios::binary);
		if (!f.is_open()) {
			if (bInDebugMode)
				std::cout << "Failed to open file: " << file << "\n";

			return;
		};
		f.seekg(0, std::ios::end);
		std::streampos fileSize = f.tellg();
		f.seekg(0, std::ios::beg);

		// Allocate a string to hold the entire file content.
		std::string fileContent;
		fileContent.resize(fileSize);
		f.read(&fileContent[0], fileSize);
		f.close();

		// Now we have the entire file in a string, we can parse it.
		size_t currentPosition = 0;
		std::stringstream strFileStream(fileContent);
		std::string accumulator = "";
		std::string strTempString;        // Temporary storage for each parsed segment of the tag.		
		strTempString.reserve(256);      // Reserve some space to avoid frequent reallocations.

		while (strFileStream.good()) 
		{
			strTempString.clear();
			strFileStream >> strTempString;
			if (strTempString.empty()) continue;

			if (accumulator.length() > 0) 
			{
				accumulator += " " + strTempString;

				//Check if it ends with '>'
				if (strTempString[strTempString.length() - 1] == '>') 
				{
					ParseTag(accumulator);
					accumulator.clear();
				}
			}
			else
				if (strTempString[0] == '<') 
				{
					//Beginning of XML tag.
					accumulator = strTempString;

					// Let check if it is an end tag </object>
					if (strTempString[strTempString.length() - 1] == '>')
					{
						// ok this is a possiable end tag
						accumulator = strTempString;
						ParseTag(accumulator);
						accumulator.clear();
					}

				}
				else 
				{
					//Start reading in data for this layer.
					std::vector<int>rowData;

					while (strTempString.find(",") != std::string::npos)
					{
						std::string datapiece = strTempString.substr(0, strTempString.find(","));
						strTempString = strTempString.substr(strTempString.find(",") + 1, std::string::npos);
						rowData.push_back(stoi(datapiece));
					}
					if (strTempString.length()) 
					{
						rowData.push_back(stoi(strTempString));
					}
					//parsedMapInfo.LayerData[parsedMapInfo.LayerData.size() - 1].tiles.push_back(rowData);
				}
		}

		//auto end = std::chrono::high_resolution_clock::now();
		//// Calculate duration in milliseconds
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		//// Output results
		//std::cout << "Start Time TSX:    " << start.time_since_epoch().count() << " ns\n";
		//std::cout << "End Time TSX:      " << end.time_since_epoch().count() << " ns\n";
		//std::cout << "Duration TSX:      " << duration.count() << " ms\n";
	}

	virtual ~TSXParser(){}
};