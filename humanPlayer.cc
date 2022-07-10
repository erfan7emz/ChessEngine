#include "humanPlayer.h"
#include "possibleMove.h"
#include "board.h"
#include "chessPieces.h"
#include <iostream>

HumanPlayer::HumanPlayer(int identifier, double currentScore) : Player{identifier,true, currentScore}{}
HumanPlayer::HumanPlayer(const HumanPlayer &humanPlayer,  bool needToCheckSelfCheck) : Player(humanPlayer, needToCheckSelfCheck){}
std::shared_ptr<Player> HumanPlayer::clone( bool needToCheckSelfCheck){
    return std::shared_ptr<Player>(new HumanPlayer(*this, needToCheckSelfCheck));
}

bool HumanPlayer::tryMakeMove(Move m, Board & board) {
    Position from = m.from;
    Position to = m.to;
    std::shared_ptr<ChessPieces> targetPiece = this->getPieceAt(from);
    
    std::vector<PossibleMove> allPossibleMoves = targetPiece.get()->getPossibleMoves(board);
    
    for(auto possMove : allPossibleMoves) {
        if(possMove.to == to) {
            if(possMove.kingSideCastle || possMove.queenSideCastle) {
                movePiece(possMove.rookFrom, possMove.rookTo, board);
            } 
            if(possMove.enPassant) { 
                // remove opponent pawn
                board.removePiece(possMove.enPassantLoc, opponentIdentifier);
            } 
             if(m.isPromotion) {
                bool checkPromotionResult = tryDoPawnPromotion(m.promotionType,targetPiece.get()->pos, targetPiece.get()->ownerIdentifier, board);
                if(!checkPromotionResult) {
                    return false;
                }
            }

            enPassantAvailabilityCorrect(board.getPieceAt(from), board, from, to);
            Position testPosition1{1,4};
            Position testPosition2{4,6};
            movePiece(from, to, board);
            return true;
        }
    }
    return false;
}

