#ifndef _PUSHBOX_H_
#define _PUSHBOX_H_

#include "Definition.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ncurses.h"

using namespace std;

class PushBox
{
private:
	char **map;
	// TODO: change rowMax = 8, colMax = 8
	int rowMax = 10;
	int colMax = 10;
	int level = 1;
	int step = 0;
	int push = 0;
	int reset = 0;
	Coordinates userPosition;
	vector<Coordinates> goalList;
	vector<vector<int>> records;
	Coordinates boxPosition;

public:
	PushBox()
	{
		ifstream in;
		in.open("level1.txt");
		in >> userPosition.x >> userPosition.y;

		map = new char *[rowMax];
		for (int i = 0; i < rowMax; i++)
		{
			int temp;
			map[i] = new char[colMax];
			for (int j = 0; j < colMax; j++)
			{
				in >> temp;
				map[i][j] = temp;
				if (temp == 3)
					goalList.push_back(Coordinates(j, i));

				if (temp == 2)
				{
					boxPosition.y = i;
					boxPosition.x = j;
				}
			}
		}
	};

	int getLevel()
	{
		return level;
	}

	void setLevel(const int &input)
	{
		this->level = input;
		return;
	}

	Coordinates getUserPos()
	{
		return userPosition;
	}

	Coordinates getBoxPos()
	{
		return boxPosition;
	}
	void setUserPos(const Coordinates &input)
	{
		userPosition.x = input.x;
		userPosition.y = input.y;
		return;
	}

	int getStep()
	{
		return step;
	}

	void addStep()
	{
		step++;
		return;
	}

	int getPush()
	{
		return push;
	}

	void addPush()
	{
		this->push++;
		return;
	}

	void stepClear()
	{
		step = 0;
	}

	void pushClear()
	{
		this->push = 0;
	}

	int getReset()
	{
		return reset;
	}

	void addReset()
	{
		this->reset++;
		return;
	}

	void resetClear()
	{
		reset = 0;
	}

	void setX_userPos(const int &input)
	{
		this->userPosition.x = input;
	}

	void setY_userPos(const int &input)
	{
		this->userPosition.y = input;
	}

	int getX_userPos()
	{
		return userPosition.x;
	}

	int getY_userPos()
	{
		return userPosition.y;
	}

	int getRow()
	{
		return rowMax;
	}

	int getCol()
	{
		return colMax;
	}

	vector<Coordinates> getGoalList()
	{
		return goalList;
	}

	vector<vector<int>> getRecords()
	{
		return records;
	}

	void addRecords(vector<int> input)
	{
		records.push_back(input);
	}

	void clearRecords()
	{
		records.clear();
	}

	void readMap()
	{
		ifstream in;
		string a = "level";
		string b = to_string(getLevel());
		string c = ".txt";
		in.open(a + b + c);
		in >> userPosition.x >> userPosition.y;

		goalList.clear();
		for (int i = 0; i < rowMax; i++)
		{
			int temp;
			for (int j = 0; j < colMax; j++)
			{
				in >> temp;
				map[i][j] = temp;
				if (temp == 3)
					goalList.push_back(Coordinates(j, i));
			}
		}
		return;
	}

	void setMap(const Coordinates &i, const char &c)
	{
		this->map[i.x][i.y] = c;
	}

	char getMap(int a, int b)
	{
		return map[a][b];
	}

	char **getMap()
	{
		return map;
	}
};

#endif