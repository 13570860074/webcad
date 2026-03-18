#include "GiSubEntityTraits.h"



void GiSubEntityTraits::setMaterial(const DbObjectId /*materialId*/)
{
}

DbObjectId GiSubEntityTraits::materialId(void) const
{
    return NULL;
}


 void GiSubEntityTraits::setVisualStyle(DbObjectId /*visualStyleId*/)
{
}

DbObjectId GiSubEntityTraits::visualStyle(void) const
{
    return NULL;
}

void GiSubEntityTraits::setSectionable(bool /*bSectionable*/)
{
}

bool GiSubEntityTraits::sectionable(void) const
{
    return false;
}

Acad::ErrorStatus GiSubEntityTraits::setDrawFlags(UInt32 /*flags*/)
{
    return Acad::eNotImplementedYet;
}

UInt32 GiSubEntityTraits::drawFlags(void) const
{
    return 0;
}

Acad::ErrorStatus GiSubEntityTraits::setShadowFlags(ShadowFlags /*flags*/)
{
    return Acad::eNotImplementedYet;
}

GiSubEntityTraits::ShadowFlags GiSubEntityTraits::shadowFlags(void) const
{
    return kShadowsCastAndReceive;
}

void GiSubEntityTraits::setSelectionGeom(bool)
{
}

bool GiSubEntityTraits::selectionGeom(void) const
{
    return false;
}

void GiSubEntityTraits::setTransparency(const CmTransparency&)
{
}


CmTransparency GiSubEntityTraits::transparency(void) const
{
    return CmTransparency();   // empty object
}

void GiSubEntityTraits::setFill(const GiFill* /*pFill*/)
{
}

const GiFill* GiSubEntityTraits::fill(void) const
{
    return nullptr;
}

