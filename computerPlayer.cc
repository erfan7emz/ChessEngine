#include "computerPlayer.h"
#include "board.h"
#include <iostream>

ComputerPlayer::ComputerPlayer(int identifier, int level, double currentScore) 
: Player{identifier, false, currentScore}, level{level} {}

ComputerPlayer::ComputerPlayer(const ComputerPlayer &computerPlayer,  bool needToCheckSelfCheck) : Player(computerPlayer,  needToCheckSelfCheck){
    level = computerPlayer.level;
}
std::shared_ptr<Player> ComputerPlayer::clone( bool needToCheckSelfCheck) {
    return std::shared_ptr<Player>(new ComputerPlayer(*this, needToCheckSelfCheck));
}


PossibleMove ComputerPlayer::randomLegalMove(Board &board, Position &from_position) {

    std::map<std::shared_ptr<ChessPieces>, std::shared_ptr<std::vector<PossibleMove>>> choices = board.getPlayerPossibleMoves(identifier);
    int totalNumMoves = 0;
    // not possible to be 0, since either stalemate/checkmate detected at last round
    for(auto pieceSet:choices) {
        for(auto move : *pieceSet.second.get()) {
            totalNumMoves++;
        }
    }
    std::srand(std::time(nullptr));
    int random_index = std::rand() % totalNumMoves;
    for (auto pieceSet : choices) {
        std::shared_ptr<std::vector<PossibleMove>> possMoves = pieceSet.second;
        for(auto move : *possMoves.get()) {
            if(random_index==0) {
                PossibleMove nextMove = move;
                from_position = pieceSet.first.get()->pos;
                return nextMove;
            }
            random_index--;
        }
    }
    PossibleMove null;
    return null;
}

bool ComputerPlayer::tryMakeMove(Move move, Board & board) {
    if (level == 1) {
        Position from;
        PossibleMove nextMove = randomLegalMove(board, from);
        SimpleMakeMove(from, nextMove, board);
    }
    else if (level == 2) {
        // Call level 2 make move
        captureCheckPriorityMove(board);
    }
    else if (level == 3) {
        // Call level 3 make move
        avoidCapturePriorityMove(board);
    }
    else {
        // Call level 4 make move
        smartMove(board);
    }

    return true;
}

bool ComputerPlayer::SimpleMakeMove(Position currentPosition, PossibleMove nextMove, Board &board) {

    std::shared_ptr<ChessPieces> targetPiece = getPieceAt(currentPosition);
    if(targetPiece==nullptr) {
        std::cout <<"nullptr outer..." <<std::endl;
    }
    if (nextMove.kingSideCastle || nextMove.queenSideCastle) {
        movePiece(nextMove.rookFrom, nextMove.rookTo, board);
    }
    if (nextMove.enPassant) {
        board.removePiece(nextMove.enPassantLoc, opponentIdentifier);
    }
    if (nextMove.isPromotion) {
        bool checkPromotionResult = tryDoPawnPromotion(nextMove.promotionType,targetPiece.get()->pos, targetPiece.get()->ownerIdentifier, board);
        if(!checkPromotionResult) {
            return false;
        }
    }
    enPassantAvailabilityCorrect(targetPiece, board, currentPosition, nextMove.to);
    movePiece(currentPosition, nextMove.to, board);
    return true;

}


bool ComputerPlayer::captureCheckPriorityMove(Board & board) {
 
    // copy the board: 
    // start by checking if there's any available moves that can put the opponent in check
    // if there's no checking move available, the player will check if there's any capture move available
    // since neither option is available, the player will choose a random legal move
    std::map<std::shared_ptr<ChessPieces>, std::shared_ptr<std::vector<PossibleMove>>> choices = board.getPlayerPossibleMoves(identifier);
    for (auto const pieceSet : choices) {
        std::shared_ptr<std::vector<PossibleMove>> possMoves = pieceSet.second;
        Position starting_position = pieceSet.first.get()->pos;
        std::shared_ptr<ChessPieces> currentPiece = pieceSet.first;
        for(auto move : *possMoves.get()) {
            Position target_position = move.to;
            Board tempBoard{board};
            tempBoard.makeAMoveWithoutCheck(starting_position, target_position, identifier);
            if (tempBoard.putInCheck(opponentIdentifier).size()!=0) {
                SimpleMakeMove(starting_position, move, board);
                return true;
            }
            if (move.capture != ' ') {
                SimpleMakeMove(starting_position, move, board);
                return true;
            }
        }
    }
    // since there's no capture or check move available for the player, the player will make a random legal move from its current piece
    Position from;
    PossibleMove nextMove = randomLegalMove(board, from);
    SimpleMakeMove(from, nextMove, board);
    return true;

}


