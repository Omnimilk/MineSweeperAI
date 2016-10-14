#include"board.hpp"


//sim.rule1: flag obvious mines
//sim.rule2: pick any uncovered cell around a 0-valued cell
//sim.rule3: if a k-valued cell is surrounding by k-flagged mines, other adjacent cells are safe
//Enu: enumerate all solutions in the fringe, interrupt if execute more than 1 second
//distance
Matrix dist2bd(const NebSet &nebSetNow,Board &board){
    vector<Point> boundary;
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

//board.getBoard;    board.getNeb
Point select(Board &board){
	int row,col;
	//first move:random, corner, center
    //random
//    if(!board.init){
//        srand(time(NULL));
//        int T = board.N*board.M;
//        int t = rand() % T;
//        int row = t/board.M;
//        int col = t-row*board.M;
//        return  Point{row,col};
//    }

    //cornor gives the best performance
    if(!board.init){
        row = board.N-1;
        col = board.M-1;
        return Point{row,col};
    }

    //center
//	if(!board.init){
//		row = (board.N-1)/2;
//		col = (board.M-1)/2;
//		return Point{row,col};
//	}

    //if there are safe cells, then choose them
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.flags[i][j]==1){//is it still safe to use == given flags now is float?
                return Point{i,j};
            }
        }
    }

	//non-decisive: Sim has three simple rules
	NebSet nebSetNow = board.getNeb();
	int numOfNebs = nebSetNow.size();
	for(int i=0;i<numOfNebs;i++){
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
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (i == 0 && j == 0) continue;
                    Point np{cellNow.x+i,cellNow.y+j};
                    if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                    if (board.flags[np.x][np.y] == -1)minesAround++;
                }
            }
            if(minesAround == board.board[cellNow.x][cellNow.y]){
                //all mines are detected, thus others are safe, mark the safe ones with 1
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        if (i == 0 && j == 0) continue;
                        Point np{cellNow.x+i,cellNow.y+j};
                        if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                        if (board.flags[np.x][np.y] != 2 && board.flags[np.x][np.y]!=-1){//mark and return
                            board.flags[np.x][np.y] = 1;
                            return Point{np.x,np.y};
                        }
                    }
                }
            }
		}
	}

    //if sim can not give an answer, then use Enu
   // Enu: enumerate all solutions in the fringe, interpret if execute over 1 sec
    //enu for whole board if unknown cells is less than 10, otherwise enu for each fringe
    int totalUnknown = 0;
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.board[i][j]!=2 && board.board[i][j]!=-1 ){
                totalUnknown++;
            }
        }
    }

    if(totalUnknown>12){
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
                //DoubleMatrix copiedFlagMatrix = board.flags;
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
//        bool found = false;
//        Point lastFound;
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
                if (mineCfgCount == numOfCompatible) {
                    board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = -1;
                    board.minesFound++;
                }
                if (mineCfgCount == 0) {
                    board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = 1;
                   return nebNow.fringe[f];
                }
                double prob = 1.0*mineCfgCount/(numOfCompatible+0.01);
                board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = prob;
            }
        }

