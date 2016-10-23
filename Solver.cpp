//
// Created by bozai on 20/10/2016.
//

#include "Solver.hpp"
Solver::Solver(Board &board1) {
    board = board1;
}

Matrix Solver::distToBoundary() {
    vector<Point> boundary;
    NebSet nebSetNow = board.neb;
    int numOfNebs = nebSetNow.size();

    for(int i=0;i<numOfNebs;i++){
        Neb nebNow = nebSetNow[i];
        boundary.reserve( boundary.size() + nebNow.boundary.size() ); // preallocate memory
        boundary.insert( boundary.end(), nebNow.boundary.begin(), nebNow.boundary.end() );
    }
    int N=board.N, M=board.M;
    Matrix dis2bd = Matrix(N, vector<int>(M));;//the distance from the uncovered cell to boundary distance. initialize -100
    Matrix bd = Matrix(N, vector<int>(M));
    //initial with -1, copy flags
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            dis2bd[i][j]=-1;
            bd[i][j]=board.board[i][j];
        }
    }

    vector<Point> S = boundary;
    vector<Point> c_p={{-1,0},{0,-1},{1,0},{0,1}};

    for(int i=0;i<S.size();i++){
        int x=boundary[i].x;
        int y=boundary[i].y;
        dis2bd[x][y]=0;
    }
    vector<Point> D;

    while(S.size()!=0){
        D.clear();
        for(int i=0;i<S.size();i++){
            for(int j=0;j<c_p.size();j++){
                int cx=S[i].x;
                int c_prx=S[i].x+c_p[j].x;
                int cy=S[i].y;
                int c_pry=S[i].y+c_p[j].y;

                if (c_prx < 0 || c_prx >= N || c_pry < 0 || c_pry >= M)continue;
                if((dis2bd[c_prx][c_pry]==-1) && (bd[c_prx][c_pry]==-1)){
                    D.push_back({c_prx,c_pry});
                    dis2bd[c_prx][c_pry]=dis2bd[cx][cy]+1;

                }
            }
        }
        S.clear();
        S=D;
    }
    return dis2bd;
}

bool Solver::safeCellExist() {
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.flags[i][j]==1){//is it still safe to use == given flags now is float?
                return true;
            }
        }
    }
    return  false;
}

Point Solver::chooseSafeCell() {
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.flags[i][j]==1){//is it still safe to use == given flags now is float?
                return Point{i,j};
            }
        }
    }
}

bool Solver::isInitialized() {
    return board.init;
}

Point Solver::ranFirstMove() {

    srand(time(NULL));
    int T = board.N*board.M;
    int t = rand() % T;
    int row = t/board.M;
    int col = t-row*board.M;
    return  Point{row,col};
}

Point Solver::corFirstMove() {
    if(!board.init){
        int row = board.N-1;
        int col = board.M-1;
        return Point{row,col};
    }
}

Point Solver::cenFirstMove() {
    if(!board.init){
		int row = (board.N-1)/2;
		int col = (board.M-1)/2;
		return Point{row,col};
	}
}

void Solver::sim() {
    NebSet nebSetNow = board.getNeb();
    int numOfNebs = nebSetNow.size();
    for(int i=0; i<numOfNebs; i++){
        Neb nebNow = nebSetNow[i];
        int boundarySize = nebNow.boundary.size();
        //for every boundary cell
        for(int j = 0;j<boundarySize;j++){
            Point cellNow = nebNow.boundary[j];
            int fringeCount = 0;
            //count its fringe cells
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (i == 0 && j == 0) continue;
                    Point np{cellNow.x+i,cellNow.y+j};
                    if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                    if (board.board[np.x][np.y] == -1) {
                        fringeCount++;
                    }
                }
            }

            //rule 1: If num of adjacent fringes equals to num of mines, then they are all mines, flag them!
            if(board.board[cellNow.x][cellNow.y] == fringeCount){
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 && j == 0) continue;
                        Point np{cellNow.x+i,cellNow.y+j};
                        if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                        if (board.board[np.x][np.y] == -1) {
                            //mark in the flag matrix
                            board.flags[np.x][np.y] = -1;
                            board.minesFound++;
                        }
                    }
                }
            }

            //rule 2:pick any uncovered cell around a 0-valued cell
            //handled by the game

            //rule3:if a k-valued cell is surrounding by k-flagged mines, other adjacent cells are safe
            int minesAround = 0;
            //count the already known mines around the boundary cell
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (i == 0 && j == 0) continue;
                    Point np{cellNow.x+i,cellNow.y+j};
                    if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                    if (board.flags[np.x][np.y] == -1)minesAround++;
                }
            }
            //if all mines are detected
            if(minesAround == board.board[cellNow.x][cellNow.y]){
                //all mines are detected, thus others are safe, mark the safe ones with 1
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 && j == 0) continue;
                        Point np{cellNow.x+i,cellNow.y+j};
                        if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                        if (board.flags[np.x][np.y] != 2 && board.flags[np.x][np.y]!=-1){//mark and return
                            board.flags[np.x][np.y] = 1;
                            return;//return as soon as safe cell is found
                        }
                    }
                }
            }
        }
    }
    return;
}

