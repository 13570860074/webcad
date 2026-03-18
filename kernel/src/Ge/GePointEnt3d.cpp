#include "GePointEnt3d.h"


GePointEnt3d::GePointEnt3d() {

}
GePointEnt3d::GePointEnt3d(const GePointEnt3d&) {

}

GePoint3d GePointEnt3d::point3d() const { 
    return GePoint3d::kOrigin;
}

GePointEnt3d::operator GePoint3d () const{
    return GePoint3d::kOrigin;
}
    
GePointEnt3d& GePointEnt3d::operator =  (const GePointEnt3d& pnt) { 
    return *this; 
}
