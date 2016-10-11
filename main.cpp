#include"board.hpp"


//sim.rule1: flag obvious mines
//sim.rule2: pick any uncovered cell around a 0-valued cell
//sim.rule3: if a k-valued cell is surrounding by k-flagged mines, other adjacent cells are safe
//Enu: enumerate all solutions in the fringe, interrupt if execute more than 1 second

//board.getBoard;    board.getNeb
Point select(Board &board){
	int row,col;
	//first move:random, corner, center
	if(!board.init){
		row = (board.N-1)/2;
		col = (board.M-1)/2;
		return Point{row,col};
	}

	//non-decisive: Sim
	//return safe cell as long as there is one
	for(int i=0;i<board.N;i++){
		for(int j=0;j<board.M;j++){
			if(board.flags[i][j]==1){
				return Point{i,j};
			}
		}
	}

	NebSet nebSetNow = board.getNeb();
	int numOfNebs = nebSetNow.size();
	for(int i=0;i<numOfNebs;i++){
		Neb nebNow = nebSetNow[i];
		int boundarySize = nebNow.boundary.size();
		int fringeSize = nebNow.fringe.size();

		//for every boundary cell
		for(int j = 0;j<boundarySize;j++){
			Point cellNow = nebNow.boundary[j];
			int fringeCount = 0;
			//count its fringe cells
			for(int k = 0;k<fringeSize;k++){
				if((nebNow.fringe[k].x - cellNow.x)==1 ||(nebNow.fringe[k].x - cellNow.x)==-1
				   ||(nebNow.fringe[k].y - cellNow.y)==1||(nebNow.fringe[k].y - cellNow.y)==-1){
					fringeCount++;
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
			if(board.flags[i][j]==1){
				return Point{i,j};
			}
		}
	}


	//tie-breaking blocks, when there is no safe cell
	//ran; Lo; Aw; Awf; Cl;Clf
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
                cout<<"Kidding you, I'm just guessing around"<<endl;
				return Point{i,j};
			}
		}
	}
};

int main() {
	Board game(8, 8, 10);
	game.printBomb();
	game.print();
	while (!game.isEOG()) {
		Point now = select(game);
		cout<<now.x<<" "<<now.y<<endl;
		game.pick(now.x, now.y);
		game.print();
		cout <<"state is : "<< game.winOrLoss() << endl;
	}
	game.printBomb();
    game.printFlags();
    NebSet finalNeb = game.getNeb();
    int finalSize = finalNeb.size();
    for(int i=0;i<finalSize;i++){
        Neb nebNow = finalNeb[i];
        int fringeSize = nebNow.fringe.size();
        cout<<fringeSize<<endl;
    }

	return 0;
}