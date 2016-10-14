#ifndef MDP_HPP
#define MDP_HPP


#include"board.hpp"
#include<cmath>

using namespace std;
const int Maxguess = 10000;
const double gama = 0.5;

class mdp {
private:
	//parameters
	const Matrix board;
	const Matrix flag;
	int N;
	int M;
	int K;//totl number of bombs

	//functions used in guess part
	int bombsNerby(Point &p, Matrix const &mines);
	bool compare( Matrix const &show);
    Matrix showboard(Matrix &mines);
	bool nextset(vector<int> &g, int maxM);

	////function used to simulate the game,in decision part
	Matrix pick(Matrix const&nboard, Matrix const&iGuess, Point p);
	void addneb(Matrix &nboard, Matrix const&iGuess, Point p);
	

	//utility function
	double utility(Matrix const nboard, Matrix const &iGuess, Point p, int it);//iterlate U function

	////
	//Using mdp ,decide which point to go according to input board and mines graph
	int decide(Matrix const &guess, vector<Point> const&uncoverP);

	//debug function
	void print(Matrix m);
public:

	mdp(Matrix const &B, Matrix const &F,int KK);


	Point  go();//main function, output the decision
	



};










#endif // !_MDP_HPP