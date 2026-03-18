#ifndef GEBOUNDBLOCK2D_H
#define GEBOUNDBLOCK2D_H

#include "GeEntity2d.h"
#include "GeEntity2dMethods.h"


class GeBoundBlock2d : public GeEntity2d
{
public:
    GeBoundBlock2d();
    GeBoundBlock2d(const GePoint2d& point1, const GePoint2d& point2);
    GeBoundBlock2d(const GePoint2d& base, const GeVector2d& dir1, const GeVector2d& dir2);
    GeBoundBlock2d(const GeBoundBlock2d& source);
    GEENTITY2D_METHODS(GeBoundBlock2d);

    /** \details
        Returns the extents of the bounding block.

        \param p1 [out]  Receives the first corner of the extents.
        \param p2 [out]  Receives the second corner of the extents.
    */
    void getMinMaxPoints(GePoint2d& p1, GePoint2d& p2) const;

    /** \details
        Returns base and sides of bounding block.

        \param base [out]  Receives the base of bounding block.
        \param side1 [out]  Receives the first side of bounding block.
        \param side2 [out]  Receives the second side of bounding block.
    */
    void get(GePoint2d& base, GeVector2d& side1, GeVector2d& side2) const;

    /** \details
        Sets the bounding block to a coordinate-aligned box.

        \param p1 [in]  First point of a coordinate-aligned box.
        \param p2 [in]  Second point of a coordinate-aligned box.
    */
    GeBoundBlock2d& set(const GePoint2d& p1, const GePoint2d& p2);

    /** \details
        Sets the bounding block to a parallelogram bounding block.

        \param base [in]  Base of parallelogram bounding block.
        \param side1 [in]  First side of parallelogram bounding block.
        \param side2 [in]  Second side of parallelogram bounding block.
    */
    GeBoundBlock2d& set(const GePoint2d& base, const GeVector2d& side1, const GeVector2d& side2);

    /** \details
        Extends the bounding block to contain
        the specified point.

        \param point [in]  Any 2D point.
    */
    GeBoundBlock2d& extend(const GePoint2d& point);

    /** \details
        Moves the walls of the bounding block the specified distance.

        \param distance [in]  Distance.
    */
    GeBoundBlock2d& swell(double distance);

    /** \details
        Returns true if and only if the point is
        contained in the bounding block.

        \param point [in]  Any 2D point.
    */
    bool contains(const GePoint2d& point) const;

    /** \details
        Returns true if and only if specified bounding block
        does not intersect this bounding block.

        \param block [in]  Any 2D bounding block.
    */
    bool isDisjoint(const GeBoundBlock2d& block) const;

    GeBoundBlock2d& operator =(const GeBoundBlock2d& block);

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
    GeBoundBlock2d& setToBox(bool toBox);
};


#endif 

