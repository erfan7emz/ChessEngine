#include "queen.h"
#include "position.h"
#include "board.h"


Queen::Queen(Position p, int identifier, bool isFirstMove) : ChessPieces(p, identifier, isFirstMove) {
    weight = 9;
    if(identifier==1) {
        icon='Q';
    } else {
        icon='q';
    }
}

std::shared_ptr<ChessPieces> Queen::clone( bool needToCheckSelfCheck) {
    return std::shared_ptr<ChessPieces>(new Queen(*this,  needToCheckSelfCheck));
}
Queen::Queen(const Queen &piece,  bool needToCheckSelfCheck) : ChessPieces(piece, needToCheckSelfCheck){
    
}

std::vector<PossibleMove> Queen::getPossibleMoves(const Board & board) {
    checkOpponent=false;
    std::vector<PossibleMove> possMoves;
    int x =pos.x;
    int y =pos.y;
    // Rook moves:
    // check right
    for(int i=x+1; i<=HIGHER_BOUND; i++) {
        Position candidate{i,y};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
    }
    // check backwards
    for(int i=y-1; i>=1; i--) {
        Position candidate{x,i};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
    }

    // check left
    for(int i=x-1; i>=1; i--) {
        Position candidate{i,y};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
    }
    // check forward
    for(int i=y+1; i<=HIGHER_BOUND; i++) {
        Position candidate{x,i};
        int tryAddResult = tryAddNextMoveCandidate(board, possMoves, candidate);
        if(tryAddResult==-1||tryAddResult==1) {
            break;
        }
    }
    //Queen moves:
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