#include "chessGame.h"
#include "move.h"
#include "position.h"
#include <string>
#include <iostream>
#include <sstream>
#include <memory>

int main() {
    std::unique_ptr<ChessGame>chess;
    std::string line;
    bool gameHasStarted = false;
    bool isFirstTime = true;
    bool inSetUp = false;
    bool isSetUpAllowed=true;
    while (std::getline(std::cin, line)) {
        std::string command;
        std::stringstream ss(line);
        ss >> command;
        std::string moveResult;
        if (!inSetUp) {
            if (command == "game") {
                std::string game1;
                std::string game2;
                std::vector<int> levels{0,0};
                bool isHuman1 = true;
                bool isHuman2 = true;
                if (ss >> game1) {
                    if (game1 == "human") {
                    } else if (game1 ==  "computer[1]") {
                        isHuman1 = false;
                        levels[0] = 1;
                    } else if (game1 == "computer[2]") {
                        isHuman1 = false;
                        levels[0] = 2;
                    } else if (game1 == "computer[3]") {
                        isHuman1 = false;
                        levels[0] = 3;
                    } else if (game1 == "computer[4]") {
                        isHuman1 = false;
                        levels[0] = 4;
                    } else {
                        std::cout << "The input for white player is not valid" << std::endl;
                        continue;
                    }
                } else {
                    std::cout << "No white player is given" << std::endl;
                    continue;
                }
                if (ss >> game2) {
                    if (game2 == "human") {
                    } else if (game2 ==  "computer[1]") {
                        isHuman2 = false;
                        levels[1] = 1;
                    } else if (game2 ==  "computer[2]") {
                        isHuman2 = false;
                        levels[1] = 2;
                    } else if (game2 ==  "computer[3]") {
                        isHuman2 = false;
                        levels[1] = 3;
                    } else if (game2 ==  "computer[4]") {
                        isHuman2 = false;
                        levels[1] = 4;
                    } else {
                        std::cout << "The input for black player is not valid" << std::endl;
                        continue;
                    }
                } else {
                    std::cout << "No black player is given" << std::endl;
                    continue;
                }
                chess = std::make_unique<ChessGame>(isHuman1, isHuman2, levels);
                gameHasStarted = true;
            } else if (command == "resign"&&gameHasStarted) {
                std::cout << chess->resign() << std::endl;
            } else if (command == "hint" &&gameHasStarted){
                std::string from_str;
                Position from;
                if(ss>>from_str) {
                    from.x = from_str[0] - 'a' + 1;
                    from.y = from_str[1] - '0';
                    chess->giveHintAt(from,std::cout);
                }
            } else if (command == "move"&&gameHasStarted) {
                isSetUpAllowed=false;
                std::string from_str;
                std::string to_str;
                Position from;
                Position to;
                Move nextMove;
                if (ss >> from_str && ss >> to_str) {
                    from.x = from_str[0] - 'a' + 1;
                    from.y = from_str[1] - '0';
                    to.x = to_str[0] - 'a' + 1;
                    to.y = to_str[1] - '0';
                    nextMove.from = from;
                    nextMove.to = to;
                    char promo;
                    if (ss >> promo) {
                        nextMove.isPromotion = true;
                        nextMove.promotionType = promo;
                        Move nextMove(from , to, promo);
                        moveResult = chess->makeAMove(nextMove);
                    } else {
                        Move nextMove(from, to);
                        moveResult = chess->makeAMove(nextMove);
                    }
                } else {
                    Move nextMove;
                    moveResult = chess->makeAMove(nextMove);
                }
                if(chess.get()->roundEnds) {
                    isSetUpAllowed=true;
                }
                std::cout << moveResult << std::endl;
            } else if (command == "setup") {
                if (gameHasStarted&&isSetUpAllowed) {
                    inSetUp = true;
                    if (isFirstTime) {
                        chess->makeBlankBoard();
                        isFirstTime=false;
                    }
                    chess->render();
                    continue;
                } else if (!gameHasStarted){
                    std::cout << "Game has not started yet, can't enter setup!" << std::endl;
                } else {
                    std::cout << "Game is still running, can't enter setup!" << std::endl;
                }
            } else {
                std::cout<<"Error, the given input is not valid"<<std::endl;
            }
        } else {
            if (command == "done") {
                if (chess->isBoardSetupValid()) {
                    inSetUp = false;
                    std::cout<<"Successfully exited setup mode!"<<std::endl;
                } else {
                    std::cout << "Cannot leave set up mode yet" << std::endl;
                }
            } else if (command == "+") {
                char p;
                std::string pos_str;
                if (ss >> p >> pos_str) {
                Position pos;
                pos.x = pos_str[0] - 'a' + 1;
                pos.y = pos_str[1] - '0';
                if(chess->addPiece(pos, p)) {
                    chess->render();
                } else {
                    std::cout << "Error, chess piece not valid" << std::endl;
                }
                
                } else {
                    std::cout << "Error, the command should be in the form '+ k e1'" << std::endl;
                }  
            } else if (command == "-") {
                std::string pos_str;
                if (ss >> pos_str) {
                    Position pos;
                    pos.x = pos_str[0] - 'a' + 1;
                    pos.y = pos_str[1] - '0';
                    if(chess->removePiece(pos)) {
                        chess->render();
                    } else {
                        std::cout << "Error, chess piece not valid" << std::endl;
                    }
                    } else {
                        std::cout << "Error, the command should be in the form '- e1'" << std::endl;
                    }
            } else if (command == "=") {
                std::string colour;
                if (ss >> colour) {
                    if (colour == "black") {
                        chess->setCurrentPlayer(0);
                        std::cout << "set to black success" << std::endl;
                    } else if (colour == "white") {
                        chess->setCurrentPlayer(1);
                        std::cout << "set to white success" << std::endl;
                    } else {
                        std::cout << "Next player should be black or white?" << std::endl;
                    }
                } else {
                    std::cout << "Error, the command should be in the form '= colour'" << std::endl;
                }
            } else {
                std::cout << "Error, the given input is not valid" << std::endl;
            }
        } 
    }
    if(gameHasStarted) {
        std::cout << "Final Score:" << std::endl;
        std::cout << "White: " << chess->whitePlayerScore() << std::endl;
        std::cout << "Black: " << chess->blackPlayerScore() << std::endl;
    }

}
