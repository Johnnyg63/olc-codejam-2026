#pragma once

#include <fstream>
#include <sstream>
#include <strstream>
#include <string>
#include <map>
#include <chrono>

using namespace olc;

/*
 * XML Tag structure for TMX parsing.
 */
struct XMLTag{

    std::string tag;                            // The name of the XML tag
    std::map<std::string,std::string> data;    // The key-value pairs within the XML tag

    /*
        * Formats the tag data into a readable string.
        * @param tiles The key-value pairs to format.
        * @return A formatted string representing the tag data.
    */
    const std::string FormatTagData(std::map<std::string,std::string>tiles) {

        std::string displayStr=""; 

        for (std::map<std::string,std::string>::iterator it=data.begin();it!=data.end();it++) {
            displayStr+= "  " + it->first + ": " + it->second + "\n";
        }
        return displayStr;
    }

    /*
        * Overloads the stream insertion operator for XMLTag.
        * @param os The output stream.
        * @param rhs The XMLTag to output.
        * @return The output stream with the XMLTag formatted.
    */
    friend std::ostream& operator << (std::ostream& os, XMLTag& rhs) { 
        os << rhs.tag <<"\n"<<  rhs.FormatTagData(rhs.data) <<"\n";
        return os; 
    }
    
    /*
        * Retrieves the integer value associated with the specified data tag.
        * @param dataTag The key for the data to retrieve.
        * @return The integer value corresponding to the data tag.
    */
    int GetInteger(std::string dataTag) {
        return std::stoi(data[dataTag]);
    }

    /*
        * Retrieves the double value associated with the specified data tag.
        * @param dataTag The key for the data to retrieve.
        * @return The double value corresponding to the data tag.
    */
    double GetDouble(std::string dataTag) {
        return std::stod(data[dataTag]);
    }

    /*
        * Retrieves the boolean value associated with the specified data tag.
        * @param dataTag The key for the data to retrieve.
        * @return The boolean value corresponding to the data tag.
    */
    bool GetBool(std::string dataTag) {
        if (data[dataTag]=="0") {
            return false;
        } else {
            return true;
        }
        
    }
};

/*
 * LayerTag
 * Represents a layer within the TMX map, containing the XML tag and tile data.
 */
struct LayerTag{
    XMLTag tag;
    std::vector<std::vector<int>> tiles;

    std::string str() {
        std::string displayStr=tag.tag+"\n"+tag.FormatTagData(tag.data);
        displayStr+="  DATA ("+std::to_string(tiles[0].size())+"x"+std::to_string(tiles.size())+")\n";
        return displayStr;
    }
};

struct Map_TMX{

    XMLTag MapData;                  // Stores the XML tag for the map itself.
    XMLTag TilesetData;              // Stores the XML tag for the tileset.
    std::vector<LayerTag> LayerData; // Stores the layer data for the map.

    /*
        * Formats the layer data for output.
        * @param os The output stream.
        * @param tiles The vector of LayerTag objects to format.
        * @return The formatted string representing the layer data.
    */
    std::string FormatLayerData(std::ostream& os, std::vector<LayerTag>tiles) {
        std::string displayStr;
        for (int i=0;i<LayerData.size();i++) {
            displayStr+=LayerData[i].str();
        }
        return displayStr;
    }

    /*
        * Overloads the output stream operator for the Map_TMX class.
        * @param os The output stream.
        * @param rhs The Map_TMX object to output.
        * @return The output stream with the formatted Map_TMX data.
    */
    friend std::ostream& operator << (std::ostream& os, Map_TMX& rhs) { 
        os << rhs.MapData <<"\n"<< rhs.TilesetData <<"\n"<< rhs.FormatLayerData(os,rhs.LayerData) <<"\n";
        return os; 
    }
};

/*
    * TMXParser class for parsing TMX map files.
*/
class TMXParser{

    public:

    /*
        * Retrieves the parsed TMX map data.
        * @return The Map_TMX object containing the parsed map information.
    */
    Map_TMX GetData() {
        return parsedMapInfo;
    }

	private:

    Map_TMX parsedMapInfo;  // Stores the parsed TMX map information.

#if NDEBUG
    bool bInDebugMode = false; // Set to false to disable debug logs
#else
    bool bInDebugMode = false; // Set to true to enable debug logs
#endif

