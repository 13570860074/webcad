import Ac from './Ac'
import Em from './Em'


declare namespace Module {


    enum EntityId {
        kEntity2d,
        kEntity3d,
        kPointEnt2d,
        kPointEnt3d,
        kPosition2d,
        kPosition3d,
        kPointOnCurve2d,
        kPointOnCurve3d,
        kPointOnSurface,
        kBoundedPlane,
        kCircArc2d,
        kCircArc3d,
        kConic2d,
        kConic3d,
        kCurve2d,
        kCurve3d,
        kEllipArc2d,
        kEllipArc3d,
        kLine2d,
        kLine3d,
        kLinearEnt2d,
        kLinearEnt3d,
        kLineSeg2d,
        kLineSeg3d,
        kPlanarEnt,
        kPlane,
        kRay2d,
        kRay3d,
        kSurface,
        kSphere,
        kCylinder,
        kTorus,
        kCone,
        kSplineEnt2d,
        kPolyline2d,
        kAugPolyline2d,
        kNurbCurve2d,
        kDSpline2d,
        kCubicSplineCurve2d,
        kSplineEnt3d,
        kPolyline3d,
        kAugPolyline3d,
        kNurbCurve3d,
        kDSpline3d,
        kCubicSplineCurve3d,
        kTrimmedCrv2d,
        kCompositeCrv2d,
        kCompositeCrv3d,
        kExternalSurface,
        kNurbSurface,
        kTrimmedSurface,
        kOffsetSurface,
        kEnvelope2d,
        kCurveBoundedSurface,
        kExternalCurve3d,
        kExternalCurve2d,
        kSurfaceCurve2dTo3d,
        kSurfaceCurve3dTo2d,
        kExternalBoundedSurface,
        kCurveCurveInt2d,
        kCurveCurveInt3d,
        kBoundBlock2d,
        kBoundBlock3d,
        kOffsetCurve2d,
        kOffsetCurve3d,
        kPolynomCurve3d,
        kBezierCurve3d,
        kObject,
        kFitData3d,
        kHatch,
        kTrimmedCurve2d,
        kTrimmedCurve3d,
        kCurveSampleData,
        kEllipCone,
        kEllipCylinder,
        kIntervalBoundBlock,
        kClipBoundary2d,
        kExternalObject,
        kCurveSurfaceInt,
        kSurfaceSurfaceInt,
        kHelix,

        kSpunSurface,
        kSegmentChain2d,

        kRevolvedSurface,
        kDummy1,
        kDummy2,
        kDummy3,
        kDummy4,
        kDummy5,
        kDummy6,
        kDummy7,
        kDummy8,
        kDummy9
    }

    enum ExternalEntityKind {
        kAcisEntity = 0, // External Entity is an ACIS entity
        kGe3dCurveEntity = 1, // External Entity is an GeCurve3d* entity
        kGe3dSurfaceEntity = 2, // External Entity is an GeSurface3d* entity
        kExternalEntityUndefined,      // External Entity is undefined. 
        kBimEntity
    }
    enum PointContainment {
        kInside,              // Point is inside the boundary.
        kOutside,             // Point is outside the boundary.
        kOnBoundary           // Point on the boundary.
    }

    enum OffsetCrvExtType {
        kFillet,
        kChamfer,
        kExtend
    }
    enum GeXConfig {
        kNotDefined = 1 << 0,
        kUnknown = 1 << 1,
        kLeftRight = 1 << 2,
        kRightLeft = 1 << 3,
        kLeftLeft = 1 << 4,
        kRightRight = 1 << 5,
        kPointLeft = 1 << 6,
        kPointRight = 1 << 7,
        kLeftOverlap = 1 << 8,
        kOverlapLeft = 1 << 9,
        kRightOverlap = 1 << 10,
        kOverlapRight = 1 << 11,
        kOverlapStart = 1 << 12,
        kOverlapEnd = 1 << 13,
        kOverlapOverlap = 1 << 14
    }

    enum BooleanType {
        kUnion,                          // Union
        kSubtract,                       // Subtraction
        kCommon                          // Intersection
    }
    enum ClipError {
        eOk,                             // OK
        eInvalidClipBoundary,            // Invalid Clip Boundary
        eNotInitialized                  // Clip Boundary was not Initialized
    }

    enum ClipCondition {
        kInvalid,                        // An error occurred, probably due to invalid initialization of clipping object.
        kAllSegmentsInside,              // All segments are inside the clip boundary.
        kSegmentsIntersect,              // At least one segment crosses the clip boundary.
        kAllSegmentsOutsideZeroWinds,    // The clip boundary is outside, and not encircled by, the clip boundary.
        kAllSegmentsOutsideOddWinds,     // The clip boundary is inside, and encircled by, the clip boundary.    
        kAllSegmentsOutsideEvenWinds     // The clip boundary is outside, but, encircled by, the clip boundary.    
    }

    /**
    */
    enum ErrorCondition {
        kOk,                                // OK
        k0This,                             // This object is 0.
        k0Arg1,                             // Argument 1 is 0.
        k0Arg2,                             // Argument 2 is 0.
        kPerpendicularArg1Arg2,             // Arguments 1 and 2 are perpendicular to each other.
        kEqualArg1Arg2,                     // Arguments 1 and 2 are equal.
        kEqualArg1Arg3,                     // Arguments 1 and 3 are equal.
        kEqualArg2Arg3,                     // Arguments 2 and 3 are equal.
        kLinearlyDependentArg1Arg2Arg3,     // Arguments 1, 2, and 3 are linearly dependent.
        kArg1TooBig,                        // Argument 1 is too big.
        kArg1OnThis,                        // Argument 1 is on this object.
        kArg1InsideThis,                    // Argument 1 is inside this object.
        kNonCoplanarGeometry,               // Geometry is not coplanar.
        kDegenerateGeometry,                // Geometry is degenerate.
        kSingularPoint                      // Geometry is one point.
    }

    enum csiConfig {
        kXUnknown,              // Unknown.
        kXOut,                  // Transverse -- Curve neighborhood is outside this surface.
        kXIn,                   // Transverse -- Curve neighborhood is inside this surface.
        kXTanOut,               // Tangent -- Curve neighborhood is outside this surface.
        kXTanIn,                // Tangent -- Curve neighborhood is inside this surface.
        kXCoincident,           // Non-zero length -- Point is on the intersection boundary.
        kXCoincidentUnbounded   // Non-zero length -- Point is on an arbitrary *point* on an unbounded intersection.
    }

    enum ssiType {
        kSSITransverse,         // Non-tangent intersection.
        kSSITangent,            // Tangent intersection with surface normals codirectional at any point within the component.
        kSSIAntiTangent         // Tangent intersection with surface normals antidirectional at any point within the component.
    }

    enum ssiConfig {
        kSSIUnknown,          // Unknown.
        kSSIOut,              // Neighborhood is outside this surface.
        kSSIIn,               // Neighborhood is inside this surface.
        kSSICoincident        // Non-zero area intersection.
    }

    enum GeIntersectError {
        kXXOk,                      // OK
        kXXIndexOutOfRange,         // Index out of range
        kXXWrongDimensionAtIndex,   // Wrong dimension at index.
        kXXUnknown                  // Unknown.
    }


    enum GeKnotParameterization {
        kChord = 0x0,   //Chord parametrization
        kSqrtChord = 0x1,   //Chord square root parametrization
        kUniform = 0x2,   //Uniform parametrization
        kCustomParameterization = 0xF,   //Custom parametrization
        kNotDefinedKnotParam = 0x10   //Knot parametrization isn't defined
    }


    class GeIntArray extends Ac.AcArray {
        constructor();
    }

    class GeDoubleArray extends Ac.AcArray {
        constructor();
    }

    class GeVector2dArray extends Ac.AcArray {
        constructor();
    }

    class GeVector3dArray extends Ac.AcArray {
        constructor();
    }

    class GePoint2dArray extends Ac.AcArray {
        constructor();
    }

    class GePoint3dArray extends Ac.AcArray {
        constructor();
    }

    class GeCurve2dPtrArray extends Ac.AcArray {
        constructor();
    }

    class GeCurve3dPtrArray extends Ac.AcArray {
        constructor();
    }

    class GeLineSeg2dArray extends Ac.AcArray {
        constructor();
    }

    class GeLineSeg3dArray extends Ac.AcArray {
        constructor();
    }



