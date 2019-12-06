#include "GameController.h"
#include <unistd.h>

void GameController::gameInitialize()
{
	setlocale(LC_ALL, "");
	initscr();
	resize_term(27, 65);
	start_color();
	curs_set(0);
	noecho();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);   //EMPTY color
	init_pair(3, COLOR_WHITE, COLOR_MAGENTA); //BLOCK color
	init_pair(4, COLOR_WHITE, COLOR_YELLOW);  //BOX color
	init_pair(5, COLOR_WHITE, COLOR_GREEN);   //GOAL color
	init_pair(6, COLOR_WHITE, COLOR_RED);	 //PLAYER color
	init_pair(7, COLOR_WHITE, COLOR_CYAN);	//Start menu color
	init_pair(8, COLOR_RED, COLOR_BLACK);	 //result color

	attron(COLOR_PAIR(7));
	mvprintw(6, 18, "                               ");
	mvprintw(7, 18, "  P U S H    B O X    G A M E  ");
	mvprintw(8, 18, "                               ");
	attroff(COLOR_PAIR(7));
	attron(COLOR_PAIR(6));
	mvprintw(12, 28, "          ");
	mvprintw(13, 28, "   |  |   ");
	mvprintw(14, 28, "          ");
	mvprintw(15, 28, "          ");
	attroff(COLOR_PAIR(6));

	mvprintw(19, 24, "Press \'S\' to start");

	while (1)
	{
		char temp = getch();
		if (temp != 's' && temp != 'S')
			continue;

		else
			break;
	}
	clear();
	startGame();
}

void GameController::gameDelete()
{
	delwin(levelBoard);
	delwin(stepBoard);
	delwin(pushBoard);
	delwin(resetBoard);
	delwin(gameBoard);
	endwin();
	return;
}

bool GameController::isInMapNow()
{
	return 0 < pushBox->getX_userPos() && pushBox->getX_userPos() < pushBox->getRow() &&
		0 < pushBox->getY_userPos() && pushBox->getY_userPos() < pushBox->getCol();
}

bool GameController::isInMapNow(int dy, int dx)
{
	return 0 < dx && dx < pushBox->getRow() && 0 < dy && dy < pushBox->getCol();
}

bool GameController::CheckPosition(Coordinates userposition)
{
	if (!isInMapNow())
		return false;

	int dx = pushBox->getX_userPos() + userposition.x;
	int dy = pushBox->getY_userPos() + userposition.y;

	if (!isInMapNow(dy, dx))
		return false;

	if (pushBox->getMap(dy, dx) == 1)
		return false;

	return true;
}

void GameController::setGoalPos(vector<Coordinates> goalList)
{
	for (int i = 0; i < goalList.size(); i++)
	{
		int goalX = goalList[i].x;
		int goalY = goalList[i].y;

		if (pushBox->getMap(goalY, goalX) == EMPTY)
			pushBox->setMap(Coordinates(goalY, goalX), GOAL);
	}
}

void GameController::move(Coordinates userposition)
{

	int curX = pushBox->getX_userPos();
	int curY = pushBox->getY_userPos();

	int nextX = curX + userposition.x;
	int nextY = curY + userposition.y;

	if (pushBox->getMap(nextY, nextX) == WALL)
	{
		return;
	}
	else
	{
		pushBox->addStep();
	}

	// When pushes the Box
	if (pushBox->getMap(nextY, nextX) == BOX)
	{
		int nextPosBox_X = nextX + userposition.x;
		int nextPosBox_Y = nextY + userposition.y;

		if (pushBox->getMap(nextPosBox_Y, nextPosBox_X) == BOX || pushBox->getMap(nextPosBox_Y, nextPosBox_X) == WALL)
			return;

		pushBox->setMap(Coordinates(curY, curX), EMPTY);
		pushBox->setMap(Coordinates(nextY, nextX), PLAYER);
		pushBox->setMap(Coordinates(nextPosBox_Y, nextPosBox_X), BOX);
		pushBox->setUserPos(Coordinates(nextX, nextY));
		pushBox->addPush();
		return;
	}

	pushBox->setMap(Coordinates(curY, curX), EMPTY);
	pushBox->setMap(Coordinates(nextY, nextX), PLAYER);
	pushBox->setUserPos(Coordinates(nextX, nextY));
}

