#include "aStarElement.h"

void aStarElement::aStarElement(string t, double c, int p) {
	cost(c);
	trl(t);
	pos(p);
}

double aStarElement::getCost() {
	return this.cost;
}

std:string aStarElement::getTrl() {
	return this.trl;
}

int aStarElement::getPos() {
	return this.pos;
}

void aStarElement::setCost(double c) {
	this.cost = c;
}

void aStarElement::setTrl(std:string t) {
	this.trl = t;
}

void aStarElement::setPos(int p) {
	this.pos = p;
}
