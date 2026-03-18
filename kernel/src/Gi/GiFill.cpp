#include "GiFill.h"
#include "GiImpl.h"


GiFill::GiFill() {
	this->mpImp = new GiImpFill();
}
GiFill::GiFill(const GiFill& fill) {
	this->mpImp = new GiImpFill();
	this->setDeviation(fill.deviation());
}
GiFill::~GiFill() {
	delete this->mpImp;
}

double GiFill::deviation() const {
	return this->mpImp->deviation;
}
void GiFill::setDeviation(double dev) {
	this->mpImp->deviation = dev;
}

