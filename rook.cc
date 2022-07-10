#include "rook.h"
#include "position.h"

Rook::Rook(Position p, int identifier, bool isFirstMove) : ChessPieces(p, identifier, isFirstMove){
    weight = 5;
    if(identifier==1) {
        icon='R';
    } else {
        icon='r';
    }
}

std::shared_ptr<ChessPieces> Rook::clone( bool needToCheckSelfCheck) {
    return std::shared_ptr<ChessPieces>(new Rook(*this, needToCheckSelfCheck));
}
Rook::Rook(const Rook &piece,  bool needToCheckSelfCheck) : ChessPieces(piece, needToCheckSelfCheck){
    
}

std::vector<PossibleMove> Rook::getPossibleMoves(const Board & board) {
    checkOpponent=false;
    std::vector<PossibleMove> possMoves;
    int x =pos.x;
    int y =pos.y;
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
    return possMoves;
}