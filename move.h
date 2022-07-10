#ifndef MOVE_H
#define MOVE_H
#include "position.h"
struct Move {
	// check if human player did not enter input(pretending to be computer)
	bool containsInput=true;
	Position from;
	Position to;
	bool isPromotion;
	char promotionType;
	// default constructor
	Move() : containsInput{false}, isPromotion{false}, promotionType{' '} {}
	// constructor for no promotion
	Move(Position from, Position to) : containsInput{true}, from{from}, to{to}, isPromotion{false}, promotionType{' '}{}
	// constructor for with promotion
	Move(Position from, Position to, char promotionType) : containsInput{true}, from{from}, to{to}, isPromotion{true}, promotionType{promotionType}{}
};
#endif