void Solver::csp() {
    NebSet nebSetNow = board.getNeb();
    int numOfNebs = nebSetNow.size();
    for (int i = 0; i < numOfNebs; i++) {
        Neb nebNow = nebSetNow[i];
        int fringeSize = nebNow.fringe.size();
        vector<int> configurations;
        int unsureInFringe = 0;
        //count unsure fringe cells in this neb
        for (int j = 0; j < fringeSize; j++) {
            if (board.flags[nebNow.fringe[j].x][nebNow.fringe[j].y] == 0) {
                unsureInFringe++;
            }
        }
        //use enu only when there are uncertain cells
        if (unsureInFringe != 0) {
            //try all combinations of these cells
            for (int k = 0; k < pow(2, unsureInFringe); k++) {
                //copy the flag board for messing around
                Matrix copiedFlagMatrix = board.flags;
                int unsureIndex = -1;
                //put in k(cconfiguration) to copiedFlagMatrix
                for (int j = 0; j < fringeSize; j++) {
                    //find unsure cell in flags
                    if (board.flags[nebNow.fringe[j].x][nebNow.fringe[j].y] ==
                        0) {//is it still safe to use == given flags now is float?
                        unsureIndex++;
                        int mine = k & (1 << unsureIndex);
                        if (mine) {
                            copiedFlagMatrix[nebNow.fringe[j].x][nebNow.fringe[j].y] = -1;
                        }
                        //copiedFlagMatrix[nebNow.fringe[j].x][nebNow.fringe[j].y] = 1;//not necessary, only count mines in copiedFlagMatrix
                    }
                }

                //test if this  config is compatible
                bool compatibility = true;
                int boundarySizeNow = nebNow.boundary.size();
                for (int b = 0; b < boundarySizeNow; b++) {
                    int bombsNearby = 0;
                    //count mines in the copiedFlagsMatrix
                    for (int p = -1; p < 2; p++) {
                        for (int q = -1; q < 2; q++) {
                            if (p == 0 && q == 0) continue;
                            Point np{nebNow.boundary[b].x + p, nebNow.boundary[b].y + q};
                            if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                            if (copiedFlagMatrix[np.x][np.y] == -1) bombsNearby++;
                        }
                    }
                    //if not compatible for this cell
                    if (board.board[nebNow.boundary[b].x][nebNow.boundary[b].y] != bombsNearby) {
                        compatibility = false;
                        break;
                    }
                }
                //if this is compatible, then and it in the configurations
                if (compatibility) {
                    configurations.push_back(k);
                }
            }
        }

        int numOfCompatible = configurations.size();
        int unsureIndex = -1;
        bool found = false;
        Point lastFound;
        for (int f = 0; f < fringeSize; f++) {
            //compute prob for each unsure cell
            if (board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] == 0) {
                unsureIndex++;
                int mineCfgCount = 0;
                for (int c = 0; c < numOfCompatible; c++) {
                    int mineOrNot = configurations[c] & (1 << unsureIndex);
                    if (mineOrNot) {
                        mineCfgCount++;
                    }
                }
//               int prob = mineCfgCount/numOfCompatible;
//               board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = -prob;
                if (mineCfgCount == numOfCompatible) {
                    board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = -1;
                    board.minesFound++;
                }
                if (mineCfgCount == 0) {
                    board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = 1;
                    //do not return here,return after mark all in this neighbor
                    found= true;
                    lastFound = Point{nebNow.fringe[f].x,nebNow.fringe[f].y};
                }
            }
        }
        if(found){
            return;
        }
    }
}

