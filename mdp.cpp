#include"mdp.hpp"


mdp::mdp(Matrix const &B, Matrix const &F,int KK) :board(B),flag(F)
{

	K = KK;
	 N = board.size();
	 M = board[0].size();
}


Point mdp::go( )
{

	//initial 
	vector<Point> uncoverP;
	vector<Point> mines;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (board[i][j] == -1)
			{

				Point np{ i,j };
				if (flag[i][j] == -1)
				{
					mines.push_back(np);
					continue;
				}
				uncoverP.push_back(np);
			}
		}
	}
	int num_Uncover = uncoverP.size();
	int num_Minefound = mines.size();
	int num_Minenotf = K - num_Minefound;


	//guess part
	int loop=Maxguess;
	vector<Matrix> guess;

	if (num_Minenotf == 0)
	{
		for (int i = 0; i < mines.size(); i++)
		{
			Matrix newmines(N, vector<int>(M, 0));
			newmines[mines[i].x][mines[i].y] = 1;
			guess.push_back(newmines);
		}
	}
	else {
		vector<int> gothrough(num_Minenotf, 0);
		for (int m = 0; m < num_Minenotf; m++)
		{
			gothrough[m] = m;
		}

		while (loop > 0)
		{
			vector<Point> tempP = uncoverP;
			vector<Point> tempM = mines;

			for (int m = 0; m < num_Minenotf; m++)
			{
				tempM.push_back(tempP[gothrough[m]]);
			}
			Matrix newmines(N, vector<int>(M, 0));
			for (int i = 0; i < tempM.size(); i++)
			{
				newmines[tempM[i].x][tempM[i].y] = 1;
			}
			//	print(newmines);
			//	cout << "case: " << gothrough[0] << " " << gothrough[1] << " " << gothrough[2] << " " << gothrough[3] << " " << endl;
			if (compare(showboard(newmines))) guess.push_back(newmines);


			if (!nextset(gothrough, uncoverP.size())) break;
			loop--;
		}
	}
	

	//for (int k = 0; k < guess.size(); k++)
//	{
//		print(guess[k]);
//		cout << endl;

//	}

	//print(board);
	//evaluate part
	
	int T = guess.size();

	vector<int> vote(num_Uncover, 0);
	for (int t = 0; t < T; t++)
	{
		int v = decide(guess[t], uncoverP);
		if (v > -1) vote[v]++;
	}
	int max = 0;
	int index = -1;
	for (int i = 0; i < num_Uncover; i++)
	{
		if (vote[i] > max)
		{
			max = vote[i];
			index = i;
		}
	}
	
	Point result{ -1,-1 };
	if (index > -1) result = uncoverP[index];
	return result;
}


int mdp::decide( Matrix const &guess, vector<Point> const&uncoverP)
{

	double max = -1;
	int index = -1;
	for (int i = 0; i < uncoverP.size(); i++)
	{
		double temp = utility(board, guess, uncoverP[i], 0);
		if (temp > max)
		{
			max = temp;
			index = i;
		}
	}
	if (max >=0 ) return index;
	return -1;

}

double mdp::utility(Matrix const nboard,Matrix const &iGuess,Point p,int it)
{
	//cout << endl;
	//cout << "it=: " << it << "  point " << p.x << p.y << endl;
	if (iGuess[p.x][p.y] == 1) return -1;
	if (it > 3) return 0;
	Matrix now = pick(nboard, iGuess, p);
	//print(now);
	double nowU = 0;
	vector<Point> way;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (now[i][j] == -1)
			{
				nowU += 1;
				Point np{ i,j };
				way.push_back(np);
			}
		}
	}
	if (nowU == N*M-K) return pow(gama,it+1);
	nowU = 1 - (nowU / (N*M-K));
	nowU =nowU* pow(gama, it+1);
	double max = 0;
	for (int l = 0; l < way.size(); l++)
	{
		double temp = utility(now, iGuess, way[l], it + 1);
		if (temp > max) max = temp;
	}
	return nowU + max;
}

Matrix mdp::pick(Matrix const&nboard, Matrix const&iGuess, Point p) {
	Matrix next;
	next = nboard;
	addneb(next, iGuess, p);
	return next;
}




void mdp::addneb(Matrix &nboard, Matrix const&iGuess, Point p) {

	if (iGuess[p.x][p.y] != 0) return;

	if (bombsNerby(p,iGuess) == 0) {
		nboard[p.x][p.y] = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i == 0 && j == 0) continue;
				Point np{ p.x + i,p.y + j };
				if (np.x < 0 || np.x >= N || np.y < 0 || np.y >= M) continue;
				if (nboard[np.x][np.y] != -1) {
					continue;
				};
				addneb(nboard,iGuess,np);//recursively search its neighborhood
			}
		}
	}
	else {
		nboard[p.x][p.y] = bombsNerby(p,iGuess);
	}
	return;
}


Matrix mdp::showboard(Matrix &mines)
{
	
	int N = mines.size();
	int M = mines[0].size();
	Matrix show(N, vector<int>(M));
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			Point t{ i,j };
			//cout << i << " " << j << endl;
			show[i][j] = bombsNerby(t, mines);
		}
	}
	return show;
}

int mdp::bombsNerby(Point &p, Matrix const&mines) {
	int sum = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (i == 0 && j == 0) continue;
			Point np{ p.x + i,p.y + j };
			if (np.x < 0 || np.x >= N || np.y < 0 || np.y >= M) continue;
			if (mines[np.x][np.y] == 1) sum++;
		}
	}
	return sum;
}


bool mdp::compare( Matrix const &show)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if (board[i][j] == -1) continue;
			if (board[i][j] != show[i][j])
			{
				//cout << "wrong at : " << i << j << endl;
				return 0;
			}

		}
	}
	return 1;
}


void mdp::print(Matrix m) {
	string edge(2 * N + 3, '=');
	cout << " |";
	for (int j = 0; j < M; j++) cout << j << " ";
	cout << endl;
	cout << edge << endl;
	for (int i = 0; i<N; i++) {
		cout << i;
		cout << "|";
		for (int j = 0; j<M; j++) {
			if (m[i][j] == -1)
			{
				cout << "- ";
				continue;
			}
			cout << m[i][j] << " ";
		}
		cout << "|" << endl;
	}
	cout << edge << endl;
}


bool mdp::nextset(vector<int> &g, int maxM)
{
	int T = g.size();
	int t = T-1;
	int back = 1;
	if (g[t] < maxM - back)
	{
		g[t]++;
		return 1;
	}
	while (g[t] >= maxM - back)
	{
		t--;
		back++;
		if (t < 0) return 0;
	}
	g[t]++;
	int temp = g[t];
	while (t < T-1)
	{
		temp++;
		t++;
		g[t] = temp;

	}
	return 1;

}