    //GeTol
    class GeTol {
        constructor(_t: number);
        constructor(_t1: number, _t2: number);
        equalPoint(): number;
        equalVector(): number;
        setEqualPoint(val: number): void;
        setEqualVector(val: number): void;
    }

    //GeInterval
    class GeInterval extends Em.EmObject{
        constructor();
        constructor(tol: number);
        constructor(lower: number, upper: number);
        constructor(lower: number, upper: number, tol: number);

        // Get/set methods.
        //
        lowerBound(): number;
        upperBound(): number;
        element(): number;
        length(): number;
        tolerance(): number;

        set(lower: number, upper: number): GeInterval;
        set(boundedBelow: boolean, bound: number): GeInterval;
        set(): GeInterval;
        setUpper(upper: number): GeInterval;
        setLower(lower: number): GeInterval;
        setTolerance(tol: number): GeInterval;

        // Interval editing.
        //
        getMerge(otherInterval: GeInterval, result: GeInterval): void;
        subtract(otherInterval: GeInterval, lInterval: GeInterval, rInterval: GeInterval): number;
        intersectWith(otherInterval: GeInterval, result: GeInterval): boolean;

        // Interval characterization.
        //
        isBounded(): boolean;
        isBoundedAbove(): boolean;
        isBoundedBelow(): boolean;
        isUnBounded(): boolean;
        isSingleton(): boolean;

        // Relation to other intervals.
        //
        isDisjoint(otherInterval: GeInterval): boolean;
        contains(otherInterval: GeInterval): boolean;
        contains(val: number): boolean;


        // Equality
        //
        isEqualAtUpper(otherInterval: GeInterval): boolean;
        isEqualAtUpper(value: number): boolean;
        isEqualAtLower(otherInterval: GeInterval): boolean;
        isEqualAtLower(value: number): boolean;
    }


    //GeMatrix2d
    class GeMatrix2d extends Em.EmObject{
        constructor();
        constructor(_mat: GeMatrix2d);
        static kIdentity: GeMatrix2d;
        set(_mat: GeMatrix2d): void;
        setToIdentity(): GeMatrix2d;
        preMultBy(leftSide: GeMatrix2d): GeMatrix2d;
        postMultBy(rightSide: GeMatrix2d): GeMatrix2d;
        setToProduct(mat1: GeMatrix2d, mat2: GeMatrix2d): GeMatrix2d;
        invert(): GeMatrix2d;
        inverse(): GeMatrix2d;
        isSingular(): boolean;
        isSingular(tol: GeTol): boolean;
        transposeIt(): GeMatrix2d;
        transpose(): GeMatrix2d;
        isEqualTo(mat: GeMatrix2d): boolean;
        isEqualTo(mat: GeMatrix2d, tol: GeTol): boolean;
        scale(): GeScale2d;
        det(): number;
        setTranslation(vec: GeVector2d): GeMatrix2d;
        translation(): GeVector2d;
        setCoordSystem(origin: GePoint2d, e0: GeVector2d, e1: GeVector2d): GeMatrix2d;
        getCoordSystem(origin: GePoint2d, e0: GeVector2d, e1: GeVector2d): void;
        setToTranslation(vec: GeVector2d): GeMatrix2d;
        setToRotation(angle: number): GeMatrix2d;
        setToRotation(angle: number, center: GePoint2d): GeMatrix2d;
        setToScaling(scaleAll: number): GeMatrix2d;
        setToScaling(scaleAll: number, center: GePoint2d): GeMatrix2d;
        setToScaling(scaleAll: GeScale2d): GeMatrix2d;
        setToScaling(scaleAll: GeScale2d, center: GePoint2d): GeMatrix2d;
        setToMirroring(pnt: GePoint2d): GeMatrix2d;
        setToMirroring(line: GeLine2d): GeMatrix2d;
        setToAlignCoordSys(fromOrigin: GePoint2d, fromE0: GeVector2d, fromE1: GeVector2d, toOrigin: GePoint2d, toE0: GeVector2d, toE1: GeVector2d): GeMatrix2d;
        static translation(vec: GeVector2d): GeMatrix2d;
        static rotation(angle: number): GeMatrix2d;
        static rotation(angle: number, center: GePoint2d): GeMatrix2d;
        static scaling(scaleAll: number): GeMatrix2d;
        static scaling(scaleAll: number, center: GePoint2d): GeMatrix2d;
        static scaling(scaleAll: GeScale2d): GeMatrix2d;
        static scaling(scaleAll: GeScale2d, center: GePoint2d): GeMatrix2d;
        static mirroring(pnt: GePoint2d): GeMatrix2d;
        static mirroring(line: GeLine2d): GeMatrix2d;
        static alignCoordSys(fromOrigin: GePoint2d, fromE0: GeVector2d, fromE1: GeVector2d, toOrigin: GePoint2d, toE0: GeVector2d, toE1: GeVector2d): GeMatrix2d;
    }

    //GeScale2d
    class GeScale2d extends Em.EmObject{
        constructor();
        constructor(_scale: GeScale2d);
        constructor(_sx: number, _sy: number);

        sx: number;
        sy: number;
        static kIdentity: GeScale2d;

        preMultBy(_scale: GeScale2d): GeScale2d;
        postMultBy(_scale: GeScale2d): GeScale2d;
        setToProduct(scaleVec: GeScale2d, factor: number): GeScale2d;
        setToProduct(scaleVec1: GeScale2d, scaleVec2: GeScale2d): GeScale2d;
        inverse(): GeScale2d;
        invert(): GeScale2d;
        isProportional(): boolean;
        isProportional(_tol: GeTol): boolean;
        isEqualTo(scaleVec: GeScale2d): boolean;
        isEqualTo(scaleVec: GeScale2d, _tol: GeTol): boolean;
        set(xFactor: number, yFactor: number): GeScale2d;
        getMatrix(xfm: GeMatrix2d): void;
        extractScale(xfm: GeMatrix2d): GeScale2d;
        removeScale(xfm: GeMatrix2d): GeScale2d;
    }


    //GeVector2d
    class GeVector2d extends Em.EmObject{
        constructor();
        constructor(_scale: GeScale2d);
        constructor(_sx: number, _sy: number);

        x: number;
        y: number;
        static kIdentity: GeVector2d;
        static kXAxis: GeVector2d;
        static kYAxis: GeVector2d;

        setToProduct(mat: GeMatrix2d, vec: GeVector2d): GeVector2d;
        setToProduct(mat: GeVector2d, scale: number): GeVector2d;
        transformBy(xfm: GeMatrix2d): GeVector2d;
        rotateBy(angle: number): GeVector2d;
        mirror(line: GeVector2d): GeVector2d;
        crossProduct(vect: GeVector2d): number;
        setToSum(vector1: GeVector2d, vector2: GeVector2d): GeVector2d;
        negate(): GeVector2d;
        perpVector(): GeVector2d;
        angle(): number;
        angleTo(vect: GeVector2d): number;
        angleToCCW(vect: GeVector2d): number;
        normal(): GeVector2d;
        normal(_tol: GeTol): GeVector2d;
        normalize(): GeVector2d;
        normalize(_tol: GeTol): GeVector2d;
        length(): number;
        lengthSqrd(): number;
        isUnitLength(): boolean;
        isUnitLength(_tol: GeTol): boolean;
        isZeroLength(): boolean;
        isZeroLength(_tol: GeTol): boolean;
        isParallelTo(vect: GeVector2d): boolean;
        isParallelTo(vect: GeVector2d, _tol: GeTol): boolean;
        isCodirectionalTo(vect: GeVector2d): boolean;
        isCodirectionalTo(vect: GeVector2d, _tol: GeTol): boolean;
        isPerpendicularTo(vect: GeVector2d): boolean;
        isPerpendicularTo(vect: GeVector2d, _tol: GeTol): boolean;
        dotProduct(vect: GeVector2d): number;
        isEqualTo(vect: GeVector2d): boolean;
        isEqualTo(vect: GeVector2d, _tol: GeTol): boolean;
        set(x: number, y: number): GeVector2d;
    }

    //GePoint2d
    class GePoint2d extends Em.EmObject {
        constructor();
        constructor(pnt: GePoint2d);
        constructor(_x: number, _y: number);

        x: number;
        y: number;
        static kOrigin: GePoint2d;