//        if(found){
//            return lastFound;
//        }
    }
    }
    else{
        //enu in the whole board
        vector<int> configurations;
        for (int k = 0; k < pow(2, totalUnknown); k++) {
            //copy the flag board for messing around
            //DoubleMatrix copiedFlagMatrix = board.flags;
            Matrix copiedFlagMatrix = board.flags;
            int unsureIndex = -1;
            //put in k(cconfiguration) to copiedFlagMatrix
            for (int i=0;i<board.N;i++){
                for(int j=0;j<board.M;j++){
                    //find unsure cell in flags
                    if (board.flags[i][j] !=2 && board.flags[i][j] !=-1) {//is it still safe to use == given flags now is float?
                        unsureIndex++;
                        int mine = k & (1 << unsureIndex);
                        if (mine) {
                            copiedFlagMatrix[i][j] = -1;
                        }
                        //copiedFlagMatrix[nebNow.fringe[j].x][nebNow.fringe[j].y] = 1;//not necessary, only count mines in copiedFlagMatrix
                    }
                }
            }

            //test if this  config is compatible
            bool compatibility = true;

            for (int i=0;i<board.N;i++){
                for(int j=0;j<board.M;j++){
                    int bombsNearby = 0;
                    //count mines in the copiedFlagsMatrix
                    for (int p = -1; p < 2; p++) {
                        for (int q = -1; q < 2; q++) {
                            if (p == 0 && q == 0) continue;
                            Point np{i + p, j + q};
                            if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                            if (copiedFlagMatrix[np.x][np.y] == -1) bombsNearby++;
                        }
                    }
                    //if not compatible for this cell
                    if (board.board[i][j] != bombsNearby) {
                        compatibility = false;
                        break;
                    }
                }
            }


            //if this is compatible, then and it in the configurations
            if (compatibility) {
                configurations.push_back(k);
            }
        }

        int numOfCompatible = configurations.size();
        int unsureIndex = -1;
//        bool found = false;
//        Point lastFound;
        for (int i=0;i<board.N;i++){
            for(int j=0;j<board.M;j++){
                //compute prob for each unsure cell
                if (board.flags[i][j] != 2 && board.flags[i][j] != -1) {
                    unsureIndex++;
                    int mineCfgCount = 0;
                    for (int c = 0; c < numOfCompatible; c++) {
                        int mineOrNot = configurations[c] & (1 << unsureIndex);
                        if (mineOrNot) {
                            mineCfgCount++;
                        }
                    }
                    if (mineCfgCount == numOfCompatible) {
                        board.flags[i][j] = -1;
                        board.minesFound++;
                    }
                    if (mineCfgCount == 0) {
                        board.flags[i][j] = 1;
                        return Point{i,j};
                    }
                    double prob = 1.0*mineCfgCount/(numOfCompatible+0.01);
                    board.flags[i][j] = prob;
                }
            }
        }
    }

    //use global constraint that there are m mines in total in the board
//    DoubleMatrix copiedFlag = board.flags;
//    int numOfUnsensed =0;
//    for(int i=0;i<board.N;i++){
//        for(int j=0;j<board.M;j++){
//            if(board.flags[i][j]==0){
//                numOfUnsensed++;
//            }
//        }
//    }
//    for(int i=0;i<board.N;i++){
//        for(int j=0;j<board.M;j++){
//            if(board.flags[i][j]==0){
//                copiedFlag[i][j] = 1.0*(board.K-board.minesFound)/numOfUnsensed;
//            }
//        }
//    }

	//tie-breaking blocks, when there is no safe cell
	//ran; Lo; Aw; Awf; Cl;Clf
	//ran: picks a cell randomly
    //Lo: prefers cells c minimizing pm(c)
    //Aw: prefers cells away from the boundary
    //Awf: prefers cells away from the boundary and the fringe
    //Cl: prefers cells c close to the boundary
    //Clf: prefers cells c close the boundary or the fringe
    //BEST PERFORMANCE! rSimEnuLoClf; rSimEnuLoAw; rSimEnuAwLo

    //ran
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



    //Lo, prob first
//    double threhold = 0.21;
//    vector<Point> probApproved;
//    for(int i=0;i<board.N;i++){
//        for(int j=0;j<board.M;j++){
//            if(copiedFlag[i][j]>0 && copiedFlag[i][j]<threhold ){
//               probApproved.push_back(Point{i,j});
//            }
//        }
//    }

    //Matrix dis_bd(const NebSet &nebSetNow,Board &board)
    //Aw, max d(c,boundary)
//    Matrix dist = dis_bd(nebSetNow,board);
//    int max=0;
//    Point maxPoint;
//    for(int i=0;i<board.N;i++){
//        for(int j=0;j<board.M;j++){
//            if(board.flags[i][j]==0 && dist[i][j]>max){//is it still safe to use == given flags now is float?
//                max = dist[i][j];
//                maxPoint = Point{i,j};
//            }
//        }
//    }
//    return maxPoint;
    //Awf, max min(d(c,boundary),d(c,frame))