    /*
        * Parses an individual XML tag and updates the parsed map information accordingly.
        * @param tag The XML tag string to parse.
    */
	void ParseTag(std::string tag) {
		XMLTag newTag;
		
        // First character is a '<' so we discard it.
		tag.erase(0,1); 

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


		std::stringstream strStream(tag); // Turn it into a string stream to now parse into individual whitespaces.
		std::string strTempString;        // Temporary storage for each parsed segment of the tag.
        strTempString.reserve(256);       // Reserve some space to avoid frequent reallocations.

		while (strStream.good()) {

            strTempString.clear();
            strStream>>strTempString; // Extract the next segment of the tag from the string stream.

			if (newTag.tag.length() == 0) { 
                //Tag's empty, so first line is the tag.
				newTag.tag=strTempString    ;
				if (bInDebugMode)
					std::cout<<"Tag: "<<newTag.tag<<"\n";

			} 
            else {
                // This segment represents an attribute key-value pair.
				std::string key   = strTempString.substr(0,strTempString.find("="));
				std::string value = strTempString.substr(strTempString.find("=")+1,std::string::npos);

                //Strip Quotation marks.
                value = value.substr(1,std::string::npos);
                value = value.substr(0,value.length()-1);

				newTag.data[key] = value;

				if (bInDebugMode)
					std::cout<<"  "<<key<<":"<<newTag.data[key]<<"\n";
			}
		}

        if (newTag.tag=="map") {
            parsedMapInfo.MapData=newTag;

        } 
        else if (newTag.tag=="tileset") {
            parsedMapInfo.TilesetData=newTag;

        } 
        else if (newTag.tag=="layer") {
            LayerTag l = {newTag};
            parsedMapInfo.LayerData.push_back(l);

        } else {
            if (bInDebugMode)
                std::cout<<"Unsupported tag format! Ignoring."<<"\n";
        }

		if (bInDebugMode)
			std::cout<<"\n"<<"=============\n";
	}

	public:

    // Constructor for TMXParser class. Takes the file path of the TMX file to parse.
	TMXParser(std::string file)
    {
		/*
        std::cout << "Parsing TMX file: " << file << "\n";
        auto start = std::chrono::high_resolution_clock::now();*/

        // Lets load the entire file into a string to make things a little faster
		std::ifstream f(file,std::ios::in | std::ios::binary);
		if (!f.is_open()) {

			if (bInDebugMode)
				std::cout << "Failed to open file: " << file << "\n";
			return;
        };
		f.seekg(0, std::ios::end);
		std::streampos fileSize = f.tellg();
		f.seekg(0, std::ios::beg);

        // Read the entire file content into a string.
		std::string fileContent;
		fileContent.resize(fileSize);
		f.read(&fileContent[0], fileSize);
        f.close();

		// Now we have the entire file in a string, we can parse it.
		size_t currentPosition = 0;
        std::stringstream strFileStream(fileContent);
		std::string accumulator = "";
        std::string data;

		while (strFileStream.good()) {

            data.clear();
			strFileStream>>data;            // Read the next segment of data from the file stream.
            if (data.empty()) continue;

			if (accumulator.length()>0) {
                // Append the current data segment to the accumulator.
				accumulator+=" "+data;
				//Check if it ends with '>'
				if (data[data.length()-1]=='>') {
					ParseTag(accumulator);
					accumulator.clear();
				}

			} 
            else if (data[0]=='<') {
				//Beginning of XML tag.
				accumulator=data;

			} 
            else {
                //Start reading in data for this layer.
                std::vector<int>rowData;

                while (data.find(",")!=std::string::npos) {
                    // Extract the next piece of data separated by a comma.
                    std::string datapiece = data.substr(0,data.find(","));
                    data = data.substr(data.find(",")+1,std::string::npos);
                    rowData.push_back(stoi(datapiece));
                }

                if (data.length()) {
                    rowData.push_back(stoi(data));
                }

                parsedMapInfo.LayerData[parsedMapInfo.LayerData.size()-1].tiles.push_back(rowData);
            }
		}

        if (bInDebugMode)
            std::cout<<"Parsed Map Data:\n"<<parsedMapInfo<<"\n";

		//auto end = std::chrono::high_resolution_clock::now();
  //      // Calculate duration in milliseconds
  //      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  //      // Output results
  //      std::cout << "Start Time TMX:    " << start.time_since_epoch().count() << " ns\n";
  //      std::cout << "End Time TMX:      " << end.time_since_epoch().count() << " ns\n";
  //      std::cout << "Duration TMX:      " << duration.count() << " ms\n";

	}

    virtual ~TMXParser(){}

};