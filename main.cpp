#include "Board.hpp"
#include "Solver.hpp"
#include "Strategy.hpp"

int main() {
    //take m mines in the whole board as a constrain!!!!!!!
    int mode,strategynum,difficulty;
    cout<<"Welcome to MineSweeper AI"<<endl;
    cout<<"Please choose the strategy you want to try out: "<<endl;
    cout<<"1 for corSimRan, 2 for corSimCSPRan, 3 for corSimCSPMDPRan"<<endl;
    cout<<"4 for ranSimRan, 5 for ranSimCSPRan, 6 for ranSimCSPMDPRan"<<endl;
    cout<<"7 for cenSimRan, 8 for cenSimCSPRan, 9 for cenSimCSPMDPRan"<<endl;
    cin>>strategynum;
    while(strategynum!=1 && strategynum!=2 && strategynum!=3 && strategynum!=4 && strategynum!=5 && strategynum!=6 && strategynum!=7 && strategynum!=8 && strategynum!=9){
        cout<<"Wrong number, please enter again"<<endl;
        cin>>strategynum;
    }
    cout<<"Please choose the mode you want to use, 1 for step-by-step mode, 2 for success rate test mode:"<<endl;
    cin>>mode;
    while(mode!=1 && mode!=2){
        cout<<"Wrong number, please enter again"<<endl;
        cin>>mode;
    }
    cout<<"Please choose difficulty level, 1 for easy, 2 for middle:"<<endl;
    cin>>difficulty;
    while(difficulty!=1 && difficulty!=2){
        cout<<"Wrong number, please enter again"<<endl;
        cin>>difficulty;
    }


    if(mode == 1){
        Board game;
        if(difficulty ==2){
             game = Board(14, 14, 30);
        }
        while (!game.isEOG()) {
            Point choice;
            Solver solver(game);
            Strategy strategy(solver);
            switch (strategynum){
                case 1:choice = strategy.corSimCSPRan();
                case 2:choice = strategy.corSimCSPRan();
                case 3:choice = strategy.corSimCSPMDPRan();
                case 4:choice = strategy.ranSimRan();
                case 5:choice = strategy.ranSimCSPRan();
                case 6:choice = strategy.ranSimCSPMDPRan();
                case 7:choice = strategy.cenSimRan();
                case 8:choice = strategy.cenSimCSPRan();
                case 9:choice = strategy.cenSimCSPMDPRan();
            }

            cout << choice.x << " " << choice.y << endl;
            game.pick(choice.x, choice.y);
            game.print();
            cout << "state is : " << game.winOrLoss() << endl;
        }
        game.printBomb();
    }
    else{
        cout<<"Please wait, this can take a few minutes"<<endl;
        int numOfTest = 100;
        int winningCase = 0;
        while (numOfTest > 0) {
            Board game;
            if(difficulty ==2){
                game = Board(14, 14, 30);
            }
            while (!game.isEOG()) {
                Point choice;
                Solver solver(game);
                Strategy strategy(solver);
                switch (strategynum){
                    case 1:choice = strategy.corSimCSPRan();
                    case 2:choice = strategy.corSimCSPRan();
                    case 3:choice = strategy.corSimCSPMDPRan();
                    case 4:choice = strategy.ranSimRan();
                    case 5:choice = strategy.ranSimCSPRan();
                    case 6:choice = strategy.ranSimCSPMDPRan();
                    case 7:choice = strategy.cenSimRan();
                    case 8:choice = strategy.cenSimCSPRan();
                    case 9:choice = strategy.cenSimCSPMDPRan();
                }
                game.pick(choice.x, choice.y);
                int winornot = game.winOrLoss();
                if (winornot == 1) winningCase++;
            }
            numOfTest--;
        }
        cout << "Wining " << winningCase << " games out of 100 games." << endl;
    }

	return 0;
}

