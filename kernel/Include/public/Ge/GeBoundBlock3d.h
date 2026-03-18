#ifndef GEBOUNDBLOCK3D_H
#define GEBOUNDBLOCK3D_H

#include "GeEntity3d.h"
#include "GePoint3d.h"
#include "GeEntity3dMethods.h"

class GeBoundBlock3d : public GeEntity3d
{
public:
    GeBoundBlock3d();
    GeBoundBlock3d(const GePoint3d& point1, const GePoint3d& point2);
    GeBoundBlock3d(const GePoint3d& base, const GeVector3d& dir1, const GeVector3d& dir2, const GeVector3d& dir3);
    GeBoundBlock3d(const GeBoundBlock3d& source);
    GEENTITY3D_METHODS(GeBoundBlock3d);

    /** \details
        Returns the extents of the bounding block.

        \param p1 [out]  Receives the first corner of the extents.
        \param p2 [out]  Receives the second corner of the extents.
    */
    void getMinMaxPoints(GePoint3d& p1, GePoint3d& p2) const;

    /** \details
        Returns base and sides of bounding block.

        \param base [out]  Receives the base of bounding block.
        \param side1 [out]  Receives the first side of bounding block.
        \param side2 [out]  Receives the second side of bounding block.
    */
    void get(GePoint3d& base, GeVector3d& dir1, GeVector3d& dir2, GeVector3d& dir3) const;

    /** \details
        Sets the bounding block to a coordinate-aligned box.

        \param p1 [in]  First point of a coordinate-aligned box.
        \param p2 [in]  Second point of a coordinate-aligned box.
    */
    GeBoundBlock3d& set(const GePoint3d& p1, const GePoint3d& p2);

    /** \details
        Sets the bounding block to a parallelogram bounding block.

        \param base [in]  Base of parallelogram bounding block.
        \param side1 [in]  First side of parallelogram bounding block.
        \param side2 [in]  Second side of parallelogram bounding block.
    */
    GeBoundBlock3d& set(const GePoint3d& base, const GeVector3d& dir1, const GeVector3d& dir2, const GeVector3d& dir3);

    /** \details
        Extends the bounding block to contain
        the specified point.

        \param point [in]  Any 2D point.
    */
    GeBoundBlock3d& extend(const GePoint3d& point);

    /** \details
        Moves the walls of the bounding block the specified distance.

        \param distance [in]  Distance.
    */
    GeBoundBlock3d& swell(double distance);

    /** \details
        Returns true if and only if the point is
        contained in the bounding block.

        \param point [in]  Any 2D point.
    */
    bool contains(const GePoint3d& point) const;

    /** \details
        Returns true if and only if specified bounding block
        does not intersect this bounding block.

        \param block [in]  Any 2D bounding block.
    */
    bool isDisjoint(const GeBoundBlock3d& block) const;

    GeBoundBlock3d& operator =(const GeBoundBlock3d& block);

    /** \details
        Returns true if and only if this bounding block is a
        coordinate-aligned box.
    */
    bool isBox() const;

    /** \details
        Sets this bounding block to a coordinate-aligned box, or a
        parallelogram bounding block. Returns a reference to this 
        bounding block.

        \param toBox [in]  If true, sets this bounding block to a 
            coordinate-aligned box; otherwise, sets it to a 
            parallelogram bounding block.
    */
    GeBoundBlock3d& setToBox(bool toBox);
};


#endif 

