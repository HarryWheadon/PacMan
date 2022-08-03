#include "LevelMap.h"
LevelMap::LevelMap(int map[MAP_HEIGHT][MAP_WIDTH])
{
    //Allocate memory for the level map
    m_map = new int* [MAP_HEIGHT];
    for (unsigned int i = 0; i < MAP_HEIGHT; i++)
    {
        m_map[i] = new int[MAP_HEIGHT];
    }

    for (unsigned int i = 0; i < MAP_HEIGHT; i++)
    {
        for (unsigned int j = 0; j < MAP_WIDTH; j++)
        {
            m_map[i][j] = map[i][j];
        }
    }
}

LevelMap::~LevelMap()
{
}
