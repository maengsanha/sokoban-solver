#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

#include "GameViewer.h"
#include<queue>
#include<map>


class GameController
{
public:
	/**
	 * Constructor
	 */
	GameController(){};

	GameController(PushBox *model, GameViewer *view) : pushBox(model), gameViewer(view)
	{
	}

	void setGoalPos(vector<Coordinates> goalposition);
	void gameInitialize();
	void gameDelete();
	bool isInMapNow();
	bool isInMapNow(int dy, int dx);
	bool CheckPosition(Coordinates userPos);
	void move(Coordinates userPos);
	bool postProcessing();
	bool isSuccess();
	WINDOW *levelBoard, *stepBoard, *pushBoard, *resetBoard, *gameBoard;
	void goNextLevel();
	void reset();
	void autoResolve();
	void startGame();
	bool showResult();
	
	//auto resolve method
	void run(vector<vector<char>> mapi, int x, int y);
	bool getSolution(vector<vector<char>> mapi, int x, int y, vector<char> sol);
	void doAuto();
	bool gameWin(vector<vector<char>> map);

private:
	const int FINALLEVEL = 6;
	PushBox *pushBox;
	GameViewer *gameViewer;
	vector<char> finalSol;
};

#endif