        setToProduct(matrix: GeMatrix2d, point: GePoint2d): GePoint2d;
        transformBy(xfm: GeMatrix2d): GePoint2d;
        translation(vec: GeVector2d): GePoint2d;
        rotateBy(angle: number): GePoint2d;
        rotateBy(angle: number, basePoint: GePoint2d): GePoint2d;
        scaleBy(scaleFactor: number): GePoint2d;
        scaleBy(scaleFactor: number, basePoint: GePoint2d): GePoint2d;
        scaleBy(scaleFactor: GeScale2d): GePoint2d;
        scaleBy(scaleFactor: GeScale2d, basePoint: GePoint2d): GePoint2d;
        mirror(line: GeLine2d): GePoint2d;
        setToSum(point: GePoint2d, vect: GeVector2d): GePoint2d;
        asVector(): GeVector2d;
        distanceTo(point: GePoint2d): number;
        isEqualTo(point: GePoint2d): boolean;
        isEqualTo(point: GePoint2d, tol: GeTol): boolean;
        set(x: number, y: number): GePoint2d;
    }


    //GeEntity2d
    abstract class GeEntity2d extends Em.EmObject{
        isKindOf(entType: EntityId): boolean;
        type(): EntityId;
        copy(): GeEntity2d;
        isEqualTo(entity: GeEntity2d): boolean
        isEqualTo(entity: GeEntity2d, tol: GeTol): boolean
        transformBy(xfm: GeMatrix2d): GeEntity2d;
        translateBy(translateVec: GeVector2d): GeEntity2d;
        rotateBy(angle: number): GeEntity2d;
        rotateBy(angle: number, wrtPoint: GePoint2d): GeEntity2d;
        mirror(line: GeLine2d): GeEntity2d;
        scaleBy(scaleFactor: number): GeEntity2d;
        scaleBy(scaleFactor: number, wrtPoint: GePoint2d): GeEntity2d;
        isOn(pnt: GePoint2d): boolean
        isOn(pnt: GePoint2d, tol: GeTol): boolean;
    }

    //GeBoundBlock2d
    class GeBoundBlock2d extends GeEntity2d {
        constructor();
        constructor(point1: GePoint2d, point2: GePoint2d);
        constructor(base: GePoint2d, dir1: GeVector2d, dir2: GeVector2d);
        getMinMaxPoints(p1: GePoint2d, p2: GePoint2d): void;
        get(base: GePoint2d, side1: GeVector2d, side2: GeVector2d): void;
        set(p1: GePoint2d, p2: GePoint2d): GeBoundBlock2d;
        set(base: GePoint2d, side1: GeVector2d, side2: GeVector2d): GeBoundBlock2d;
        extend(point: GePoint2d): GeBoundBlock2d;
        swell(distance: number): GeBoundBlock2d;
        contains(point: GePoint2d): boolean;
        isDisjoint(block: GeBoundBlock2d): boolean;
        isBox(): boolean;
        setToBox(toBox: boolean): GeBoundBlock2d;
    }

    //GePointEnt2d
    abstract class GePointEnt2d extends GeEntity2d {
        point2d(): GePoint2d;
    }

    //GePointOnCurve2d
    class GePointOnCurve2d extends GePointEnt2d {
        constructor();
        constructor(crv: GeCurve2d);
        constructor(crv: GeCurve2d, param: number);
        curve(): GeCurve2d;
        parameter(): number;
        point(): GePoint2d;
        point(param: number): GePoint2d;
        point(crv: GeCurve2d, param: number): GePoint2d;
        setCurve(crv: GeCurve2d): GePointOnCurve2d;
        setParameter(param: number): GePointOnCurve2d;

        isEqualTo(entity: GePointOnCurve2d): boolean
        isEqualTo(entity: GePointOnCurve2d, tol: GeTol): boolean
    }

    //GeCurve
    abstract class GeCurve2d extends GeEntity2d {
        distanceTo(pnt: GePoint2d): number;
        distanceTo(pnt: GePoint2d, tol: GeTol): number;
        distanceTo(entity: GeCurve2d): number;
        distanceTo(entity: GeCurve2d, tol: GeTol): number;

        // Return the point on this object that is closest to the other object.
        // These methods return point on this curve as a simple 2d point.
        //
        closestPointTo(pnt: GePoint2d): GePoint2d;
        closestPointTo(pnt: GePoint2d, tol: GeTol): GePoint2d;
        closestPointTo(curve2d: GeCurve2d, pntOnOtherCrv: GePoint2d): GePoint2d;
        closestPointTo(curve2d: GeCurve2d, pntOnOtherCrv: GePoint2d, tol: GeTol): GePoint2d;

        // Alternate signatures for above functions.  These methods return point
        // on this curve as an GePointOnCurve2d.
        //
        getClosestPointTo(pnt: GePoint2d, pntOnCrv: GePointOnCurve2d): void;
        getClosestPointTo(pnt: GePoint2d, pntOnCrv: GePointOnCurve2d, tol: GeTol): void;
        getClosestPointTo(curve2d: GeCurve2d, pntOnThisCrv: GePointOnCurve2d, pntOnOtherCrv: GePointOnCurve2d): void;
        getClosestPointTo(curve2d: GeCurve2d, pntOnThisCrv: GePointOnCurve2d, pntOnOtherCrv: GePointOnCurve2d, tol: GeTol): void;


        // Tests if point is on curve.
        //
        isOn(pnt: GePoint2d): boolean;
        isOn(pnt: GePoint2d, tol: GeTol): boolean;

        // Parameter of the point on curve.  Contract: point IS on curve
        //
        paramOf(pnt: GePoint2d): number;
        paramOf(pnt: GePoint2d, tol: GeTol): number;


        // Geometric inquiry methods.
        //
        isClosed(): boolean;
        isClosed(tol: GeTol): boolean;

        // Length based methods.
        //
        length(): number;
        length(fromParam: number, toParam: number): number;
        length(fromParam: number, toParam: number, tol: number): number;
        paramAtLength(datumParam: number, length: number): number;
        paramAtLength(datumParam: number, length: number, tol: number): number;
        area(): number;
        area(tol: GeTol): number;


        // Modify methods.
        //
        getSplitCurves(param: number, piece1: GeCurve2d, piece2: GeCurve2d): void;


        // Return oriented bounding box of curve.
        //
        boundBlock(): GeBoundBlock2d;
        boundBlock(range: GeInterval): GeBoundBlock2d;

        // Return bounding box whose sides are parallel to coordinate axes.
        //
        orthoBoundBlock(): GeBoundBlock2d;
        orthoBoundBlock(range: GeInterval): GeBoundBlock2d;

        // Return start and end points.
        //
        hasStartPoint(startPoint: GePoint2d): boolean;
        hasEndPoint(endPoint: GePoint2d): boolean;

        // Evaluate methods.
        //
        evalPoint(param: number): GePoint2d;
    }

    //GeLinearEnt2d
    abstract class GeLinearEnt2d extends GeCurve2d {
        static getVertical(pt: GePoint2d, line: GeLinearEnt2d): GePoint2d;
        static getVertical(pt: GePoint2d, line: GeLinearEnt2d, tol: GeTol): GePoint2d;
        intersectWith(line: GeLinearEnt2d, intPnt: GePoint2d): boolean;
        intersectWith(line: GeLinearEnt2d, intPnt: GePoint2d, tol: GeTol): boolean;
        isParallelTo(line: GeLinearEnt2d): boolean;
        isParallelTo(line: GeLinearEnt2d, tol: GeTol): boolean;
        isPerpendicularTo(cline: GeLinearEnt2d): boolean;
        isPerpendicularTo(line: GeLinearEnt2d, tol: GeTol): boolean;
        isColinearTo(line: GeLinearEnt2d): boolean;
        isColinearTo(line: GeLinearEnt2d, tol: GeTol): boolean;
        getPerpLine(pnt: GePoint2d, perpLine: GeLine2d): void;
        pointOnLine(): GePoint2d;
        direction(): GeVector2d;
        getLine(line: GeLine2d): void;
    }

    //GeRay2d
    class GeRay2d extends GeLinearEnt2d {
        constructor();
        constructor(line: GeRay2d);
        constructor(pnt1: GePoint2d, pnt2: GePoint2d);

        set(pnt: GePoint2d, vec: GeVector2d): GeRay2d;
        set(pnt1: GePoint2d, pnt2: GePoint2d): GeRay2d;
    }

    //GeLine2d
    class GeLine2d extends GeLinearEnt2d {
        constructor();
        constructor(line: GeLine2d);
        constructor(pnt1: GePoint2d, pnt2: GePoint2d);

