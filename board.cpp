#include"board.hpp"

Board::Board() {
	N = 8;
	M = 8;
	K = 10;
	init = 0;
	booom = 0;
    minesFound=0;
	board = Matrix(N, vector<int>(M));
	bombs = Matrix(N, vector<int>(M));
    flags = Matrix(N, vector<int>(M));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			board[i][j] = -1;
			bombs[i][j] = 0;
		}
	}
}

Board::Board(int a,int b,int c) {
    N = a;
    M = b;
    K = c;
    init = 0;
    booom = 0;
    minesFound = 0;
    board = Matrix(N, vector<int>(M));
    bombs = Matrix(N, vector<int>(M));
    flags = Matrix(N, vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            board[i][j] = -1;
            bombs[i][j] = 0;
            flags[i][j] = 0;
        }
    }
}

void Board::print() {
    string edge(2*N + 3, '=');
    cout << " |";
    for (int j = 0; j < M; j++) cout << j<<" ";
    cout << endl;
    cout << edge << endl;
    for (int i = 0; i<N; i++) {
        cout << i;
        cout << "|";
        for (int j = 0; j<M; j++) {
            if (board[i][j] == -1) { cout << "* "; }//where is unknown
            else {
                if (board[i][j] == 0) cout << "  ";//where is the interior
                else if (board[i][j] == -2) cout << "x ";//where you lose
                else cout << board[i][j]<<" ";//where is the boudary
            }
        }
        cout << "|" << endl;
    }
    cout << edge << edge << endl;
}

void Board::printFlags() {
    string edge(2*N + 3, '=');
    cout << " |";
    for (int j = 0; j < M; j++) cout << j << " ";
    cout << endl;
    cout << edge << endl;
    for (int i = 0; i<N; i++) {
        cout << i;
        cout << "|";
        for (int j = 0; j<M; j++) {
            cout<<flags[i][j]<<" ";
        }
        cout << "|" << endl;
    }
    cout << edge << endl;
}


void Board::printBomb() {
    string edge(2*N + 3, '=');
    cout << " |";
    for (int j = 0; j < M; j++) cout << j << " ";
    cout << endl;
    cout << edge << endl;
    for (int i = 0; i<N; i++) {
        cout << i;
        cout << "|";
        for (int j = 0; j<M; j++) {
            cout<<bombs[i][j]<<" ";
        }
        cout << "|" << endl;
    }
    cout << edge << endl;
}

bool Board::isEOG() {
    if (booom == 1) return 1;
    int count =0;
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++){
            if(board[i][j]==-1)count++;
        }
    }
    if (K == count) return 1;
    return 0;
}

void Board::pick(int x, int y) {
    if (!init) initialBombs(x,y);

    if (board[x][y] != -1) return;//this place has already been checked
    if (bombs[x][y] == 1) {
        booom = 1;
        board[x][y] = -2;
        return;
    }
    Neb newNeb;
    Point p{x,y};
    addNeb(p,newNeb);
    neb.push_back(newNeb);
}

//void Board::addNeb(Point p, Neb &f) {
//    if (bombs[p.x][p.y] != 0) return;
//
//    if (bombsNerby(p) == 0) {
//        f.interior.push_back(p);
//        board[p.x][p.y] = 0;
//        for (int i = -1; i < 2; i++) {
//            for (int j = -1; j < 2; j++) {
//                if (i == 0 && j == 0) continue;
//                Point np{p.x+i,p.y+j};
//                if (np.x < 0 || np.x >= N || np.y < 0 || np.y >= M) continue;
//                if (board[np.x][np.y] != -1) continue;
//                addNeb(np,f);//recursively search its neighborhood
//            }
//        }
//    }
//    else {
//        f.boundary.push_back(p);
//        board[p.x][p.y] = bombsNerby(p);
//    }
//}

void Board::addNeb(Point p, Neb &f) {
    if (bombs[p.x][p.y] != 0) return;//is boom

    if (bombsNerby(p) == 0) {
        flags[p.x][p.y] =2;//interior cells have been checked
        board[p.x][p.y] = 0;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (i == 0 && j == 0) continue;
                Point np{p.x+i,p.y+j};
                if (np.x < 0 || np.x >= N || np.y < 0 || np.y >= M) continue;
                if (board[np.x][np.y] != -1){
                    f.fringe.push_back(np);
                    continue;
                };
                addNeb(np,f);//recursively search its neighborhood
            }
        }
    }
    else {
        flags[p.x][p.y] =2;//boundary cells have been checked
        f.boundary.push_back(p);
        board[p.x][p.y] = bombsNerby(p);
    }
}

int Board::bombsNerby(Point &p) {
    int sum= 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) continue;
            Point np{ p.x + i,p.y + j };
            if (np.x < 0 || np.x >= N || np.y < 0 || np.y >= M) continue;
            if (bombs[np.x][np.y] == 1) sum++;
        }
    }
    return sum;
}

void Board::initialBombs(int x, int y) {//called only once, make sure the first pick is not a mine
    init = 1;
    srand(time(NULL));
    int T = N*M;
    int count = 0;
    while(count<K){
        int t = rand() % T;
        int row = t/M;
        int col = t-row*M;
        if (x == row && y == col) continue;//if it is what's picked, skip
        if (bombs[row][col] == 1) continue;//if it is already a mine, skip
        bombs[row][col] = 1;
        count++;
    }
}

int Board::winOrLoss() {
    if (isEOG()==1 && booom == 1) return -1;
    if (isEOG()==1 && booom != 1) return 1;
    else return 0;
}

NebSet Board::getNeb() {
    return neb;
}

Matrix Board::getBoard() {
    return board;
}


