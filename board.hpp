#ifndef BOARD_HPP
#define BOARD_HPP

#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<stdio.h>
#include<ctime>
using namespace std;

typedef std::vector<std::vector<int> > Matrix;

struct Point {
	int x;
	int y;
};

struct Neb {
	vector<Point> fringe;
	vector<Point> boundary;
};

typedef vector<Neb> NebSet;

class Board {
private:
	Matrix bombs;
	bool booom;
	NebSet neb;
	void addNeb(Point p , Neb &f);
	void initialBombs(int x, int y);
	int bombsNerby(Point &p);

public:
	Board();
	Board(int a, int b,int c);//a*b matrix with c bombs
	void print();//print the board
	void printBomb();//print the arrange of mines
	bool isEOG();//end of game
	void pick(int x, int y);//try to select the2
	int winOrLoss();//1=win -1=loss 0=in game
	NebSet getNeb();//get the neighbour sets
	Matrix getBoard();//get the board
	Matrix flags;//default is 0, -1 for mine, 1 for unpicked safe, 2 for picked safe
	Matrix board;
	int minesFound;
	bool init;
	int N;
	int M;
	int K;
};

#endif