        static kXAxis: GeLine2d;
        static kYAxis: GeLine2d;

        isKindOf(entType: EntityId): boolean;
        type(): EntityId;
        copy(): GeEntity2d;
        isEqualTo(entity: GeLine2d): boolean
        isEqualTo(entity: GeLine2d, tol: GeTol): boolean
        transformBy(xfm: GeMatrix2d): GeLine2d;
        translateBy(translateVec: GeVector2d): GeLine2d;
        rotateBy(angle: number): GeLine2d;
        rotateBy(angle: number, wrtPoint: GePoint2d): GeLine2d;
        mirror(line: GeLine2d): GeLine2d;
        scaleBy(scaleFactor: number): GeLine2d;
        scaleBy(scaleFactor: number, wrtPoint: GePoint2d): GeLine2d;
        isOn(pnt: GePoint2d): boolean
        isOn(pnt: GePoint2d, tol: GeTol): boolean;

        set(pnt: GePoint2d, vec: GeVector2d): GeLine2d;
        set(pnt1: GePoint2d, pnt2: GePoint2d): GeLine2d;
    }

    //GeLine2d
    class GeLineSeg2d extends GeLinearEnt2d {
        constructor();
        constructor(line: GeLine2d);
        constructor(pnt1: GePoint2d, pnt2: GePoint2d);

        static kXAxis: GeLine2d;
        static kYAxis: GeLine2d;

        //GeEntity2d
        isKindOf(entType: EntityId): boolean;
        type(): EntityId;
        copy(): GeLineSeg2d;
        isEqualTo(entity: GeLineSeg2d): boolean
        isEqualTo(entity: GeLineSeg2d, tol: GeTol): boolean
        transformBy(xfm: GeMatrix2d): GeLineSeg2d;
        translateBy(translateVec: GeVector2d): GeLineSeg2d;
        rotateBy(angle: number): GeLineSeg2d;
        rotateBy(angle: number, wrtPoint: GePoint2d): GeLineSeg2d;
        mirror(line: GeLine2d): GeLineSeg2d;
        scaleBy(scaleFactor: number): GeLineSeg2d;
        scaleBy(scaleFactor: number, wrtPoint: GePoint2d): GeLineSeg2d;
        isOn(pnt: GePoint2d): boolean
        isOn(pnt: GePoint2d, tol: GeTol): boolean;

        // Set methods.
        //
        set(pnt: GePoint2d, vec: GeVector2d): GeLineSeg2d;
        set(pnt1: GePoint2d, pnt2: GePoint2d): GeLineSeg2d;


        // Bisector.
        //
        getBisector(line: GeLine2d): void;

        // Barycentric combination of end points.
        //
        baryComb(blendCoeff: number): GePoint2d;

        // Definition of linear segment
        //
        startPoint(): GePoint2d;
        midPoint(): GePoint2d;
        endPoint(): GePoint2d;
    }

    //GeCircArc2d
    class GeCircArc2d extends GeCurve2d {
        constructor();
        constructor(arc: GeCircArc2d);
        constructor(cent: GePoint2d, radius: number);
        constructor(cent: GePoint2d, radius: number, startAngle: number, endAngle: number);
        constructor(cent: GePoint2d, radius: number, startAngle: number, endAngle: number, refVec: GeVector2d);
        constructor(cent: GePoint2d, radius: number, startAngle: number, endAngle: number, refVec: GeVector2d, isClockWise: boolean);
        constructor(startPoint: GePoint2d, point: GePoint2d, endPoint: GePoint2d);

        // Intersection with other geometric objects.
        //
        intersectWith(line: GeLinearEnt2d): GePoint2dArray;
        intersectWith(line: GeLinearEnt2d, tol: GeTol): GePoint2dArray;
        intersectWith(arc: GeCircArc2d): GePoint2dArray;
        intersectWith(arc: GeCircArc2d, tol: GeTol): GePoint2dArray;

        // Tangent line to the circular arc at given point.
        //
        tangent(pnt: GePoint2d, line: GeLine2d): boolean;
        tangent(pnt: GePoint2d, line: GeLine2d, tol: GeTol): boolean;


        // Test if point is inside circle.
        //
        isInside(pnt: GePoint2d): boolean;
        isInside(pnt: GePoint2d, tol: GeTol): boolean;

        // Definition of circular arc
        //
        center(): GePoint2d;
        radius(): number;
        startAng(): number;
        endAng(): number;
        refVec(): GeVector2d;
        startPoint(): GePoint2d;
        endPoint(): GePoint2d;
        isClockWise(): boolean;

        setCenter(cent: GePoint2d): GeCircArc2d;
        setRadius(radius: number): GeCircArc2d;
        setAngles(startAng: number, endAng: number): GeCircArc2d;

        // Reverses the direction of the arc while keeping the same startand endpoints.For bounded arcs, 
        // this function returns the complement of the original arc.
        //GeCircArc2d& setToComplement();
        set(cent: GePoint2d, radius: number): GeCircArc2d;
        set(cent: GePoint2d, radius: number, ang1: number, ang2: number): GeCircArc2d;
        set(cent: GePoint2d, radius: number, startAngle: number, endAngle: number, refVec: GeVector2d): GeCircArc2d;
        set(cent: GePoint2d, radius: number, startAngle: number, endAngle: number, refVec: GeVector2d, isClockWise: boolean): GeCircArc2d;
        set(startPoint: GePoint2d, pnt: GePoint2d, endPoint: GePoint2d): GeCircArc2d;

        // Possible errors:  kEqualArg1Arg2, kEqualArg1Arg3, kEqualArg2Arg3, 
        // kLinearlyDependentArg1Arg2Arg3.
        // Degenerate results: none.
        // On error, the object is unchanged.

        // If bulgeFlag is kTrue, then bulge is interpreted to be the maximum
        // distance between the arc and the chord between the two input points.
        // If bulgeFlag is kFalse, then bulge is interpreted to be tan(ang/4),
        // where ang is the angle of the arc segment between the two input points.
        set(startPoint: GePoint2d, endPoint: GePoint2d, bulge: number): GeCircArc2d;
        set(startPoint: GePoint2d, endPoint: GePoint2d, bulge: number, bulgeFlag: boolean): GeCircArc2d;
    }


    class GeQuaternion extends Em.EmObject{
        constructor();
        constructor(theMat: GeMatrix3d);
        constructor(theAxis: GeVector3d, theAngle: number);
        constructor(theX: number, theY: number, theZ: number, theW: number);

        x: number;
        y: number;
        z: number;
        w: number;

        isEqual(theOther: GeQuaternion): boolean;
        setRotation(theVecFrom: GeVector3d, theVecTo: GeVector3d): void;
        setRotation(theVecFrom: GeVector3d, theVecTo: GeVector3d, theHelpCrossVec: GeVector3d): void;
        setVectorAndAngle(theAxis: GeVector3d, theAngle: number): void;
        getVectorAndAngle(): GeVector3d;
        setMatrix(_mat: GeMatrix3d): void;
        getMatrix(): GeMatrix3d;
        set(theX: number, theY: number, theZ: number, theW: number): void;
        set(theQuaternion: GeQuaternion): void;
        setIdent(): void;
        reverse(): void;
        reversed(): GeQuaternion;
        invert(): void;
        inverted(): GeQuaternion;
        squareNorm(): number;
        norm(): number;
        scale(): number;
        scaled(theScale: number): GeQuaternion;
        stabilizeLength(): void;
        normalize(): void;
        normalized(): GeQuaternion;
        negated(): GeQuaternion;
        added(theOther: GeQuaternion): GeQuaternion;
        subtracted(theOther: GeQuaternion): GeQuaternion;
        multiplied(theOther: GeQuaternion): GeQuaternion;
        add(theOther: GeQuaternion): void;
        subtract(theOther: GeQuaternion): void;
        multiply(theOther: GeQuaternion): void;
        dot(theOther: GeQuaternion): number;
        getRotationAngle(): number;
        multiply(theVec: GeVector3d): GeVector3d;
    }


