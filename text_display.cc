#include "text_display.h"
#include "position.h"

Text_Display::Text_Display(ChessGame *g) {Game = g;}

void Text_Display::update() {

    for (int i = 8; i >= 1; i--) {
        out << i << " ";
        for (int j = 1; j <= 8; j++) {
            Position coordinate;
            coordinate.x = j;
            coordinate.y = i;
            char c = Game->getPieceCharAt(coordinate);
            if (c==' ') {
                if ((coordinate.x % 2 ==0 && coordinate.y % 2 ==0)
                || (coordinate.x % 2 !=0 && coordinate.y % 2 !=0)) {
                    c =  '_';
                }
            }
            out << c;
        }
        out << std::endl;
    }
    out << "" << std::endl;
    out << "  abcdefgh" << std::endl;
}
