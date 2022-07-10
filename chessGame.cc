#include "chessGame.h"
#include "player.h"
#include "move.h"
#include "board.h"
#include "position.h"
#include "observer.h"
#include "text_display.h"
#include "humanPlayer.h"
#include "computerPlayer.h"
#include <iostream>

ChessGame::ChessGame(bool isHuman1, bool isHuman2,  std::vector<int> levels) : currentPlayer{white} {
    std::shared_ptr<Player> playerOne;
    std::shared_ptr<Player> playerTwo;
    if(isHuman1) {
        playerOne = std::make_shared<HumanPlayer>(1);
    } else if(!isHuman1) {
        playerOne = std::make_shared<ComputerPlayer>(1, levels[0]);
    }
    if(isHuman2) {
        playerTwo = std::make_shared<HumanPlayer>(0);
    } else if(!isHuman2) {
        playerTwo = std::make_shared<ComputerPlayer>(0, levels[1]);
    }
    players.emplace_back(playerTwo);
    players.emplace_back(playerOne);
    board = std::make_shared<Board>(players);
    textObserver = std::make_shared<Text_Display>(this);
    observers.emplace_back(textObserver.get());
    render();
}

void ChessGame::setCurrentPlayer(int playerId) {
    currentPlayer = playerId;
}

void ChessGame::nextTurn() {
    if(currentPlayer==black) {
        currentPlayer=white;
    } else {
        currentPlayer=black;
    }
}

void ChessGame::newRound() {
    board.get()->resetBoard();
    setCurrentPlayer(white);
    render();
}

std::string ChessGame::resign() {
    if(currentPlayer==white) {
        players[black].get()->currentScore++;
        newRound();
        roundEnds=true;
        return "Black Wins!";
    } else {
        players[white].get()->currentScore++;
        newRound();
        roundEnds=true;
        return "White Wins!";
    }
}

double ChessGame::whitePlayerScore() {
    return players[white].get()->currentScore;
}

double ChessGame::blackPlayerScore() {
    return players[black].get()->currentScore;
}

std::string ChessGame::makeAMove(Move nextMove) {
    roundEnds=false;
    if((players[currentPlayer].get()->isHuman && !nextMove.containsInput)||(!players[currentPlayer].get()->isHuman&&nextMove.containsInput)) {
        return "Input not match player type!! Please enter again";
    }
    if(nextMove.containsInput) {
        char c = players[currentPlayer].get()->getPieceCharAt(nextMove.from);
        if(c==' ') {
            return "Only move your own piece! Try again";
        }
    }
    int moveResult = board.get()->makeAMove(nextMove, currentPlayer);
    
    std::string outputString="";
    if (moveResult!=-1) {
        switch (moveResult)
        {
        case 0:
            break;
        case 1:
            if(currentPlayer==white) {
                players[black].get()->isInCheck=true;
                outputString = "Black is in check.";
            } else {
                players[white].get()->isInCheck=true;
                outputString = "White is in check.";
            }
            break;
        case 2:
            players[currentPlayer].get()->currentScore++;
            if(currentPlayer==white) {
                outputString = "Checkmate! White wins!";
            } else {
                outputString = "Checkmate! Black wins!";
            }
            break;
        case 3:
            for(auto player:players) {
                player.get()->currentScore+=0.5;
                outputString = "Stalemate!";
            }
        default:
            break;
        }
        nextTurn();
        render();
        if(moveResult==2 || moveResult==3) {
            newRound();
            roundEnds=true;
        }
    } else {
        render();
        outputString = "Invalid move!! Please enter again";
    }
    return outputString;
}

void ChessGame::makeBlankBoard() {
    std::vector<int> change;
    for (int i = 1; i <= HIGHER_BOUND; ++i) {
        change.emplace_back(i);
    }
    for(int x:change) {
        for(int y:change) {
            Position p;
            p.x=x;
            p.y=y;
            removePiece(p);
        }  
    }
}

void ChessGame::notifyObservers() {
    for (auto ob: observers) {
        ob->update();
    }
}

void ChessGame::giveHintAt(Position focus, std::ostream& out) {
    std::shared_ptr<ChessPieces> piece;
    for(auto player:players) {
        piece = player.get()->getPieceAt(focus);
        if(piece!=nullptr) {
            break;
        }
    }
    
    if(piece==nullptr) {
        out << "no piece exist at input location!"<<'\n';
        return;
    } else if(piece.get()->ownerIdentifier!=currentPlayer){
        out << "Focus on your own piece please"<<'\n';
        return;
    } else {
        const char iniRef='a';
        auto possMoves = piece.get()->getPossibleMoves(*board.get());
        out << piece.get()->icon << " can move from " << (char)(piece.get()->pos.x-1+iniRef) << piece.get()->pos.y << " to: "<< '\n';
        if(possMoves.size()==0) {
            out << "No valid move exist!"<<'\n';
            return ;
        }
        Position bestMove = board.get()->getHint(focus, possMoves, currentPlayer);
        for(auto move : possMoves) {
            if(move.capture!=' ') {
                out << (char)(move.to.x-1+iniRef) << move.to.y  << " [capture]";
            } else {
                out << (char)(move.to.x-1+iniRef) << move.to.y;
            }    
            if(bestMove==move.to &&!(bestMove==focus)) {
                    out<< " *recommended*";
                }
            out << '\n';
        }
        return;
    }
}

bool ChessGame::removePiece(Position p) {
    if(board.get()->removePiece(p, white) || board.get()->removePiece(p, black)) {
        return true;
    }
    return false;
}
bool ChessGame::addPiece(Position p, char c) {
    return board.get()->addPiece(p, c);
}
bool ChessGame::isBoardSetupValid() {
    return board.get()->isBoardSetupValid();
}
char ChessGame::getPieceCharAt(Position p) {
    return board.get()->getPieceCharAt(p);
}

void ChessGame::render() {
    notifyObservers();
}