    //GeMatrix3d
    class GeMatrix3d extends Em.EmObject{
        constructor();
        constructor(_mat: GeMatrix3d);
        static kIdentity: GeMatrix3d;
        set(_mat: GeMatrix3d): void;
        get(_row: number, _col: number): number;
        setToIdentity(): GeMatrix3d;
        preMultBy(leftSide: GeMatrix3d): GeMatrix3d;
        postMultBy(rightSide: GeMatrix3d): GeMatrix3d;
        setToProduct(mat1: GeMatrix3d, mat2: GeMatrix3d): GeMatrix3d;
        invert(): GeMatrix3d;
        inverse(): GeMatrix3d;
        inverse(tol: GeTol): GeMatrix3d;
        inverse(inverseMatrix: GeMatrix3d, tol: number): boolean;
        isSingular(): boolean;
        isSingular(tol: GeTol): boolean;
        isEqualTo(mat: GeMatrix3d): boolean;
        isEqualTo(mat: GeMatrix3d, tol: GeTol): boolean;
        det(): number;
        setTranslation(vec: GeVector3d): GeMatrix3d;
        setCoordSystem(origin: GePoint3d, e0: GeVector3d, e1: GeVector3d): GeMatrix3d;
        getCoordSystem(origin: GePoint3d, e0: GeVector3d, e1: GeVector3d): void;
        getCsOrigin(): GePoint3d;
        getCsXAxis(): GeVector3d;
        getCsYAxis(): GeVector3d;
        getCsZAxis(): GeVector3d;
        setToTranslation(vec: GeVector3d): GeMatrix3d;
        setToRotation(angle: number, axis: GeVector3d): GeMatrix3d;
        setToRotation(angle: number, axis: GeVector3d, center: GePoint3d): GeMatrix3d;
        setToScaling(scaleAll: number): GeMatrix3d;
        setToScaling(scaleAll: number, center: GePoint3d): GeMatrix3d;
        setToScaling(scaleAll: GeScale3d): GeMatrix3d;
        setToScaling(scaleAll: GeScale3d, center: GePoint3d): GeMatrix3d;
        transposeIt(): GeMatrix3d;
        transpose(): GeMatrix3d;
        scale(): GeScale3d;
        translation(): GeVector3d;
        setToMirroring(mirrorPlane: GePlane): GeMatrix3d;
        setToMirroring(pnt: GePoint3d): GeMatrix3d;
        setToMirroring(line: GeLine3d): GeMatrix3d;
        setToAlignCoordSys(fromOrigin: GePoint3d, fromXAxis: GeVector3d, fromYAxis: GeVector3d, fromZAxis: GeVector3d, toOrigin: GePoint3d, toXAxis: GeVector3d, toYAxis: GeVector3d, toZAxis: GeVector3d): GeMatrix3d;
        setToWorldToPlane(normal: GeVector3d): GeMatrix3d;
        setToWorldToPlane(plane: GePlane): GeMatrix3d;
        setToPlaneToWorld(normal: GeVector3d): GeMatrix3d;
        setToPlaneToWorld(plane: GePlane): GeMatrix3d;
        static translation(vec: GeVector3d): GeMatrix3d;
        static rotation(angle: number, axis: GeVector3d): GeMatrix3d;
        static rotation(angle: number, axis: GeVector3d, center: GePoint3d): GeMatrix3d;
        static scaling(scaleAll: number): GeMatrix3d;
        static scaling(scaleAll: number, center: GePoint3d): GeMatrix3d;
        static scaling(scaleAll: GeScale3d): GeMatrix3d;
        static scaling(scaleAll: GeScale3d, center: GePoint3d): GeMatrix3d;
        static mirroring(mirrorPlane: GePlane): GeMatrix3d;
        static mirroring(pnt: GePoint3d): GeMatrix3d;
        static mirroring(line: GeLine3d): GeMatrix3d;
        static alignCoordSys(fromOrigin: GePoint3d, fromXAxis: GeVector3d, fromYAxis: GeVector3d, fromZAxis: GeVector3d, toOrigin: GePoint3d, toXAxis: GeVector3d, toYAxis: GeVector3d, toZAxis: GeVector3d): GeMatrix3d;
        static worldToPlane(normal: GeVector3d): GeMatrix3d;
        static worldToPlane(plane: GePlane): GeMatrix3d;
        static planeToWorld(normal: GeVector3d): GeMatrix3d;
        static worldToPlane(plane: GePlane): GeMatrix3d;
    }

    //GeScale3d
    class GeScale3d extends Em.EmObject{
        constructor();
        constructor(_scale: GeScale3d);
        constructor(_sx: number, _sy: number, _sz: number);

        sx: number;
        sy: number;
        sz: number;
        static kIdentity: GeScale3d;

        preMultBy(_scale: GeScale3d): GeScale3d;
        postMultBy(_scale: GeScale3d): GeScale3d;
        setToProduct(scaleVec: GeScale3d, factor: number): GeScale3d;
        setToProduct(scaleVec1: GeScale3d, scaleVec2: GeScale3d): GeScale3d;
        inverse(): GeScale2d;
        invert(): GeScale2d;
        isEqualTo(scaleVec: GeScale3d): boolean;
        isEqualTo(scaleVec: GeScale3d, _tol: GeTol): boolean;
        set(xFactor: number, yFactor: number, zFactor: number): GeScale3d;
        getMatrix(xfm: GeMatrix3d): void;
        extractScale(xfm: GeMatrix3d): GeScale3d;
        removeScale(xfm: GeMatrix3d): GeScale3d;
    }


    //GeVector3d
    class GeVector3d extends Em.EmObject{
        constructor();
        constructor(_scale: GeScale3d);
        constructor(plane: GePlanarEnt, vector: GeVector2d);
        constructor(_sx: number, _sy: number, _sz: number);

        x: number;
        y: number;
        z: number;
        static kIdentity: GeVector3d;
        static kXAxis: GeVector3d;
        static kYAxis: GeVector3d;
        static kZAxis: GeVector3d;

        setToProduct(mat: GeMatrix3d, vec: GeVector3d): GeVector3d;
        setToProduct(mat: GeVector3d, scale: number): GeVector3d;
        transformBy(xfm: GeMatrix3d): GeVector3d;
        rotateBy(angle: number, axis: GeVector3d): GeVector3d;
        mirror(normalToPlane: GeVector3d): GeVector3d;
        convert2d(plane: GePlanarEnt): GeVector2d;
        convert2d(): GeVector2d;
        asPoint(): GePoint3d;
        setToSum(vector1: GeVector3d, vector2: GeVector3d): GeVector3d;
        negate(): GeVector3d;
        perpVector(): GeVector3d;
        angle(): number;
        angleTo(vect: GeVector3d): number;
        angleToCCW(vect: GeVector3d): number;
        angleOnPlane(plane: GePlanarEnt): number;
        normal(): GeVector3d;
        normal(_tol: GeTol): GeVector3d;
        normalize(): GeVector3d;
        normalize(_tol: GeTol): GeVector3d;
        length(): number;
        setLength(length: number): void;
        lengthSqrd(): number;
        isUnitLength(): boolean;
        isUnitLength(_tol: GeTol): boolean;
        isZeroLength(): boolean;
        isZeroLength(_tol: GeTol): boolean;
        isParallelTo(vect: GeVector3d): boolean;
        isParallelTo(vect: GeVector3d, _tol: GeTol): boolean;
        isCodirectionalTo(vect: GeVector3d): boolean;
        isCodirectionalTo(vect: GeVector3d, _tol: GeTol): boolean;
        isPerpendicularTo(vect: GeVector3d): boolean;
        isPerpendicularTo(vect: GeVector3d, _tol: GeTol): boolean;
        dotProduct(vect: GeVector3d): number;
        crossProduct(vect: GeVector3d): number;
        orthoProject(planeNormal: GeVector3d): GeVector3d;
        isEqualTo(vect: GeVector3d): boolean;
        isEqualTo(vect: GeVector3d, _tol: GeTol): boolean;
        set(x: number, y: number, z: number): GeVector3d;
        set(plane: GePlanarEnt, vect: GeVector2d): GeVector3d;
    }

    //GePoint3d
    class GePoint3d extends Em.EmObject{
        constructor();
        constructor(pnt: GePoint3d);
        constructor(_x: number, _y: number, _z: number);

        x: number;
        y: number;
        z: number;
        static kOrigin: GePoint3d;

