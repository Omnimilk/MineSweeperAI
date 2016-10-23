#ifndef BOARD_HPP
#define BOARD_HPP

#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<stdio.h>
#include<ctime>
#include <math.h>
using namespace std;

typedef std::vector<std::vector<int> > Matrix;

//typedef std::vector<std::vector<double > > DoubleMatrix;

struct Point {
	int x;
	int y;
};

struct Neb {
    vector<Point> interior;
	vector<Point> fringe;
	vector<Point> boundary;
};

typedef vector<Neb> NebSet;


class Board {
private:
	Matrix bombs;
	bool booom;
	void nebOf(Point p , Neb &f);
	void initialBombs(int x, int y);
	int bombsNearby(Point &p);

public:
	Board();
	Board(int a, int b,int c);//a*b matrix with c bombs
	void print();//print the board
	void printBomb();//print the arrange of mines
	void printFlags();
	bool isEOG();//end of game
	void pick(int x, int y);//try to select the2
	int winOrLoss();//1=win -1=loss 0=in game
	NebSet getNeb();//get the neighbour sets
	Matrix getBoard();//get the board
	//DoubleMatrix flags;//default is 0, -1 for mine, 1 for unpicked safe, 2 for picked safe
    Matrix flags;
	Matrix board;//-1 for unknown; non-neg for safe
	int minesFound;
	bool init;
    NebSet neb;
	int N;
	int M;
	int K;
};

#endif