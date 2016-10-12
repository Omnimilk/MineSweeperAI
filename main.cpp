#include"board.hpp"


//sim.rule1: flag obvious mines
//sim.rule2: pick any uncovered cell around a 0-valued cell
//sim.rule3: if a k-valued cell is surrounding by k-flagged mines, other adjacent cells are safe
//Enu: enumerate all solutions in the fringe, interrupt if execute more than 1 second

//board.getBoard;    board.getNeb
Point select(Board &board){
	int row,col;
	//first move:random, corner, center
    //random
    if(!board.init){
        srand(time(NULL));
        int T = board.N*board.M;
        int t = rand() % T;
        int row = t/board.M;
        int col = t-row*board.M;
        return  Point{row,col};
    }
    //cornor gives the best performance
//    if(!board.init){
//        row = board.N-1;
//        col = board.M-1;
//        return Point{row,col};
//    }

    //center
//	if(!board.init){
//		row = (board.N-1)/2;
//		col = (board.M-1)/2;
//		return Point{row,col};
//	}

	//non-decisive: Sim
	//return safe cell as long as there is one
	for(int i=0;i<board.N;i++){
		for(int j=0;j<board.M;j++){
			if(board.flags[i][j]==1){//is it still safe to use == given flags now is float?
				return Point{i,j};
			}
		}
	}

	NebSet nebSetNow = board.getNeb();
	int numOfNebs = nebSetNow.size();
	for(int i=0;i<numOfNebs;i++){
		Neb nebNow = nebSetNow[i];
		int boundarySize = nebNow.boundary.size();
		//int fringeSize = nebNow.fringe.size();

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
			if(minesAround ==board.board[nebNow.boundary[j].x][nebNow.boundary[j].y]){
				//all mines are detected, thus others are sage, mark the safe ones with 1
				for (int i = -1; i < 2; i++) {
					for (int j = -1; j < 2; j++) {
						if (i == 0 && j == 0) continue;
						Point np{cellNow.x+i,cellNow.y+j};
						if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
						if (board.flags[np.x][np.y] == 0)board.flags[np.x][np.y] = 1;//mark unknown neb(0) as safe(1)
					}
				}
			}
		}
	}

	//if there are safe cells, then choose them
	for(int i=0;i<board.N;i++){
		for(int j=0;j<board.M;j++){
			if(board.flags[i][j]==1){//is it still safe to use == given flags now is float?
				return Point{i,j};
			}
		}
	}


    //if sim can not give an answer, then use Enu
    //Enu: enumerate all solutions inthe fringe, interpret if execute over 1 sec
    for(int i=0;i<numOfNebs;i++){
        Neb nebNow = nebSetNow[i];
        int fringeSize = nebNow.fringe.size();
        vector<int> configurations;
        int unsureInFringe = 0;
        for(int j=0;j<fringeSize;j++){
            if(board.flags[nebNow.fringe[j].x][nebNow.fringe[j].y] ==0){
                unsureInFringe++;
            }
        }

        for(int k=0;k<pow(2,unsureInFringe);k++){
            //copy the flag board for messing around
            DoubleMatrix copiedFlagMatrix = board.flags;
            int unsureIndex = -1;
            //put in k(cconfiguration) to copiedFlagMatrix
            for(int j=0;j<fringeSize;j++){
                //find unsure cell in flags
                if(board.flags[nebNow.fringe[j].x][nebNow.fringe[j].y] ==0) {//is it still safe to use == given flags now is float?
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
            for(int r=0;r<board.N;r++){
                if(compatibility) {
                    for (int c = 0; c < board.M; c++) {
                        //count bombs for each cell in this config
                        int bombsNearby = 0;
                        for (int p = -1; p < 2; p++) {
                            for (int q = -1; q < 2; q++) {
                                if (p == 0 && q == 0) continue;
                                Point np{r + p, c + q};
                                if (np.x < 0 || np.x >= board.N || np.y < 0 || np.y >= board.M) continue;
                                if (copiedFlagMatrix[np.x][np.y] == -1) bombsNearby++;
                            }
                        }
                        //if not compatible for this cell
                        if (board.board[r][c] != bombsNearby) {
                            compatibility = false;
                            break;
                        }
                    }
                }
            }

            //if this is compatible, then and it in the configurations
            if(compatibility){
                configurations.push_back(k);
            }
        }

        int numOfCompatible = configurations.size();
        int unsureIndex =-1;
        for(int f=0;f<fringeSize;f++){
            //compute prob for each unsure cell
            if(board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] ==0){
                unsureIndex++;
                int mineCfgCount = 0;
                for(int c=0;c<numOfCompatible;c++){
                    int mineOrNot = configurations[c]&(1<<unsureIndex);
                    if(mineOrNot){
                        mineCfgCount++;
                    }
                }
                float prob = mineCfgCount*1.0/numOfCompatible;
                board.flags[nebNow.fringe[f].x][nebNow.fringe[f].y] = prob;
            }
        }
    }

    //if there are safe cells, then choose them
    for(int i=0;i<board.N;i++){
        for(int j=0;j<board.M;j++){
            if(board.flags[i][j]==1){//is it still safe to use == given flags now is float?
                return Point{i,j};
            }
        }
    }

	//tie-breaking blocks, when there is no safe cell
	//ran; Lo; Aw; Awf; Cl;Clf
	//ran: picks a cell randomly
    //Lo: prefers cells c minimizing pm(c)
    //Aw: prefers cells away from the boundary
    //Awf: prefers cells away from the boundary and the fringe
    //Cl: prefers cells c close to the boundary
    //Clf: prefers cells c close the boundary or the fringe
    //BEST PERFORMANCE! rSimEnuLoClf; rSimEnuLoAw; rSimEnuAwLo
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
                //cout<<"Kidding you, I'm just guessing around"<<endl;
				return Point{i,j};
			}
		}
	}
};

int main() {

	//game.printBomb();
	//game.print();
        Board game(8, 8, 10);
        while (!game.isEOG()) {
            Point now = select(game);
            game.printFlags();
            cout << now.x << " " << now.y << endl;
            game.pick(now.x, now.y);
            game.print();
            cout << "state is : " << game.winOrLoss() << endl;
        }
    cout<<"========================================================================"<<endl;
	return 0;
}

//int main() {
//
//    //game.printBomb();
//    //game.print();
//    int numOfTest = 10000;
//    int winningCase = 0;
//    while (numOfTest > 0) {
//        Board game(8, 8, 10);
//        while (!game.isEOG()) {
//            Point now = select(game);
//            //cout << now.x << " " << now.y << endl;
//            game.pick(now.x, now.y);
//            //game.print();
//            //cout << "state is : " << game.winOrLoss() << endl;
//            int winornot = game.winOrLoss();
//            if (winornot == 1) winningCase++;
//        }
//
//        numOfTest--;
//    }
//
//    cout << "Wining " << winningCase << " games." << endl;
////	game.printBomb();
////    game.printFlags();
////    NebSet finalNeb = game.getNeb();
////    int finalSize = finalNeb.size();
////    for(int i=0;i<finalSize;i++){
////        Neb nebNow = finalNeb[i];
////        int fringeSize = nebNow.fringe.size();
////        cout<<fringeSize<<endl;
////    }
//
//    return 0;
//}