bool GameController::postProcessing()
{
	setGoalPos(pushBox->getGoalList());
	gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);

	if (isSuccess())
	{
		vector<int> rec;
		rec.push_back(pushBox->getStep());
		rec.push_back(pushBox->getPush());
		rec.push_back(pushBox->getReset());
		pushBox->addRecords(rec);

		if (pushBox->getLevel() == FINALLEVEL)
			return showResult();

		else
		{
			pushBox->setLevel(pushBox->getLevel() + 1);
			pushBox->stepClear();
			pushBox->pushClear();
			pushBox->resetClear();
			pushBox->readMap();
			gameViewer->renderInit(levelBoard, stepBoard, pushBoard, resetBoard);
			gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
		}
	}

	return false;
}

bool GameController::isSuccess()
{
	for (int i = 0; i < pushBox->getGoalList().size(); i++)
	{
		int x = pushBox->getGoalList()[i].x;
		int y = pushBox->getGoalList()[i].y;

		if (pushBox->getMap(y, x) == BOX)
			continue;

		else
			return false;
	}

	return true;
}

void GameController::goNextLevel()
{
	for (int i = 0; i < pushBox->getGoalList().size(); i++)
	{
		int x = pushBox->getGoalList()[i].x;
		int y = pushBox->getGoalList()[i].y;
		pushBox->setMap(Coordinates(y, x), BOX);
	}

	return;
}

void GameController::reset()
{
	pushBox->stepClear();
	pushBox->pushClear();
	pushBox->readMap();
	gameViewer->renderInit(levelBoard, stepBoard, pushBoard, resetBoard);
	gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
	pushBox->addReset();
	return;
}

