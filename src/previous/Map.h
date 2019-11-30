#ifndef _MAP_H_
#define _MAP_H_

/* Interface for Sokoban Map and its Logics */

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
public:
    // Constructor

    // Default Constructor
    Map();

    // @Overload
    Map(int map_of_stage[8][8]);

    // Member Functions
    Map &setElement(int target_x, int target_y, int input);
    Map &setCharacter(int target_x, int target_y);
    Map &move(char arrow);
    Map &find_character();
    Map &do_nothing();
    Map &countDest();
    Map &countBoxOnDest();
    Map &increaseNumStep();
    Map &increaseNumPush();

    // Member Variables
    int numDest;
    int numBoxOnDest;
    int map[8][8];
    int default_map[8][8];
    int location_of_character[2];
    int numStep;
    int numPush;
};

#endif