        setToProduct(matrix: GeMatrix3d, point: GePoint3d): GePoint3d;
        transformBy(xfm: GeMatrix3d): GePoint3d;
        translation(vec: GeVector3d): GePoint3d;
        rotateBy(angle: number, _axis: GeVector3d): GePoint3d;
        rotateBy(angle: number, _axis: GeVector3d, basePoint: GePoint3d): GePoint3d;
        scaleBy(scaleFactor: number): GePoint3d;
        scaleBy(scaleFactor: number, basePoint: GePoint3d): GePoint3d;
        scaleBy(scaleFactor: GeScale3d): GePoint3d;
        scaleBy(scaleFactor: GeScale3d, basePoint: GePoint3d): GePoint3d;
        mirror(line: GeLine3d): GePoint3d;
        setToSum(point: GePoint3d, vect: GeVector3d): GePoint3d;
        asVector(): GeVector3d;
        distanceTo(point: GePoint3d): number;
        isEqualTo(point: GePoint3d): boolean;
        isEqualTo(point: GePoint3d, tol: GeTol): boolean;
        set(x: number, y: number, z: number): GePoint3d;
    }


    //GeEntity3d
    abstract class GeEntity3d extends Em.EmObject{
        isKindOf(entType: EntityId): boolean;
        type(): EntityId;
        copy(): GeEntity3d;
        isEqualTo(entity: GeEntity3d): boolean
        isEqualTo(entity: GeEntity3d, tol: GeTol): boolean
        transformBy(xfm: GeMatrix3d): GeEntity3d;
        translateBy(translateVec: GeVector3d): GeEntity3d;
        rotateBy(angle: number, vec: GeVector3d): GeEntity3d;
        rotateBy(angle: number, vec: GeVector3d, wrtPoint: GePoint3d): GeEntity3d;
        mirror(plane: GePlane): GeEntity3d;
        scaleBy(scaleFactor: number): GeEntity3d;
        scaleBy(scaleFactor: number, wrtPoint: GePoint3d): GeEntity3d;
        isOn(pnt: GePoint3d): boolean
        isOn(pnt: GePoint3d, tol: GeTol): boolean;
    }


    //GeBoundBlock3d
    class GeBoundBlock3d extends GeEntity3d {
        constructor();
        constructor(source: GeBoundBlock3d);
        constructor(point1: GePoint3d, point2: GePoint3d);
        constructor(base: GePoint3d, dir1: GeVector3d, dir2: GeVector3d, dir3: GeVector3d);
        getMinMaxPoints(p1: GePoint3d, p2: GePoint3d): void;
        get(base: GePoint3d, side1: GeVector3d, side2: GeVector3d, side3: GeVector3d): void;
        set(p1: GePoint3d, p2: GePoint3d, p3: GePoint3d): GeBoundBlock3d;
        set(base: GePoint3d, side1: GeVector3d, side2: GeVector3d, side3: GeVector3d): GeBoundBlock3d;
        extend(point: GePoint3d): GeBoundBlock3d;
        swell(distance: number): GeBoundBlock3d;
        contains(point: GePoint3d): boolean;
        isDisjoint(block: GeBoundBlock3d): boolean;
        isBox(): boolean;
        setToBox(toBox: boolean): GeBoundBlock3d;
    }


    //GePointEnt3d
    abstract class GePointEnt3d extends GeEntity3d {
        point3d(): GePoint3d;
    }

    //GePointOnCurve3d
    class GePointOnCurve3d extends GePointEnt3d {
        constructor();
        constructor(crv: GeCurve3d);
        constructor(crv: GeCurve3d, param: number);
        curve(): GeCurve3d;
        parameter(): number;
        point(): GePoint3d;
        point(param: number): GePoint3d;
        point(crv: GeCurve3d, param: number): GePoint3d;
        setCurve(crv: GeCurve3d): GePointOnCurve3d;
        setParameter(param: number): GePointOnCurve3d;

        isEqualTo(entity: GePointOnCurve3d): boolean
        isEqualTo(entity: GePointOnCurve3d, tol: GeTol): boolean
    }

    //GePointOnSurface
    class GePointOnSurface extends GePointEnt3d {
        constructor();
        constructor(surf: GeSurface);
        constructor(surf: GeSurface, param: GePoint2d);

        // Query functions.
        //
        surface(): GeSurface;
        parameter(): GePoint2d;

        // Functions to evaluate a point.
        //
        point(): GePoint3d;
        point(param: GePoint2d): GePoint3d;
        point(surf: GeSurface, param: GePoint2d): GePoint3d;

        // Set functions.
        //
        setSurface(surf: GeSurface): GePointOnSurface;
        setParameter(param: GePoint2d): GePointOnSurface;
    }


    //GeSurface
    abstract class GeSurface extends GeEntity3d {
        // Parameter related.
        //
        paramOf(pnt: GePoint3d): GePoint2d;
        paramOf(pnt: GePoint3d, tol: GeTol): GePoint2d;

        // Point containment
        //
        isOn(pnt: GePoint3d): boolean;
        isOn(pnt: GePoint3d, tol: GeTol): boolean;

        // Operations.
        //
        closestPointTo(pnt: GePoint3d): GePoint3d;
        closestPointTo(pnt: GePoint3d, tol: GeTol): GePoint3d;

        getClosestPointTo(pnt: GePoint3d, result: GePointOnSurface): void;
        getClosestPointTo(pnt: GePoint3d, result: GePointOnSurface, tol: GeTol): void;

        distanceTo(pnt: GePoint3d): number;
        distanceTo(pnt: GePoint3d, tol: GeTol): number;

        isNormalReversed(): boolean;
        reverseNormal(): GeSurface;

        // Evaluators.
        // Derivative arrays are indexed partialU, partialV followed by
        // the mixed partial.
        //
        evalPoint(param: GePoint2d): GePoint3d;
    }

    //GePlanarEnt
    abstract class GePlanarEnt extends GeSurface {

        // Intersection
        //
        intersectWith(linEnt: GeLinearEnt3d, pnt: GePoint3d): boolean;
        intersectWith(linEnt: GeLinearEnt3d, pnt: GePoint3d, tol: GeTol): boolean;
        // Closest point
        //
        closestPointToLinearEnt(line: GeLinearEnt3d, pointOnLine: GePoint3d): GePoint3d;
        closestPointToLinearEnt(line: GeLinearEnt3d, pointOnLine: GePoint3d, tol: GeTol): GePoint3d;
        closestPointToPlanarEnt(otherPln: GePlanarEnt, pointOnOtherPln: GePoint3d): GePoint3d;
        closestPointToPlanarEnt(otherPln: GePlanarEnt, pointOnOtherPln: GePoint3d, tol: GeTol): GePoint3d;
        // Direction tests.
        //
        isParallelTo(linEnt: GeLinearEnt3d): boolean;
        isParallelTo(linEnt: GeLinearEnt3d, tol: GeTol): boolean;
        isParallelTo(otherPlnEnt: GePlanarEnt): boolean;
        isParallelTo(otherPlnEnt: GePlanarEnt, tol: GeTol): boolean;
        isPerpendicularTo(linEnt: GeLinearEnt3d): boolean;
        isPerpendicularTo(linEnt: GeLinearEnt3d, tol: GeTol): boolean;
        isPerpendicularTo(linEnt: GePlanarEnt): boolean;
        isPerpendicularTo(linEnt: GePlanarEnt, tol: GeTol): boolean;

        // Point set equality.
        //
        isCoplanarTo(otherPlnEnt: GePlanarEnt): boolean;
        isCoplanarTo(otherPlnEnt: GePlanarEnt, tol: GeTol): boolean;

        // Get methods.
        //
        get(origin: GePoint3d, uVec: GeVector3d, vVec: GeVector3d): void;

        // Geometric properties.
        //
        pointOnPlane(): GePoint3d;
        normal(): GeVector3d;
        getCoordSystem(origin: GePoint3d, axis1: GeVector3d, axis2: GeVector3d): void;
    }

    //GePlane
    class GePlane extends GePlanarEnt {
        static kXYPlane: GePlane;
        static kYZPlane: GePlane;
        static kZXPlane: GePlane;

        constructor();
        constructor(src: GePlane);
        constructor(origin: GePoint3d, normal: GeVector3d);
        constructor(p1: GePoint3d, origin: GePoint3d, p2: GePoint3d);
        constructor(a: number, b: number, c: number, d: number);

        // Signed distance from a point to a plane.
        //
        signedDistanceTo(pnt: GePoint3d): number;

