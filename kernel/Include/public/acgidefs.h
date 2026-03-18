
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2019 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _Gi_DEFS_H
#define _Gi_DEFS_H 1


// These are the current kinds of viewport regeneration modes.
// This mode is not settable by the user, but it can be queried 
// in case you need to take different actions for different
// regeneration modes.
//
typedef enum {
    eGiRegenTypeInvalid = 0,
    kGiStandardDisplay = 2,
    kGiHideOrShadeCommand,
    kGiRenderCommand, // deprecated
    kGiShadedDisplay = kGiRenderCommand,
    kGiForExplode,
    kGiSaveWorldDrawForProxy
} GiRegenType;

// No longer supported and will be removed
//
#define kGiSaveWorldDrawForR12 kGiForExplode 

// These are the current face filling modes
//
typedef enum {
     kGiFillAlways = 1,
     kGiFillNever
} GiFillType;

// These are the edge visibility types
//
typedef enum {
    kGiInvisible = 0,
    kGiVisible,
    kGiSilhouette
} GiVisibility;

// These are the types of arcs
//
typedef enum {
    kGiArcSimple = 0,                 // just the arc (not fillable)
    kGiArcSector,                     // area bounded by the arc and its center of curvature
    kGiArcChord                       // area bounded by the arc and its end points
} GiArcType;

// These are the possible types of vertex orientation 
// 
typedef enum {
    kGiCounterClockwise = -1,
    kGiNoOrientation = 0,
    kGiClockwise = 1
} GiOrientationType;

// This signifies how to calculate maximum deviation for tessellation
//
typedef enum {
    kGiMaxDevForCircle = 0,
    kGiMaxDevForCurve,
    kGiMaxDevForBoundary,
    kGiMaxDevForIsoline,
    kGiMaxDevForFacet
} GiDeviationType;

// Raster image organization
//
typedef enum {
    kGiBitonal,
    kGiPalette,
    kGiGray,
    kGiRGBA,
    kGiBGRA,
    kGiARGB,
    kGiABGR,
    kGiBGR,
    kGiRGB                              
} GiImageOrg;

// Raster image orientation
//
typedef enum {
    kGiXLeftToRightTopFirst,
    kGiXLeftToRightBottomFirst,
    kGiXRightToLeftTopFirst,
    kGiXRightToLeftBottomFirst,
    kGiYTopToBottomLeftFirst,
    kGiYTopToBottomRightFirst,
    kGiYBottomToTopLeftFirst,
    kGiYBottomToTopRightFirst
} GiImageOrient;

// scale filter method to use
typedef enum {
    kDefaultScaleFilter,
    kNearestScaleFilter,
    kBellScaleFilter,
    kMitchellScaleFilter,
    kTriangleScaleFilter,
    kCubicScaleFilter,
    kBsplineScaleFilter,
    kLanczos3ScaleFilter
} GiScaleFilterType;

// rotation filter method to use
typedef enum {
    kDefaultRotationFilter,
    kInterpolatedRotationFilter,
    kNearestRotationFilter,
} GiRotationFilterType;

// how to interpret nRasterRatio
typedef enum {
    kDefaultScale,                  // classic scaling scheme
    kRelativeScale,                 // relative to original image, do rotation
    kUnTransformedScale             // original pixels, unscaled, unrotated
} GiScaleType;

// highlight style
enum GiHighlightStyle { 
    kGiHighlightNone,
    kGiHighlightCustom,
    kGiHighlightDashedAndThicken,
    kGiHighlightDim,
    kGiHighlightThickDim,
    kGiHighlightGlow
};

enum GiEdgeStyleMaskValues {
    kGiNoEdgeStyleMask = 0,
    kGiJitterMask      = 1,
    kGiOverhangMask    = 2,
    kGiEdgeColorMask   = 4
};
typedef unsigned int GiEdgeStyleMask;



#endif