// returns true if the opponent can capture the piece at Position targetPosition
bool ComputerPlayer::OpponentCaptureAvailable(Board &board, Position targetPosition) {

    auto opponentAllMoves = board.getPlayerPossibleMoves(opponentIdentifier);
    for (auto i: opponentAllMoves) {
        for (auto j: *i.second) {
            if (j.capture != ' ' && j.to == targetPosition) {
                return true;
            }
        }
    }
    return false;

}

PossibleMove ComputerPlayer::escapeCapture(Board &board, Position p) {

    std::shared_ptr<ChessPieces> piece = board.getPieceAt(p);
    std::map<std::shared_ptr<ChessPieces>, std::shared_ptr<std::vector<PossibleMove>>> allPossibleMoves = board.getPlayerPossibleMoves(identifier);
    std::shared_ptr<std::vector<PossibleMove>> escapeChoices = allPossibleMoves[piece];
    if (escapeChoices->size() == 0) {
        // can't avoid the capture, returning a null position
        // return Position{-1, -1}; 
        PossibleMove escape{escapeChoices->at(0)};
        escape.to = Position{-1, -1};
    }
    return escapeChoices->at(0);
}


bool ComputerPlayer::avoidCapturePriorityMove(Board &board) {

    for (auto i: playerPieces) {
        Position piecePosition = i.first;
        bool opponentCanCapture = OpponentCaptureAvailable(board, piecePosition);
        if (opponentCanCapture) {
            PossibleMove escapeMove = escapeCapture(board, piecePosition);
            Position nullPosition{-1, -1};
            if (escapeMove.to == nullPosition) {
                // can't escape
            } else {
                SimpleMakeMove(piecePosition, escapeMove, board);
                return true;
            }
        }
        // we can move on checking on the next piece
    }

    // at this point, the player finished avoiding capture since either:
        // the player detected capture and avoided it,
        // the player detected capture and can't avoid it
        // the player's pieces won't be captured for the moment

    captureCheckPriorityMove(board);

    return true;

}

bool ComputerPlayer::smartMove(Board &board) {

    std::map<std::shared_ptr<ChessPieces>, std::shared_ptr<std::vector<PossibleMove>>> allPossibleMoves = board.getPlayerPossibleMoves(identifier);
    int playerValue = board.getPlayerAllPiecesWeight(identifier);
    int opponentValue = board.getPlayerAllPiecesWeight(opponentIdentifier);
    int bestValue = playerValue - opponentValue;
    int currentValue;
    Position moveFrom;
    PossibleMove moveTo = randomLegalMove(board, moveFrom);
    for (auto i: allPossibleMoves) {
        for (auto j: *i.second) {
            Board tempBoard{board};
            Position starting_position = i.first->pos;
            Position target_position = j.to;
            tempBoard.makeAMoveWithoutCheck(starting_position, target_position, identifier);
            // the algorithm still prefers check over anything else, since check is the most important move of all moves in the game 
                // so if there's a check move available for the player, then the player has found the best move for this round;
            if (tempBoard.putInCheck(opponentIdentifier).size()!=0) {
                std::cout << "check move found: that's the best move" << std::endl;
                moveFrom = i.first.get()->pos;
                moveTo = j;
                break;
            }
            playerValue = tempBoard.getPlayerAllPiecesWeight(identifier);
            opponentValue = tempBoard.getPlayerAllPiecesWeight(opponentIdentifier);
            std::cout << "player value: " << playerValue << std::endl;
            std::cout << "opponent value: " << opponentValue << std::endl;
            currentValue = playerValue - opponentValue;
            if (currentValue > bestValue) {
                bestValue = currentValue;
                moveFrom = i.first.get()->pos;
                moveTo = j;
            }
        }
    }
    std::cout << "best value possible is: " << bestValue << std::endl;
    std::cout << "moving to " << moveTo.to.x << "|" << moveTo.to.y << std::endl;
    std::cout << "from " <<  moveFrom.x << "|" << moveFrom.y << std::endl;
    SimpleMakeMove(moveFrom, moveTo, board);
    return true;
}
