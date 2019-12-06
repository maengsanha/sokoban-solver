#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include "Definition.h"
#include "PushBox.h"

class Solver
{
private:
    char **map;
    std::map<pair<char **, pair<int, int>>, bool> visited;
    string finalSol;
    pair<int, int> userPos;

public:
    Solver();
    void run(char **map, pair<int, int> userPos);
    string getSol();
    bool gameWin(char **map1);
    bool getSolution(char **map, pair<int, int> userPos, string sol);
};

#endif
