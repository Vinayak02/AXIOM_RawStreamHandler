#include "metaman.h"
#include "../common/mlv.h"
#include <fstream>
#include <iostream>
#include <memory.h>

using namespace RawStreamHandler;

void RawStreamHandler::MetaManEntry(const char *metaStreamLoc, const char *metaCache) {
    // Create Buffer
    int buffersize = 128 * 1024 * 1024;
    char *buf = new char[buffersize];

    std::ifstream metaStream(metaStreamLoc, std::ios::binary | std::ios::in);

    if (!metaStream.is_open()) {
        std::cout << "MetaMan: Could not access meta stream " << metaStreamLoc << std::endl;
        return;
    } else {
        std::cout << "MetaMan: Success in accessing stream " << metaStreamLoc << std::endl;
    }

    std::map<std::string, uint8_t> sizeChart = getMlvSizes();
    int offset = 0;
    std::string blocksEncountered = "";
    uint8_t *blockType[4];
    std::map<std::string, uint8_t>::iterator iter;
    bool markerFound = false;
    int marker;
    metaStream>>marker;
    if(marker == 233)
        markerFound = true;
    while(markerFound){
        metaStream.read(reinterpret_cast<char *>(blockType), 4);
        std::cout<<"Meta Block: "<< reinterpret_cast<char *>(blockType)<<" encountered"<<std::endl;
        if (std::string(reinterpret_cast<char *>(blockType)).compare("")==0){
            std::cout<<"MetaMan: Found unnamed blockType. Assuming eof."<<std::endl;
            break;
        }
        iter = sizeChart.find(std::string(reinterpret_cast<char *>(blockType)));
        if(iter == sizeChart.end()){
            std::cout<<"MetaMan: Could not find size of meta block in sizeChart"<<std::endl;
            break;
        }
        memcpy(buf+offset, blockType, 4);
        offset += 4;
        metaStream.read(buf + offset, iter->second - 4);
        offset += iter->second - 4;
        blocksEncountered += std::string(reinterpret_cast<char *>(blockType));
        blocksEncountered += " ";
        if(offset > buffersize){
            std::cout<<"MetaMan: Buffer overflow error! "<<std::endl;
            return;
        }

        marker = 0;
        markerFound = false;
        metaStream>>marker;
        if(marker == 233)
            markerFound = true;

    }
    std::cout<<"MetaMan: metaStream read into memory buffer"<<std::endl;

    std::ofstream out(metaCache, std::ios::binary|std::ios::out);
    if(!out.is_open()){
        std::cout<<"MetaMan: Cache file could not be opened "<<metaCache<<std::endl;
    }
    else{
        out.write(buf, offset);
        std::cout<<"MetaMan: Cache file writing done"<<metaCache<<std::endl;
    }
    out.close();
    metaStream.close();
    //std::cout<<"MetaMan: Encountered following blocks: \n"<<blocksEncountered<<std::endl;
    delete buf;
    std::cout<<"MetaMan: MetaStream ended"<<std::endl;
}

std::map<std::string, uint8_t> RawStreamHandler::getMlvSizes() {
    std::map<std::string, uint8_t> sizeChart;
    sizeChart.insert(std::pair<std::string, uint8_t>("MLVI", 52));
    sizeChart.insert(std::pair<std::string, uint8_t>("RAWI", 20+ sizeof(raw_info_t)));
    sizeChart.insert(std::pair<std::string, uint8_t>("EXPO", 40));
    sizeChart.insert(std::pair<std::string, uint8_t>("LENS", 96));
    sizeChart.insert(std::pair<std::string, uint8_t>("RTCI", 44));
    return sizeChart;
};