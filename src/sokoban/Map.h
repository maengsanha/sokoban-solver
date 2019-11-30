#ifndef __MAP_H__
#define __MAP_H__

#include <vector>

#include "Object.h"
#include "Character.h"
#include "Destination.h"
#include "Box.h"
#include "Wall.h"


//Interface for sokoban map and its logic

/*
0 : Null space
1 : Wall
2 : Box
3 : Destination
4 : Outside
5 : Character
6 : Box on destination
*/

class Map
{
private:
    std::vector<std::vector<Object>> map [8][8];

public:
    //Constructor
    Map();
    //Constructor
    Map(std::vector<std::vector<Object>> map);

    //member function
    Map &init();
    Map &setElement(Object &element);
    void stateChanged();
};

#endif
