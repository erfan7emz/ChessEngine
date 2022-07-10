#include "pawn.h"
#include "position.h"
#include "board.h"

Pawn::Pawn(Position p, int identifier, bool isFirstMove) : ChessPieces(p, identifier, isFirstMove){
    weight = 1;
    if(identifier==1) {
        icon='P';
    } else {
        icon='p';
    }
}

std::shared_ptr<ChessPieces> Pawn::clone( bool needToCheckSelfCheck) {
    return std::shared_ptr<ChessPieces>(new Pawn(*this,  needToCheckSelfCheck));
}
Pawn::Pawn(const Pawn &piece,  bool needToCheckSelfCheck) : ChessPieces(piece,  needToCheckSelfCheck){ }

// *promotion*
std::vector<PossibleMove> Pawn::getPossibleMoves(const Board & board) {
    checkOpponent=false;
    std::vector<PossibleMove> possMoves;
    int x = pos.x;
    int y = pos.y;
    std::vector<Position> candidates;
    if (ownerIdentifier == 1) {
        candidates.emplace_back(x, y+2);
        candidates.emplace_back(x, y+1);
        candidates.emplace_back(x+1, y+1);
        candidates.emplace_back(x-1, y+1);
    } else {
        candidates.emplace_back(x, y-2);
        candidates.emplace_back(x, y-1);
        candidates.emplace_back(x-1, y-1);
        candidates.emplace_back(x+1, y-1);
    }
    if (isFirstMove && board.isEmpty(candidates[0])&&board.isEmpty(candidates[1])) {
        tryAddNextMoveCandidate(board, possMoves, candidates[0]);
    }
    if (board.isEmpty(candidates[1])) {
        tryAddNextMoveCandidate(board, possMoves, candidates[1]);
    }
    if (!(board.isEmpty(candidates[2])) && board.isOpponentPiece(candidates[2], ownerIdentifier) != ' ') {
        tryAddNextMoveCandidate(board, possMoves, candidates[2]);
    }
    if (!(board.isEmpty(candidates[3])) && board.isOpponentPiece(candidates[3], ownerIdentifier)  != ' ') {
        tryAddNextMoveCandidate(board, possMoves, candidates[3]);
    }
    
    // en passant
    std::vector<Position> enPassantCaptureLoc;
    enPassantCaptureLoc.emplace_back(x+1,y);
    enPassantCaptureLoc.emplace_back(x-1,y);
    addEnPassantIfPoss(candidates[2], enPassantCaptureLoc[0], possMoves,board);
    addEnPassantIfPoss(candidates[2], enPassantCaptureLoc[1], possMoves,board);
    addEnPassantIfPoss(candidates[3], enPassantCaptureLoc[0], possMoves,board);
    addEnPassantIfPoss(candidates[3], enPassantCaptureLoc[1], possMoves,board);
    return possMoves;
}


void Pawn::addEnPassantIfPoss(Position to, Position captureLoc, std::vector<PossibleMove> & possMoves, const Board & board) {
    if(withinBound(to) && withinBound(captureLoc)) {
        char captureLocChar = board.isOpponentPiece(captureLoc,ownerIdentifier);
        if(board.isEmpty(to) && (!board.isEmpty(captureLoc)) && (captureLocChar=='p' || captureLocChar=='P')) {
            if(board.getPieceAt(captureLoc).get()->availableForEnPassant) {
                PossibleMove newPossMove;
                newPossMove.capture = captureLocChar;
                newPossMove.to = to;
                newPossMove.enPassant=true;
                newPossMove.enPassantLoc = captureLoc;
                if(!isCurrentPlayerKingInCheckAfterMove(newPossMove,board)) { 
                    possMoves.emplace_back(newPossMove);
                }
            }
        }
    }
    
}