void GameController::startGame()
{
	mvprintw(3, 18, "P U S H    B O X    G A M E");
	mvprintw(24, 23, "Press \'r\' to reset ");
	mvprintw(26, 23, "Press \'a\' to auto-resolve ");
	refresh();

	gameBoard = newwin(15, 38, 8, 4);
	levelBoard = newwin(3, 13, 8, 45);
	stepBoard = newwin(3, 13, 12, 45);
	pushBoard = newwin(3, 13, 16, 45);
	resetBoard = newwin(3, 13, 20, 45);
	wbkgd(levelBoard, COLOR_PAIR(1));
	wattron(levelBoard, COLOR_PAIR(1));
	wbkgd(stepBoard, COLOR_PAIR(1));
	wattron(stepBoard, COLOR_PAIR(1));
	wbkgd(pushBoard, COLOR_PAIR(1));
	wattron(pushBoard, COLOR_PAIR(1));
	wbkgd(resetBoard, COLOR_PAIR(1));
	wattron(resetBoard, COLOR_PAIR(1));

	mvwprintw(levelBoard, 1, 1, "Level: ");
	mvwprintw(levelBoard, 1, 8, " ");
	mvwprintw(stepBoard, 1, 1, "Move: ");
	mvwprintw(stepBoard, 1, 8, " ");
	mvwprintw(pushBoard, 1, 1, "Push: ");
	mvwprintw(pushBoard, 1, 8, " ");
	mvwprintw(resetBoard, 1, 1, "Reset: ");
	mvwprintw(resetBoard, 1, 8, " ");
	wborder(levelBoard, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(stepBoard, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(pushBoard, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(resetBoard, '|', '|', '-', '-', '+', '+', '+', '+');
	wborder(gameBoard, '|', '|', '-', '-', '+', '+', '+', '+');

	wrefresh(levelBoard);
	wrefresh(stepBoard);
	wrefresh(pushBoard);
	wrefresh(resetBoard);
	wrefresh(gameBoard);
}

bool GameController::showResult()
{
	clear();
	gameViewer->renderResult();
	while (1)
	{
		char t = getch();
		if (t == 'q' || t == 'Q')
			return true;

		else if (t == 'n' || t == 'N')
		{
			pushBox->setLevel(1);
			pushBox->clearRecords();
			pushBox->resetClear();
			pushBox->readMap();
			clear();
			startGame();
			gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
			return false;
		}

		else
			continue;
	}
}

void GameController::autoResolve()
{
	static queue<char> routes;

	// Get Goals' position
	vector<Coordinates> goals = pushBox->getGoalList();
  
	int goalsize = 0;
	while (goalsize != goals.size())
	{
		int goalPosX = goals[goalsize].x;
		int goalPosY = goals[goalsize].y;
		goalsize++;
		// Get Boxs' position
		int boxPosX, boxPosY;

		for (int i = 0; i < pushBox->getCol(); i++)
			for (int j = 0; j < pushBox->getRow(); j++)
				if (pushBox->getMap(i, j) == BOX)
					boxPosX = j, boxPosY = i;

		int moveBoxX = goalPosX - boxPosX, moveBoxY = goalPosY - boxPosY;

		for (int i = 0; i < abs(moveBoxX); i++)
		{
			if (moveBoxX > 0)
				routes.push('r');
			else
				routes.push('l');
		}

		for (int i = 0; i < abs(moveBoxY); i++)
		{
			if (moveBoxY > 0)
				routes.push('d');
			else
				routes.push('u');
		}

		char buf[128];
		//sprintf_s(buf, "�޴� ��... x=%d y=%d gx=%d gy=%d", boxPosX, boxPosY, goalPosX, goalPosY);
		//OutputDebugString(buf);
		while (!routes.empty())
		{
			char direction = routes.front();
			routes.pop();
			int playerPosX = pushBox->getX_userPos(), playerPosY = pushBox->getY_userPos();
			char buf[128];
			//sprintf_s(buf, "�޴� ��... px=%d py=%d bx=%d by=%d gx=%d gy=%d key=%c \n", playerPosX, playerPosY, boxPosX, boxPosY, goalPosX, goalPosY, direction);
			//OutputDebugString(buf);
			for (int i = 0; i < pushBox->getCol(); i++)
				for (int j = 0; j < pushBox->getRow(); j++)
					if (pushBox->getMap(i, j) == BOX)
						boxPosX = j, boxPosY = i;
			switch (direction)
			{
				// Get Player's position

			case 'u':
				// set Player to Box's down position, move Up
				if (playerPosX != boxPosX || playerPosY != boxPosY + 1)
				{
					int movePlayerX = boxPosX - playerPosX, movePlayerY = boxPosY + 1 - playerPosY;

					for (int i = 0; i < abs(movePlayerY); i++)
					{
						if (movePlayerY > 0)
						{
							move(Coordinates(0, 1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(0, -1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}

					for (int i = 0; i < abs(movePlayerX); i++)
					{
						if (movePlayerX > 0)
						{
							move(Coordinates(1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(-1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}
				}
				move(Coordinates(0, -1));
				gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
				usleep(300 * 1000);
				break;

			case 'd':
				// set Player to Box's up position, move Down
				if (playerPosX != boxPosX || playerPosY != boxPosY - 1)
				{
					int movePlayerX = boxPosX - playerPosX, movePlayerY = boxPosY - 1 - playerPosY;
					for (int i = 0; i < abs(movePlayerX); i++)
					{
						if (movePlayerX > 0)
						{
							move(Coordinates(1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(-1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}

					for (int i = 0; i < abs(movePlayerY); i++)
					{
						if (movePlayerY > 0)
						{
							move(Coordinates(0, 1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(0, -1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}
				}
				move(Coordinates(0, 1));
				gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
				usleep(300 * 1000);
				break;

			case 'r':
				// set Player to Box's left position, move Right
				if (playerPosX != boxPosX - 1 || playerPosY != boxPosY)
				{
					int movePlayerX = boxPosX - 1 - playerPosX, movePlayerY = boxPosY - playerPosY;
					for (int i = 0; i < abs(movePlayerX); i++)
					{
						if (movePlayerX > 0)
						{
							move(Coordinates(1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(-1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}

					for (int i = 0; i < abs(movePlayerY); i++)
					{
						if (movePlayerY > 0)
						{
							move(Coordinates(0, 1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(0, -1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}
				}
				move(Coordinates(1, 0));
				gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
				usleep(300 * 1000);
				break;

			case 'l':
				// set Player to Box's right position, move Left
				if (playerPosX != boxPosX + 1 || playerPosY != boxPosY)
				{
					int movePlayerX = boxPosX + 1 - playerPosX, movePlayerY = boxPosY - playerPosY;

					for (int i = 0; i < abs(movePlayerX); i++)
					{
						if (movePlayerX > 0)
						{
							move(Coordinates(1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(-1, 0));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}

					for (int i = 0; i < abs(movePlayerY); i++)
					{
						if (movePlayerY > 0)
						{
							move(Coordinates(0, 1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
						else
						{
							move(Coordinates(0, -1));
							gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
							usleep(300 * 1000);
						}
					}
				}

				move(Coordinates(-1, 0));
				gameViewer->renderAll(levelBoard, stepBoard, pushBoard, resetBoard, gameBoard);
				usleep(300 * 1000);
				break;
			}
		}
	}
}

bool GameController::getSolution(vector<vector<char>> mapi, int x, int y, vector<char> soli)
{
	queue <pair <pair <vector <vector<char>>, vector<char>>, pair<int, int>>> q;
	pair <pair <vector<vector<char>>, vector<char>>,pair<int, int>> initial;

	initial.first.first = mapi;
	initial.first.second = soli;
	initial.second.first = x;
	initial.second.second = y;
	
	q.push(initial);
	map<pair<vector<vector<char>>, pair<int , int>>, bool> visited;
	while(!q.empty())
	{
		pair<pair<vector<vector<char>>, vector<char>>, pair<int, int>> top = q.front();
		vector<vector<char>> map = top.first.first;
		vector<char> sol = top.first.second;
		int userX = top.second.first;
		int userY = top.second.second;

		q.pop();
		if(gameWin(map))
		{
			finalSol = sol;
			return true;
		}

		pair<vector<vector<char>>, pair<int ,int>> vi;
		vi.first = map;
		vi.second.first = userX;
		vi.second.second = userY;
		if(visited[vi])
		{
			continue;
		}

		visited[vi] = true;
		//Right
		if(userX < 9)
		{
			if(map[userY][userX + 1] != WALL)
			{
				if(map[userY][userX+1] == BOX)
				{
					if(userX < 8)
					{
						if(map[userY][userX+2] != WALL && map[userY][userX+2] != BOX)
						{
							swap(map[userY][userX+1], map[userY][userX+2]);
							if(!visited[make_pair(map, make_pair(userX+1, userY))])
							{
								sol.push_back('R');
								q.push(make_pair(make_pair(map, sol), make_pair(userX+1, userY)));
								sol.pop_back();
							}
							swap(map[userY][userX+1], map[userY][userX+2]);
						}
					}
				} else
				{
					if(!visited[make_pair(map, make_pair(userX+1, userY))])
					{
						sol.push_back('R');
						q.push(make_pair(make_pair(map, sol), make_pair(userX+1, userY)));
						sol.pop_back();
					}
				}
			}
		}
		//Left
		if(userX > 0)
		{
			if(map[userY][userX - 1] != WALL)
			{
				if(map[userY][userX-1] == BOX)
				{
					if(userX > 1)
					{
						if(map[userY][userX-2] != WALL && map[userY][userX-2] != BOX)
						{
							swap(map[userY][userX-1], map[userY][userX-2]);
							if(!visited[make_pair(map, make_pair(userX-1, userY))])
							{
								sol.push_back('L');
								q.push(make_pair(make_pair(map, sol), make_pair(userX-1, userY)));
								sol.pop_back();
							}
							swap(map[userY][userX-1], map[userY][userX-2]);
						}
					}
				} else
				{
					if(!visited[make_pair(map, make_pair(userX-1, userY))])
					{
						sol.push_back('L');
						q.push(make_pair(make_pair(map, sol), make_pair(userX-1, userY)));
						sol.pop_back();
					}
				}
			}
		}
		//UP
		if(userY > 0)
		{
			if(map[userY-1][userX] != WALL)
			{
				if(map[userY-1][userX] == BOX)
				{
					if(userX > 1)
					{
						if(map[userY-2][userX] != WALL && map[userY-2][userX] != BOX)
						{
							
							swap(map[userY-1][userX], map[userY-2][userX]);
							if(!visited[make_pair(map, make_pair(userX, userY-1))])
							{
								sol.push_back('U');
								q.push(make_pair(make_pair(map, sol), make_pair(userX, userY-1)));
								sol.pop_back();
							}
							swap(map[userY-1][userX], map[userY-2][userX]);
						}
					}
				} else
				{
					if(!visited[make_pair(map, make_pair(userX, userY-1))])
					{
						sol.push_back('U');
						q.push(make_pair(make_pair(map, sol), make_pair(userX, userY-1)));
						sol.pop_back();
					}
				}
			}
		}

		//DOWN
		if(userY < 9)
		{
			if(map[userY+1][userX] != WALL)
			{
				if(map[userY+1][userX] == BOX)
				{
					if(userY < 8)
					{
						if(map[userY+2][userX] != WALL && map[userY+2][userX] != BOX)
						{
							swap(map[userY+1][userX], map[userY+2][userX]);
							if(!visited[make_pair(map, make_pair(userX, userY+1))])
							{
								sol.push_back('D');
								q.push(make_pair(make_pair(map, sol), make_pair(userX, userY+1)));
								sol.pop_back();
							}
							swap(map[userY+1][userX], map[userY+2][userX]);
						}
					}
				} else
				{
					if(!visited[make_pair(map, make_pair(userX, userY+1))])
					{
						sol.push_back('D');
						q.push(make_pair(make_pair(map, sol), make_pair(userX, userY+1)));
						sol.pop_back();
					}
				}
			}
		}
	}

	return false;
}

void GameController::run(vector<vector<char>> mapi, int x, int y)
{
	mapi.resize(pushBox -> getCol());
	for(int i = 0; i < mapi.size(); i++)
		mapi[i].resize(pushBox -> getRow());
	
	for(int i = 0; i < mapi.size(); i++)
	{
		for(int j = 0; j < mapi[i].size(); j++)
		{
			mapi[i][j] = pushBox -> getMap(i,j);
		}
	}

	vector<char> sol;
	bool isGet = getSolution(mapi,x,y, sol);
}

void GameController::doAuto()
{
	vector<vector<char>> map;
	run(map, pushBox -> getX_userPos(), pushBox -> getY_userPos());
	for(int i = 0; i < finalSol.size(); i++)
	{
		switch(finalSol.at(i))
		{
			case 'R':
				move(Coordinates(1,0));
				break;
			case 'L':
				move(Coordinates(-1,0));
				break;
			case 'U':
				move(Coordinates(0,-1));
				break;
			case 'D':
				move(Coordinates(0,1));
				break;
		}

		postProcessing();
		sleep(1);
	}
}

bool GameController::gameWin(vector<vector<char>> map)
{
	vector<Coordinates> goal = pushBox -> getGoalList();
	for(int i = 0; i < goal.size(); i++)
	{
		int goal_x = goal[i].x;
		int goal_y = goal[i].y;

		if(map[goal_y][goal_x] == BOX)
			continue;
		else
			return false;
	}

	return true;
}