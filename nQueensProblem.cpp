#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <stack>
#include <algorithm>

using namespace std;

class NonAttackingConfig;
void printChessBoard(std::vector<std::vector<int>> &);
stack<NonAttackingConfig> getNonAttackingQueenConfig(int );
bool isPositionAttackedByAnotherQueen(vector<vector<int>> &  ,  pair<int, int> &);
pair<int, int> getASafePositionForTheQueen( vector<vector<int>> &, pair<int, int>);


inline int getOneDimPosition(pair<int,int> twoDimPosition, int gridSize){
    return twoDimPosition.first * gridSize + twoDimPosition.second ;
}

inline pair<int,int> getTwoDimPosition(int oneDimPosition, int gridSize){

    return make_pair(oneDimPosition / gridSize, oneDimPosition % gridSize) ;
}

int main()
{
  getNonAttackingQueenConfig(5) ; //change the parameter for  different N queen problems.. 
  return 0 ;
}

class NonAttackingConfig {

    //let i,j represent a position of one queen
    //there will be such n queens
    //need a vector of pairs
    private:
     vector<vector<int>> chessBoardConfig;
     pair<int, int> lastPlacementMade;

    public:
     NonAttackingConfig(vector<vector<int>> chessBoardConfig, pair<int, int> lastPlacementMade ){
         this->chessBoardConfig = chessBoardConfig;
         this->lastPlacementMade = lastPlacementMade ;
     }

     vector<vector<int>> getConfig(){
         return this->chessBoardConfig;
     }

     pair<int, int> getLastPlacementMade(){
         return this->lastPlacementMade;
     }
    
};

 stack<NonAttackingConfig> getNonAttackingQueenConfig(int n){

    vector<vector<int>> chessBoard(n, vector<int>(n));

    stack<NonAttackingConfig> chessBoardConfigStack ;

    pair<int, int> positionToScanFrom = make_pair(0,0);

    int i = 0 ;
    while( i < n ){

        pair<int, int> position = getASafePositionForTheQueen(chessBoard, positionToScanFrom) ;
        if(position.first == -1){
            //we did not get a safe position for the queen
            //need to pop from the stack and start trying again
            // find the furthest position used. that will ensure that we ask further logic to not try that position
            NonAttackingConfig lastNonAttackingConfig = chessBoardConfigStack.top() ;
            vector<vector<int>>  boardConfigBeforeMakingMove = lastNonAttackingConfig.getConfig() ;
            pair<int , int> moveMadeLastTime = lastNonAttackingConfig.getLastPlacementMade();
            chessBoardConfigStack.pop();
            i = i - 1 ; 
            chessBoard = boardConfigBeforeMakingMove;
            int oneDimIndexOfMoveMadeLastTime = getOneDimPosition(moveMadeLastTime, n);
            int oneDimIndexToScanFromNextAttempt = oneDimIndexOfMoveMadeLastTime + 1;
            positionToScanFrom = getTwoDimPosition(oneDimIndexToScanFromNextAttempt, n) ;
            cout << "After undoing the last move: \n";
            printChessBoard(chessBoard) ;
            cout << " Now trying other positions starting from (" << positionToScanFrom.first << " , " << positionToScanFrom.second << " )\n" ; 
            
        }else{

            vector<vector<int>> currBoardConfig(n, vector<int>(n)) ;
            currBoardConfig.assign(chessBoard.begin(), chessBoard.end()) ;
            chessBoardConfigStack.push( NonAttackingConfig(currBoardConfig, position)   ) ;
            chessBoard[position.first][position.second] = 1;
            i = i + 1 ;
            int oneDimIndexOfMoveMadeLastTime = getOneDimPosition(position, n) ;
            int oneDimIndexToScanFromNextAttempt = oneDimIndexOfMoveMadeLastTime + 1;
            positionToScanFrom =  getTwoDimPosition(oneDimIndexToScanFromNextAttempt, n) ;
            cout << "After placing Queen " << i << " at ( " << position.first << " , " << position.second << " )\n";
            printChessBoard(chessBoard) ;
            
        }

    }

    return chessBoardConfigStack;
}


void printChessBoard(vector<vector<int>> &chessBoard){

    for (int i = 0; i < chessBoard.size(); i++)
    {
       for (int j = 0; j < chessBoard[i].size(); j++)
       {
        cout << chessBoard[i][j] << " ";
       }
       cout << "\n" ;
    }
}


pair<int, int> getASafePositionForTheQueen( vector<vector<int>> &chessBoard, pair<int,int> positionToScanFrom ){

    pair<int, int> pos; 

    for (int i = positionToScanFrom.first; i < chessBoard.size(); i++)
    {
       for (int j = positionToScanFrom.second; j < chessBoard[i].size(); j++)
       {
           if(chessBoard[i][j] == 1){
               continue; //already occupied
           }else{
              pos = make_pair(i,j);
              if(isPositionAttackedByAnotherQueen(chessBoard, pos)){
                  cout << "Position ( " << pos.first << " , " << pos.second << ") is under attack \n " ;
                  continue;
              } 

              return pos ; 

           }
       }
       
    } 

   return make_pair(-1,-1) ; //indicates no position 

}


bool isPositionAttackedByAnotherQueen(vector<vector<int>> &chessBoard  ,  pair<int, int> &pos){

    int posRow = pos.first ;
    int posCol = pos.second ;

    for(int j=0; j < chessBoard.size(); j++){
        if(chessBoard[posRow][j] == 1){
            return true; //means there is another queen in this row
        }         
    }

    for(int i=0; i < chessBoard.size(); i++){
        if(chessBoard[i][posCol] == 1){
            return true; //means there is another queen in this col
        }         
    }

    //lets find the top left corner for the diagonal , and also find the top right corener of diagonal
    int lower = posRow < posCol ? posRow : posCol ;
    pair<int,int> topLeftPos = make_pair(posRow-lower, posCol - lower) ;

    int i = topLeftPos.first;
    int j = topLeftPos.second;

    //scan from top left position (first diagonal)
    while( i < chessBoard.size() && j < chessBoard.size() ){

        if(chessBoard[i][j] == 1){
            return true ;
        }

        i = i + 1;
        j = j + 1;
    }

    int numberToConsider = (posRow + posCol ) > ( chessBoard.size() - 1 ) ? ( posRow + posCol) % ( chessBoard.size() - 1 ) : posRow ;
    pair<int,int> topRightPos = make_pair(posRow-numberToConsider, posCol + numberToConsider) ;

    int m = topRightPos.first;
    int n = topRightPos.second;

    //scan from top right position
    while( m < chessBoard.size() && n >= 0 ){

        
        if(chessBoard[m][n] == 1){
            return true ;
        }

        m = m + 1;
        n = n - 1;
    }

    return false;

}