//    Matrix dist = dis_bd(nebSetNow,board);
//    int max=0;
//    Point maxPoint;
//    for(int i=0;i<board.N;i++){
//        for(int j=0;j<board.M;j++){
//             int distFrame = i<j?i:j;
//            distFrame = distFrame<(7-i)?distFrame:(7-i);
//            distFrame = distFrame<(7-j)?distFrame:(7-j);
//            distFrame++;
//            int minBetween = dist[i][j]<distFrame?dist[i][j]:distFrame;
//            if(board.flags[i][j]==0 && minBetween>max){//is it still safe to use == given flags now is float?
//                max = minBetween;
//                maxPoint = Point{i,j};
//            }
//        }
//    }
//    return maxPoint;

    //Cl, min d(c,boundary)
//    Matrix dist = dis_bd(nebSetNow,board);
//    int min=1000;
//    Point minPoint;
//    for(int i=0;i<board.N;i++){
//        for(int j=0;j<board.M;j++){
//            if(board.flags[i][j]==0 && dist[i][j]<min){//is it still safe to use == given flags now is float?
//                min = dist[i][j];
//                minPoint = Point{i,j};
//            }
//        }
//    }
//    return minPoint;

    //Clf, min min(d(c,boundary),d(c,frame))
//    Matrix dist = dist2bd(nebSetNow,board);
//    int min=1000;
//    Point minPoint;
//    if(probApproved.size()) {
//        for (int k = 0; k < probApproved.size(); k++) {
//            int i = probApproved[k].x;
//            int j = probApproved[k].y;
//            int distFrame = i < j ? i : j;
//            distFrame = distFrame < (7 - i) ? distFrame : (7 - i);
//            distFrame = distFrame < (7 - j) ? distFrame : (7 - j);
//            distFrame++;
//            int minBetween = dist[i][j] < distFrame ? dist[i][j] : distFrame;
//            if (minBetween < min) {
//                min = minBetween;
//                minPoint = Point{i, j};
//            }
//        }
//        return minPoint;
//    }
//    else{
//        for(int i=0;i<board.N;i++){
//            for(int j=0;j<board.M;j++){
//                 int distFrame = i<j?i:j;
//                distFrame = distFrame<(7-i)?distFrame:(7-i);
//                distFrame = distFrame<(7-j)?distFrame:(7-j);
//                distFrame++;
//                int minBetween = dist[i][j]<distFrame?dist[i][j]:distFrame;
//                if(board.flags[i][j]==0 && minBetween<min){//is it still safe to use == given flags now is float?
//                    min = minBetween;
//                    minPoint = Point{i,j};
//                }
//            }
//        }
//        return minPoint;
//    }

}
//
//int main() {
//    //take m mines in the whole board as a constrain!!!!!!!
//        Board game(8, 8, 10);
//        while (!game.isEOG()) {
//            Point now = select(game);
//            cout << now.x << " " << now.y << endl;
//            game.pick(now.x, now.y);
//            game.print();
//            game.printFlags();
//            cout << "state is : " << game.winOrLoss() << endl;
//        }
//    game.printBomb();
//    cout<<"====================================================================================================================================="<<endl;
//	return 0;
//}

int main() {
    int numOfTest = 1000;
    int winningCase = 0;
    while (numOfTest > 0) {
        Board game(8, 8, 10);
        while (!game.isEOG()) {
            Point now = select(game);
            //cout << now.x << " " << now.y << endl;
            game.pick(now.x, now.y);
            //game.print();
            //cout << "state is : " << game.winOrLoss() << endl;
            int winornot = game.winOrLoss();
            if (winornot == 1) winningCase++;
        }
        numOfTest--;
    }
    cout << "Wining " << winningCase << " games." << endl;
    return 0;
}