//Point Solver::mdp() {}

Point Solver::ranTieBreaking() {
    int riskyChoices = 0;
	for(int i=0;i<board.N;i++){
		for(int j=0;j<board.M;j++){
			if(board.flags[i][j]==0){
				riskyChoices++;
			}
		}
	}
	int choice = rand()%riskyChoices;
	for(int i=0;i<board.N;i++){
		for(int j=0;j<board.M;j++){
			if(board.flags[i][j]==0){
				riskyChoices--;
			}
			if(choice==riskyChoices){
               // cout<<"Kidding you, I'm just guessing around"<<endl;
				return Point{i,j};
			}
		}
	}
}

/*
vector<Point> Solver::lowProb() {
    DoubleMatrix copiedFlag = board.flags;
    vector<Point> probApproved;
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(copiedFlag[i][j]>0 && copiedFlag[i][j]< PROBTHRESHOLD){
                probApproved.push_back(Point{i,j});
            }
        }
    }
}
 */

Point Solver::awayFromBoundary() {
    Matrix dist = distToBoundary();
    int max=0;
    Point maxPoint;
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.flags[i][j]==0 && dist[i][j]>max){//is it still safe to use == given flags now is float?
                max = dist[i][j];
                maxPoint = Point{i,j};
            }
        }
    }
    return maxPoint;
}

Point Solver::awayFromBoundaryAndFringe() {
    Matrix dist = distToBoundary();
    int max=0;
    Point maxPoint;
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
             int distFrame = i<j?i:j;
            distFrame = distFrame<(7-i)?distFrame:(7-i);
            distFrame = distFrame<(7-j)?distFrame:(7-j);
            distFrame++;
            int minBetween = dist[i][j]<distFrame?dist[i][j]:distFrame;
            if(board.flags[i][j]==0 && minBetween>max){//is it still safe to use == given flags now is float?
                max = minBetween;
                maxPoint = Point{i,j};
            }
        }
    }
    return maxPoint;
}

Point Solver::closeToBoundary() {
    Matrix dist = distToBoundary();
    int min=1000;
    Point minPoint;
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.flags[i][j]==0 && dist[i][j]<min){//is it still safe to use == given flags now is float?
                min = dist[i][j];
                minPoint = Point{i,j};
            }
        }
    }
    return minPoint;
}

/*
Point Solver::closeToBoundaryAndFringe() {
    Matrix dist = distToBoundary();
    vector<Point> probApproved = lowProb();
    int min=1000;
    Point minPoint;

    if(probApproved.size()) {
        //when the size of the Prob Approved set is not 0,choose one with wanted distance from the approved set
        for (int k = 0; k < probApproved.size(); k++) {
            int i = probApproved[k].x;
            int j = probApproved[k].y;
            int distFrame = i < j ? i : j;
            distFrame = distFrame < (7 - i) ? distFrame : (7 - i);
            distFrame = distFrame < (7 - j) ? distFrame : (7 - j);
            distFrame++;
            int minBetween = dist[i][j] < distFrame ? dist[i][j] : distFrame;
            if (minBetween < min) {
                min = minBetween;
                minPoint = Point{i, j};
            }
        }
        return minPoint;
    }
    else{
        //when the size of the Prob Approved set is 0, choose the one with desired distance
        for(int i=0;i<board.N;i++){
            for(int j=0;j<board.M;j++){
                int distFrame = i<j?i:j;
                distFrame = distFrame<(7-i)?distFrame:(7-i);
                distFrame = distFrame<(7-j)?distFrame:(7-j);
                distFrame++;
                int minBetween = dist[i][j]<distFrame?dist[i][j]:distFrame;
                if(board.flags[i][j]!=2 && board.flags[i][j] !=-1 && minBetween<min){//is it still safe to use == given flags now is float?
                    min = minBetween;
                    minPoint = Point{i,j};
                }
            }
        }
        return minPoint;
    }
}
 */