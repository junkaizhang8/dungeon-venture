#include <iostream>
#include <fstream>
#include "editorfilewriter.hpp"

// TODO
int EditorFileWriter::saveGameData(const MapData *const mapData, const char filename[MAX_FILENAME_SIZE])
{
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file)
    {
        return FILE_CANNOT_OPEN_ERR;
    }

    // Specify header information
    uint32_t vertexTreeSize;

    file.close();

}