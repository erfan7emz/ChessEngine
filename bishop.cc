#include "bishop.h"
#include "position.h"
#include "board.h"

Bishop::Bishop(Position p, int identifier, bool isFirstMove) : ChessPieces(p, identifier, isFirstMove){
    weight = 3;
    if(identifier==1) {
        icon='B';
    } else {
        icon='b';
    }
}

std::shared_ptr<ChessPieces> Bishop::clone( bool needToCheckSelfCheck) {
    return std::shared_ptr<ChessPieces>(new Bishop(*this, needToCheckSelfCheck));
}
Bishop::Bishop(const Bishop &piece,  bool needToCheckSelfCheck) : ChessPieces(piece, needToCheckSelfCheck){
    
}

std::vector<PossibleMove> Bishop::getPossibleMoves(const Board & board) {
    checkOpponent=false;
    std::vector<PossibleMove> possMoves;
    int x =pos.x;
    int y =pos.y;
    // check up right
    int j = y + 1;
    for(int i=x+1; i<=HIGHER_BOUND; i++) {
        Position candidate{i,j};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if (j > HIGHER_BOUND) {break;}
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
        ++j;
    }
    // check up left
    int k = y + 1;
    for(int i=x-1; i>=1; i--) {
        if (k > HIGHER_BOUND) {break;}
        Position candidate{i,k};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
        ++k;
    }

    // check down left
    int p = y - 1;
    for(int i=x-1; i>=1; i--) {
        if (p < 1) {break;}
        Position candidate{i,p};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
        --p;
    }
    // check down right
    int q = y - 1;
    for(int i=x+1; i<=HIGHER_BOUND; i++) {
        if (q < 1) {break;}
        Position candidate{i,q};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
        --q;
    }
    return possMoves;
}