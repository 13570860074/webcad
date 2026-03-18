#ifndef GIFILL_H
#define GIFILL_H

#include "RxObject.h"

class GiImpFill;
class  GiFill : public RxObject
{
public:
    GiFill();
    GiFill(const GiFill&);
    virtual ~GiFill();
    RX_DECLARE_MEMBERS(GiFill);

    double deviation() const;
    void   setDeviation(double dev);

private:
    GiImpFill* mpImp;
};



#endif