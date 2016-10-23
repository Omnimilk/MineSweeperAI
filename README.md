# MineSweeperAI
##How to read the abbreviations of strategy?
The first part represents how to choose the first move, ran means choosing one randomly, cor means choosing one at the cornor of the board, cen means chooding one at the center of the board.    

The second part is the all capital letter part, representing which inference methods are used. SIM for simple rules, CSP for CSP methods and MDP for MDP methods.    

The last part has the lowest priotiry, used when must choose one.Currently only implemented ran which means randomly choose one in the uncovered cells.    

##Compile and run
compile with:  g++ *.cpp -std=c++11 -o MineSweeper    

run with:     ./MineSweeper