        // Intersection
        //
        intersectWith(linEnt: GeLinearEnt3d, resultPnt: GePoint3d): boolean;
        intersectWith(linEnt: GeLinearEnt3d, resultPnt: GePoint3d, tol: GeTol): boolean;
        intersectWith(otherPln: GePlane, resultLine: GeLine3d): boolean;
        intersectWith(otherPln: GePlane, resultLine: GeLine3d, tol: GeTol): boolean;
        intersectWith(bndPln: GeBoundedPlane, resultLineSeg: GeLineSeg3d): boolean;
        intersectWith(bndPln: GeBoundedPlane, resultLineSeg: GeLineSeg3d, tol: GeTol): boolean;

        // Geometry redefinition.
        //
        set(pnt: GePoint3d, normal: GeVector3d): GePlane;
        set(pntU: GePoint3d, org: GePoint3d, pntV: GePoint3d): GePlane;
        set(a: number, b: number, c: number, d: number): GePlane;
        set(org: GePoint3d, uAxis: GeVector3d, vAxis: GeVector3d): GePlane;
    }

    //GeBoundedPlane
    class GeBoundedPlane extends GePlanarEnt {
        constructor();
        constructor(plane: GeBoundedPlane);
        constructor(p1: GePoint3d, origin: GePoint3d, p2: GePoint3d);

        // Intersection.
        //
        intersectWith(linEnt: GeLinearEnt3d, point: GePoint3d): boolean;
        intersectWith(linEnt: GeLinearEnt3d, point: GePoint3d, tol: GeTol): boolean;
        intersectWith(plane: GePlane, results: GeLineSeg3d): boolean;
        intersectWith(plane: GePlane, results: GeLineSeg3d, tol: GeTol): boolean;
        intersectWith(plane: GeBoundedPlane, result: GeLineSeg3d): boolean;
        intersectWith(plane: GeBoundedPlane, result: GeLineSeg3d, tol: GeTol): boolean;

        // Set methods.
        //
        set(origin: GePoint3d, uVec: GeVector3d, vVec: GeVector3d): GeBoundedPlane;
        set(p1: GePoint3d, origin: GePoint3d, p2: GePoint3d): GeBoundedPlane;
    }



    //GeCurve3d
    abstract class GeCurve3d extends GeEntity3d {
        distanceTo(pnt: GePoint3d): number;
        distanceTo(pnt: GePoint3d, tol: GeTol): number;
        distanceTo(entity: GeCurve3d): number;
        distanceTo(entity: GeCurve3d, tol: GeTol): number;

        // Return the point on this object that is closest to the other object.
        // These methods return point on this curve as a simple 3d point.
        //
        closestPointTo(pnt: GePoint3d): GePoint3d;
        closestPointTo(pnt: GePoint3d, tol: GeTol): GePoint3d;
        closestPointTo(curve3d: GeCurve3d, pntOnOtherCrv: GePoint3d): GePoint3d;
        closestPointTo(curve3d: GeCurve3d, pntOnOtherCrv: GePoint3d, tol: GeTol): GePoint3d;

        // Alternate signatures for above functions.  These methods return point
        // on this curve as an GePointOnCurve3d.
        //
        getClosestPointTo(pnt: GePoint3d, pntOnCrv: GePointOnCurve3d): void;
        getClosestPointTo(pnt: GePoint3d, pntOnCrv: GePointOnCurve3d, tol: GeTol): void;
        getClosestPointTo(curve3d: GeCurve3d, pntOnThisCrv: GePointOnCurve3d, pntOnOtherCrv: GePointOnCurve3d): void;
        getClosestPointTo(curve3d: GeCurve3d, pntOnThisCrv: GePointOnCurve3d, pntOnOtherCrv: GePointOnCurve3d, tol: GeTol): void;

        // Return closest points when projected in a given direction.
        // These methods return point on this curve as a simple 3d point.
        //
        projClosestPointTo(pnt: GePoint3d, projectDirection: GeVector3d): GePoint3d;
        projClosestPointTo(pnt: GePoint3d, projectDirection: GeVector3d, tol: GeTol): GePoint3d;
        projClosestPointTo(curve3d: GeCurve3d, projectDirection: GeVector3d, pntOnOtherCrv: GePoint3d): GePoint3d;
        projClosestPointTo(curve3d: GeCurve3d, projectDirection: GeVector3d, pntOnOtherCrv: GePoint3d, tol: GeTol): GePoint3d;

        // Alternate signatures for above functions.  These methods return point
        // on this curve as an GePointOnCurve3d.
        //
        getProjClosestPointTo(pnt: GePoint3d, projectDirection: GeVector3d, pntOnCrv: GePointOnCurve3d): void;
        getProjClosestPointTo(pnt: GePoint3d, projectDirection: GeVector3d, pntOnCrv: GePointOnCurve3d, tol: GeTol): void;
        getProjClosestPointTo(curve3d: GeCurve3d, projectDirection: GeVector3d, pntOnThisCrv: GePoint3d, pntOnOtherCrv: GePointOnCurve3d): void;
        getProjClosestPointTo(curve3d: GeCurve3d, projectDirection: GeVector3d, pntOnThisCrv: GePoint3d, pntOnOtherCrv: GePointOnCurve3d, tol: GeTol): void;

        // Return point on curve whose normal vector passes thru input point.
        // Second parameter contains initial guess value and also 
        // contains output point.

        // Returns true or false depending on whether a normal point was found.
        //
        getNormalPoint(pnt: GePoint3d, pntOnCrv: GePointOnCurve3d): boolean;
        getNormalPoint(pnt: GePoint3d, pntOnCrv: GePointOnCurve3d, tol: GeTol): boolean;

        // Return oriented bounding box of curve.
        //
        boundBlock(): GeBoundBlock3d;
        boundBlock(range: GeInterval): GeBoundBlock3d;

        // Return bounding box whose sides are parallel to coordinate axes.
        //
        orthoBoundBlock(): GeBoundBlock3d;
        orthoBoundBlock(range: GeInterval): GeBoundBlock3d;

        // Project methods.
        //
        project(projectionPlane: GePlane, projectDirection: GeVector3d): GeEntity3d;
        project(projectionPlane: GePlane, projectDirection: GeVector3d, tol: GeTol): GeEntity3d;
        orthoProject(projectionPlane: GePlane): GeEntity3d;
        orthoProject(projectionPlane: GePlane, tol: GeTol): GeEntity3d;

        // Tests if point is on curve.
        //
        isOn(pnt: GePoint3d): boolean;
        isOn(pnt: GePoint3d, tol: GeTol): boolean;
        isOn(param: number): boolean;
        isOn(param: number, tol: GeTol): boolean;

        // Parameter of the point on curve.  Contract: point IS on curve
        //
        paramOf(pnt: GePoint3d): number;
        paramOf(pnt: GePoint3d, tol: GeTol): number;

        // Return the offset of the curve.
        //
        //getTrimmedOffset(distance: number, planeNormal: GeVector3d, offsetCurveList: GeVoidPointerArray): void;
        //getTrimmedOffset(distance: number, planeNormal: GeVector3d, offsetCurveList: GeVoidPointerArray, extensionType: OffsetCrvExtType): void;
        //getTrimmedOffset(distance: number, planeNormal: GeVector3d, offsetCurveList: GeVoidPointerArray, extensionType: OffsetCrvExtType, tol: GeTol): void;

        // Geometric inquiry methods.
        //
        isClosed(): boolean;
        isClosed(tol: GeTol): boolean;
        isPlanar(plane: GePlane): boolean;
        isPlanar(plane: GePlane, tol: GeTol): boolean;
        isLinear(line: GeLine3d): boolean;
        isLinear(line: GeLine3d, tol: GeTol): boolean;
        isCoplanarWith(curve3d: GeCurve3d, plane: GePlane): boolean;
        isCoplanarWith(curve3d: GeCurve3d, plane: GePlane, tol: GeTol): boolean;

        // Length based methods.
        //
        length(): number;
        length(fromParam: number, toParam: number): number;
        length(fromParam: number, toParam: number, tol: number): number;
        paramAtLength(datumParam: number, length: number): number;
        paramAtLength(datumParam: number, length: number, tol: number): number;
        area(): number;
        area(tol: GeTol): number;


        // Modify methods.
        //
        getSplitCurves(param: number, piece1: GeCurve3d, piece2: GeCurve3d): void;

        // Explode curve into its component sub-curves.
        //
        //explode(explodedCurves: GeVoidPointerArray, newExplodedCurves: GeIntArray): boolean;

        // Return start and end points.
        //
        hasStartPoint(startPoint: GePoint3d): boolean;
        hasEndPoint(endPoint: GePoint3d): boolean;

