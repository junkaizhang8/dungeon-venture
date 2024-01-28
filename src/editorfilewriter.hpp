#pragma once

#include "mapdata.hpp"

#define MAX_FILENAME_SIZE 64
#define FILE_CANNOT_OPEN_ERR 1

// TODO
class EditorFileWriter
{
public:
    EditorFileWriter() = default;
    ~EditorFileWriter() = default;
    int saveGameData(const MapData *const mapData, const char filename[MAX_FILENAME_SIZE]);
};