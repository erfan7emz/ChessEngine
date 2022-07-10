#include "king.h"
#include "position.h"
#include "board.h"
#include "move.h"
#include <iostream>

King::King(Position p, int identifier, bool isFirstMove) 
    : ChessPieces(p, identifier, isFirstMove){
    weight = 0;
    if(identifier==1) {
        icon='K';
    } else {
        icon='k';
    }
}

std::shared_ptr<ChessPieces> King::clone( bool needToCheckSelfCheck) {
    return std::shared_ptr<ChessPieces>(new King(*this, needToCheckSelfCheck));
}
King::King(const King &piece,  bool needToCheckSelfCheck) : ChessPieces(piece, needToCheckSelfCheck){
    
}

bool King::isKingNextToKing(Position candidate, const Board & board) {
    // check all surrounding 8 squares, make sure it is not original position
    // -1 0 1
    std::vector<int> change = {-1,0,1};
    for(int x:change) {
        for(int y:change) {
            Position neighbour;
            neighbour.x=x+candidate.x;
            neighbour.y=y+candidate.y;
            if(!(neighbour==candidate)&&!(neighbour==pos)) {
                char ifKing = board.getPieceCharAt(neighbour);
                if(ifKing=='k' || ifKing == 'K') {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<PossibleMove> King::getPossibleMoves(const Board & board) {
    checkOpponent=false;
    std::vector<PossibleMove> possMoves;
    int x =pos.x;
    int y =pos.y;
    std::vector<int> change = {-1,0,1};
    for(int x_change:change) {
        for(int y_change:change) {
            Position candidate;
            candidate.x=x+x_change;
            candidate.y=y+y_change;
            if(!(candidate==pos)) {
                if(!isKingNextToKing(candidate, board)) {
                    tryAddNextMoveCandidate(board, possMoves, candidate);
                }
            }
        }
    }

    // ****
    // CAN"T CASTLE OUT OF CHECK
    // ****
    if(!board.ifInCheck(ownerIdentifier)) {
        if (Kingsidecastle(board)) {
            Position newKCastle;
            newKCastle.y = pos.y;
            newKCastle.x = 7;
            Position rookF{HIGHER_BOUND,pos.y};
            Position rookT{HIGHER_BOUND - 2,pos.y};
            PossibleMove pm;
            pm.kingSideCastle = true;
            pm.rookFrom = rookF;
            pm.rookTo = rookT;
            pm.to = newKCastle;
            if(!isCurrentPlayerKingInCheckAfterMove(pm, board)) {
                possMoves.emplace_back(pm);
            }
        }
        if (Queensidecastle(board)) {
            Position newKCastle;
            newKCastle.y = pos.y;
            newKCastle.x = 3;
            Position rookF{1,pos.y};
            Position rookT{4,pos.y};
            PossibleMove pm;
            pm.queenSideCastle=true;
            pm.rookFrom = rookF;
            pm.rookTo = rookT;
            pm.to = newKCastle;
            if(!isCurrentPlayerKingInCheckAfterMove(pm, board)) {
                possMoves.emplace_back(pm);
            }
        }
    }
    return possMoves;
}


bool King::Queensidecastle(const Board & board) {
    for(int i = 2; i <= 4; i++) {
        Position shouldBeEmpty;
        shouldBeEmpty.x=i;
        shouldBeEmpty.y = pos.y;
        if(!board.isEmpty(shouldBeEmpty)) {
            return false;
        }
    }
    char rookChar;
    if(ownerIdentifier==1) {
        rookChar = 'R';
    } else {
        rookChar = 'r';
    }
    if (this->isFirstMove) {
        if (board.getPieceCharAt(Position{1, pos.y}) != rookChar) {
            return false;
        }
        if (!((board.getPieceAt(Position{1, pos.y}))->isFirstMove)) {
            return false;
        }
        Move newMove;
        newMove.from = Position{5, pos.y};
        newMove.to = Position{4, pos.y};
        if (isCurrentPlayerKingInCheckAfterMove(newMove, board)) {
            return false;
        }
        newMove.to = Position{3, pos.y};
        if (isCurrentPlayerKingInCheckAfterMove(newMove, board)) {
            return false;
        }
        return true;
    } 
    return false;
}

bool King::Kingsidecastle(const Board & board) {
    for(int i = 6; i <= HIGHER_BOUND - 1; i++) {
        Position shouldBeEmpty;
        shouldBeEmpty.x=i;
        shouldBeEmpty.y = pos.y;
        if(!board.isEmpty(shouldBeEmpty)) {
            return false;
        }
    }
    char rookChar;
    if(ownerIdentifier==1) {
        rookChar = 'R';
    } else {
        rookChar = 'r';
    }
    if (this->isFirstMove) {
        if (board.getPieceCharAt(Position{HIGHER_BOUND, pos.y}) != rookChar) {
            return false;
        }
        if (!((board.getPieceAt(Position{HIGHER_BOUND, pos.y}).get())->isFirstMove)) {
            return false;
        }
        Move newMove;
        newMove.from = Position{5, pos.y};
        newMove.to = Position{6, pos.y};
        if (isCurrentPlayerKingInCheckAfterMove(newMove, board)) {
            return false;
        }
        newMove.to = Position{7, pos.y};
        if (isCurrentPlayerKingInCheckAfterMove(newMove, board)) {
            return false;
        }
        return true;
    } 
    return false;
}
