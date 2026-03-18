#include "GePointEnt2d.h"


GePointEnt2d::GePointEnt2d() {

}
GePointEnt2d::GePointEnt2d(const GePointEnt2d&) {

}

GePoint2d GePointEnt2d::point2d() const { 
    return GePoint2d::kOrigin;
}

GePointEnt2d::operator GePoint2d () const{
    return GePoint2d::kOrigin;
}
    
GePointEnt2d& GePointEnt2d::operator =  (const GePointEnt2d& pnt) { 
    return *this; 
}
