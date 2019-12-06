#include "Solver.h"

Solver::Solver() : finalSol("")
{
}

void Solver::run(char **map1, pair<int, int> userPosition)
{
    printf("run solver\n");
    map = new char *[10];
    for (int i = 0; i < 10; i++)
    {
        map[i] = new char[10];
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            map[i][j] = map1[i][j];
        }
    }

    userPos.first = userPosition.first;
    userPos.second = userPosition.second;

    string sol = "";
    printf("start solution\n");
    getSolution(map, userPos, sol);
    printf("answer is %s\n", sol);
}

string Solver::getSol()
{
    return finalSol;
}

bool Solver::getSolution(char **mapi, pair<int, int> userPosi, string soli)
{
    queue<pair<pair<char **, string>, pair<int, int>>> q;
    pair<pair<char **, string>, pair<int, int>> initial;

    initial.first.first = mapi;
    initial.first.second = soli;
    initial.second.first = userPos.first;
    initial.second.second = userPos.second;

    q.push(initial);

    while (!q.empty())
    {
        pair<pair<char **, string>, pair<int, int>> top = q.front();
        char **map = top.first.first;
        string sol = top.first.second;
        int userX = top.second.first;
        int userY = top.second.second;

        q.pop();
        if (gameWin(map))
        {
            finalSol = sol;
            return true;
        }

        pair<char **, pair<int, int>> cur;
        cur.first = map;
        cur.second.first = userX;
        cur.second.second = userY;

        if (visited[cur])
            continue;

        visited[cur] = true;

        if (userX < 9)
        {
            if (map[userY][userX + 1] != WALL) //Right
            {
                if (map[userY][userX + 1] == BOX)
                {
                    if (userX < 8)
                    {
                        if (map[userY][userX + 2] != WALL && map[userY][userX + 2] != BOX)
                        {
                            swap(map[userY][userX + 1], map[userY][userX + 2]);
                            if (!visited[make_pair(map, make_pair(userX + 1, userY))])
                            {
                                q.push(make_pair(make_pair(map, sol + "R"), make_pair(userX + 1, userY)));
                            }
                            swap(map[userY][userX + 1], map[userY][userX + 2]);
                        }
                    }
                }
                else
                {
                    if (!visited[make_pair(map, make_pair(userX + 1, userY))])
                    {
                        q.push(make_pair(make_pair(map, sol + "R"), make_pair(userX + 1, userY)));
                    }
                }
            }
        }

        if (userX > 0)
        {
            if (map[userY][userX - 1] != WALL) //Left
            {
                if (map[userY][userX - 1] == BOX)
                {
                    if (userX > 1)
                    {
                        if (map[userY][userX - 2] != WALL && map[userY][userX - 2] != BOX)
                        {
                            swap(map[userY][userX - 1], map[userY][userX - 2]);
                            if (!visited[make_pair(map, make_pair(userX - 1, userY))])
                                q.push(make_pair(make_pair(map, sol + "L"), make_pair(userX - 1, userY)));

                            swap(map[userY][userX - 1], map[userY][userX - 2]);
                        }
                    }
                }
                else if (!visited[make_pair(map, make_pair(userX - 1, userY))])
                    q.push(make_pair(make_pair(map, sol + "L"), make_pair(userX - 1, userY)));
            }
        }

        if (userY > 0)
        {
            if (map[userY - 1][userX] != WALL) //Up
            {
                if (map[userY - 1][userX] == BOX)
                {
                    if (userY > 1)
                    {
                        if (map[userY - 2][userX] != WALL && map[userY - 2][userX] != BOX)
                        {
                            swap(map[userY - 1][userX], map[userY - 2][userX]);
                            if (!visited[make_pair(map, make_pair(userX, userY - 1))])
                            {
                                q.push(make_pair(make_pair(map, sol + "U"), make_pair(userX, userY - 1)));
                            }
                            swap(map[userY - 1][userX], map[userY - 2][userX]);
                        }
                    }
                }
                else if (!visited[make_pair(map, make_pair(userX, userY - 1))])
                    q.push(make_pair(make_pair(map, sol + "U"), make_pair(userX, userY - 1)));
            }
        }

        if (userY < 9)
        {
            if (map[userY + 1][userX] != WALL) //Down
            {
                if (map[userY + 1][userX] == BOX)
                {
                    if (userY < 8)
                    {
                        if (map[userY + 2][userX] != WALL && map[userY + 2][userX] != BOX)
                        {
                            swap(map[userY + 2][userX], map[userY + 2][userX]);
                            if (!visited[make_pair(map, make_pair(userX, userY + 1))])
                                q.push(make_pair(make_pair(map, sol + "D"), make_pair(userX, userY + 1)));

                            swap(map[userY + 1][userX], map[userY + 2][userX]);
                        }
                    }
                }
                else if (!visited[make_pair(map, make_pair(userX, userY + 1))])
                    q.push(make_pair(make_pair(map, sol + "D"), make_pair(userX, userY + 1)));
            }
        }
    }

    return false;
}

bool Solver::gameWin(char **map1)
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (map1[i][j] == BOX && map[i][j] != GOAL)
                return false;

    return true;
}