        // Evaluate methods.
        //
        evalPoint(param: number): GePoint3d;

        // Polygonize curve to within a specified tolerance.
        // Note: forceResampling will make sure that the actual error is
        //       as close to approxEps as possible
        getSamplePoints(fromParam: number, toParam: number, approxEps: number, pointArray: GePoint3dArray, paramArray: GeDoubleArray): void;
        getSamplePoints(fromParam: number, toParam: number, approxEps: number, pointArray: GePoint3dArray, paramArray: GeDoubleArray, forceResampling: boolean): void;
        getSamplePoints(numSample: number, pointArray: GePoint3dArray): void;
        getSamplePoints(numSample: number, pointArray: GePoint3dArray, paramArray: GeDoubleArray): void;
    }

    //GeLinearEnt3d
    abstract class GeLinearEnt3d extends GeCurve3d {
        static getVertical(pt: GePoint3d, line: GeLinearEnt3d): GePoint3d;
        static getVertical(pt: GePoint3d, line: GeLinearEnt3d, tol: GeTol): GePoint3d;

        // Intersection with other geometric objects.
        //
        intersectWith(line: GeLinearEnt3d, intPnt: GePoint3d): boolean;
        intersectWith(line: GeLinearEnt3d, intPnt: GePoint3d, tol: GeTol): boolean;
        intersectWith(plane: GePlanarEnt, intPnt: GePoint3d): boolean;
        intersectWith(plane: GePlanarEnt, intPnt: GePoint3d, tol: GeTol): boolean;

        // Projection-intersection with other geometric objects.
        //
        projIntersectWith(line: GeLinearEnt3d, projDir: GeVector3d, pntOnThisLine: GePoint3d, pntOnOtherLine: GePoint3d): boolean;
        projIntersectWith(line: GeLinearEnt3d, projDir: GeVector3d, pntOnThisLine: GePoint3d, pntOnOtherLine: GePoint3d, tol: GeTol): boolean;

        // Containment tests.
        //
        isOn(pnt: GePoint3d): boolean;
        isOn(pnt: GePoint3d, tol: GeTol): boolean;
        isOn(param: number): boolean;
        isOn(param: number, tol: GeTol): boolean;
        isOn(plane: GePlane): boolean;
        isOn(plane: GePlane, tol: GeTol): boolean;

        // Direction tests.
        //
        isParallelTo(line: GeLinearEnt3d): boolean;
        isParallelTo(line: GeLinearEnt3d, tol: GeTol): boolean;
        isParallelTo(plane: GePlanarEnt): boolean;
        isParallelTo(plane: GePlanarEnt, tol: GeTol): boolean;
        isPerpendicularTo(cline: GeLinearEnt3d): boolean;
        isPerpendicularTo(line: GeLinearEnt3d, tol: GeTol): boolean;
        isPerpendicularTo(plane: GePlanarEnt): boolean;
        isPerpendicularTo(plane: GePlanarEnt, tol: GeTol): boolean;
        // Test if two lines are colinear.
        //
        isColinearTo(line: GeLinearEnt3d): boolean;
        isColinearTo(line: GeLinearEnt3d, tol: GeTol): boolean;

        // Perpendicular through a given point
        //
        getPerpLine(pnt: GePoint3d, perpLine: GeLine3d): void;

        // Definition of line.
        //
        pointOnLine(): GePoint3d;
        direction(): GeVector3d;
        getLine(line: GeLine3d): void;
    }

    //GeRay3d
    class GeRay3d extends GeLinearEnt3d {
        constructor();
        constructor(line: GeRay3d);
        constructor(pnt1: GePoint3d, pnt2: GePoint3d);

        set(pnt: GePoint3d, vec: GeVector3d): GeRay3d;
        set(pnt1: GePoint3d, pnt2: GePoint3d): GeRay3d;
    }

    //GeLine3d
    class GeLine3d extends GeLinearEnt3d {
        constructor();
        constructor(line: GeLine3d);
        constructor(pnt1: GePoint3d, pnt2: GePoint3d);

        static kXAxis: GeLine3d;
        static kYAxis: GeLine3d;
        static kZAxis: GeLine3d;

        set(pnt: GePoint3d, vec: GeVector3d): GeLine3d;
        set(pnt1: GePoint3d, pnt2: GePoint3d): GeLine3d;
    }

    //GeLineSeg3d
    class GeLineSeg3d extends GeLinearEnt3d {
        constructor();
        constructor(line: GeLineSeg3d);
        constructor(pnt1: GePoint3d, pnt2: GePoint3d);

        // Set methods.
        //
        set(pnt: GePoint3d, vec: GeVector3d): GeLineSeg3d;
        set(pnt1: GePoint3d, pnt2: GePoint3d): GeLineSeg3d;

        // Bisector.
        //
        getBisector(line: GeLine3d): void;

        // Barycentric combination of end points.
        //
        baryComb(blendCoeff: number): GePoint3d;

        // Definition of linear segment
        //
        startPoint(): GePoint3d;
        midPoint(): GePoint3d;
        endPoint(): GePoint3d;
    }

    //GeCircArc3d
    class GeCircArc3d extends GeCurve3d {
        constructor();
        constructor(arc: GeCircArc3d);
        constructor(cent: GePoint3d, radius: number);
        constructor(cent: GePoint3d, radius: number, startAngle: number, endAngle: number);
        constructor(cent: GePoint3d, radius: number, startAngle: number, endAngle: number, refVec: GeVector3d);
        constructor(cent: GePoint3d, radius: number, startAngle: number, endAngle: number, refVec: GeVector3d, isClockWise: boolean);
        constructor(startPoint: GePoint3d, point: GePoint3d, endPoint: GePoint3d);

        // Intersection with other geometric objects.
        //
        intersectWith(line: GeLinearEnt3d): GePoint3dArray;
        intersectWith(line: GeLinearEnt3d, tol: GeTol): GePoint3dArray;
        intersectWith(arc: GeCircArc3d): GePoint3dArray;
        intersectWith(arc: GeCircArc3d, tol: GeTol): GePoint3dArray;

        // Tangent line to the circular arc at given point.
        //
        tangent(pnt: GePoint3d, line: GeLine3d): boolean;
        tangent(pnt: GePoint3d, line: GeLine3d, tol: GeTol): boolean;


        // Test if point is inside circle.
        //
        isInside(pnt: GePoint3d): boolean;
        isInside(pnt: GePoint3d, tol: GeTol): boolean;

        // Definition of circular arc
        //
        center(): GePoint3d;
        radius(): number;
        startAng(): number;
        endAng(): number;
        refVec(): GeVector3d;
        startPoint(): GePoint3d;
        endPoint(): GePoint3d;
        isClockWise(): boolean;

        setCenter(cent: GePoint3d): GeCircArc3d;
        setRadius(radius: number): GeCircArc3d;
        setAngles(startAng: number, endAng: number): GeCircArc3d;

        // Reverses the direction of the arc while keeping the same startand endpoints.For bounded arcs, 
        // this function returns the complement of the original arc.
        //GeCircArc3d& setToComplement();
        set(cent: GePoint3d, nrm: GeVector3d, radius: number): GeCircArc3d;
        set(cent: GePoint3d, nrm: GeVector3d, refVec: GeVector3d, radius: number, ang1: number, ang2: number): GeCircArc3d;
        set(startPoint: GePoint3d, pnt: GePoint3d, endPoint: GePoint3d): GeCircArc3d;
        set(startPoint: GePoint3d, endPoint: GePoint3d, bulge: number): GeCircArc3d;

        // Possible errors:  kEqualArg1Arg2, kEqualArg1Arg3, kEqualArg2Arg3, 
        // kLinearlyDependentArg1Arg2Arg3.
        // Degenerate results: none.
        // On error, the object is unchanged.

        // If bulgeFlag is kTrue, then bulge is interpreted to be the maximum
        // distance between the arc and the chord between the two input points.
        // If bulgeFlag is kFalse, then bulge is interpreted to be tan(ang/4),
        // where ang is the angle of the arc segment between the two input points.
        set(startPoint: GePoint3d, endPoint: GePoint3d, bulge: number): GeCircArc3d;
        set(startPoint: GePoint3d, endPoint: GePoint3d, bulge: number, bulgeFlag: boolean): GeCircArc3d;
    }
}



import Ge = Module;
export default Ge;


