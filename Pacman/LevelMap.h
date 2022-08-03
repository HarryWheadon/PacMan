#pragma once
#ifndef _LEVELMAP_H
#define _LEVELMAP_H
#include "constants.h"
class LevelMap
{
public:
	LevelMap(int map[MAP_HEIGHT][MAP_WIDTH]);
	~LevelMap();
private:
	int** m_map;
};
#endif
