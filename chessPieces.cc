#include "chessPieces.h"
#include "position.h"
#include "move.h"
#include <iostream>
int HIGHER_BOUND = 8;

ChessPieces::ChessPieces(Position p, int identifier, bool isFirstMove) 
    : ownerIdentifier{identifier}, isFirstMove{isFirstMove}, pos{p}{}

ChessPieces::ChessPieces(const ChessPieces &chessPieces,  bool needToCheckSelfCheck) {
    pos = chessPieces.pos;
    icon = chessPieces.icon;
    isFirstMove = chessPieces.isFirstMove;
    checkOpponent  = chessPieces.checkOpponent;
    availableForEnPassant = chessPieces.availableForEnPassant;
    ownerIdentifier = chessPieces.ownerIdentifier;
    this->needCheckSelfCheck = needToCheckSelfCheck;
    this->weight = chessPieces.weight;
}

void ChessPieces::afterFirstMove() {
    if(isFirstMove) {
        isFirstMove = false;
    }
}

bool ChessPieces::withinBound(Position candidate) {
    return candidate.x <= HIGHER_BOUND && candidate.y <= HIGHER_BOUND && candidate.x >=1 && candidate.y >=1;
}
void ChessPieces::setPos(const Position newPos) {
    this->pos=newPos;
}

int ChessPieces::tryAddNextMoveCandidate(const Board & board, std::vector<PossibleMove> & possibleMove, Position candidate) {
    char returnKey = -1;
    if(withinBound(candidate)) {
        const char noCapture = ' ';
        const char currentPlayerPiece = ' ';
        if(board.isEmpty(candidate)) {
            PossibleMove newPossMove;
            newPossMove.capture = noCapture;
            newPossMove.to = candidate;
            if(pos.x==1 && pos.y==2 && candidate.x==1 && candidate.y == 4) {

            }
            bool checkResult = isCurrentPlayerKingInCheckAfterMove(newPossMove,board);
            if(!checkResult) {
                if((icon == 'p'||icon=='P') && (candidate.y == HIGHER_BOUND || candidate.y == 1)) {
                    newPossMove.isPromotion=true;
                    if(ownerIdentifier==1) {
                        newPossMove.promotionType='Q';
                    } else{
                        newPossMove.promotionType='q';
                    }
                } 
                possibleMove.emplace_back(newPossMove);
                returnKey = 0;
            } else {
                returnKey = -2;
            }
            
        } else {
            char pieceResult = board.isOpponentPiece(candidate, ownerIdentifier);
            if(pieceResult == currentPlayerPiece) {
                return returnKey;
            } else {
                PossibleMove newPossMove;
                newPossMove.capture = pieceResult;
                newPossMove.to = candidate;
                if(!isCurrentPlayerKingInCheckAfterMove(newPossMove,board)) {
                    if((icon == 'p'||icon=='P') && (candidate.y == HIGHER_BOUND || candidate.y == 1)) {
                        newPossMove.isPromotion=true;
                        if(ownerIdentifier==1) {
                            newPossMove.promotionType='Q';
                        } else{
                            newPossMove.promotionType='q';
                        }
                    }
                    if(pieceResult=='K'||pieceResult=='k') {
                        checkOpponent = true;
                    }    
                    possibleMove.emplace_back(newPossMove);
                    returnKey = 1;
                } else {
                    returnKey = -2;
                }
            }
        }
    }
    return returnKey;
}

bool ChessPieces::isCurrentPlayerKingInCheckAfterMove(Move newMove, const Board & board) {
    if(needCheckSelfCheck) {
        if(!(withinBound(newMove.from) && withinBound(newMove.to))) {
        throw;
        }
        // **Make a copy of the board
        Board newBoard(board, false);
        newBoard.makeAMoveWithoutCheck(newMove.from, newMove.to, ownerIdentifier);
        return (newBoard.putInCheck(ownerIdentifier).size()!=0);
    }
    return false;
}

bool ChessPieces::isCurrentPlayerKingInCheckAfterMove(PossibleMove possMove, const Board & board) {
    if(needCheckSelfCheck) {
                int opponentIdentifier;
        if(ownerIdentifier==1) {
            opponentIdentifier=0;
        } else {
            opponentIdentifier=1;
        }
        if(!(withinBound(possMove.to))) {
            throw;
            return false;
        }
        // **Make a copy of the board
        Board newBoard(board, false);
        if(possMove.kingSideCastle || possMove.queenSideCastle) {
            newBoard.makeAMoveWithoutCheck(possMove.rookFrom, possMove.rookTo, ownerIdentifier);
        } 
        if(possMove.enPassant) { 
            // remove opponent pawn
            newBoard.removePiece(possMove.enPassantLoc, opponentIdentifier);
        }
        newBoard.makeAMoveWithoutCheck(pos, possMove.to, ownerIdentifier);
        auto checkResult = newBoard.putInCheck(ownerIdentifier);
        return (checkResult.size()!=0);
    } 
    return false;
}