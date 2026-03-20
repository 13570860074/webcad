#include "GeCurveBoundary.h"

#include "GeCurve2d.h"
#include "GeEntity2d.h"
#include "GeEntity3d.h"
#include "GePosition3d.h"

GeCurveBoundary::GeCurveBoundary()
    : m_iNumCurves(0)
    , m_pArrCrv3d(NULL)
    , m_pArrCrv2d(NULL)
    , m_bArrOrientation3d(NULL)
    , m_bArrOrientation2d(NULL)
    , m_bMakeCopy(false)
{
}

GeCurveBoundary::GeCurveBoundary(
    int numCurves,
    const GeEntity3d* const* crv3d,
    const GeCurve2d* const* crv2d,
    bool* orientation3d,
    bool* orientation2d,
    bool makeCopy)
    : m_iNumCurves(0)
    , m_pArrCrv3d(NULL)
    , m_pArrCrv2d(NULL)
    , m_bArrOrientation3d(NULL)
    , m_bArrOrientation2d(NULL)
    , m_bMakeCopy(false)
{
    set(numCurves, crv3d, crv2d, orientation3d, orientation2d, makeCopy);
}

GeCurveBoundary::GeCurveBoundary(const GeCurveBoundary& source)
    : m_iNumCurves(0)
    , m_pArrCrv3d(NULL)
    , m_pArrCrv2d(NULL)
    , m_bArrOrientation3d(NULL)
    , m_bArrOrientation2d(NULL)
    , m_bMakeCopy(false)
{
    set(
        source.m_iNumCurves,
        const_cast<const GeEntity3d* const*>(source.m_pArrCrv3d),
        const_cast<const GeCurve2d* const*>(source.m_pArrCrv2d),
        source.m_bArrOrientation3d,
        source.m_bArrOrientation2d,
        source.m_bMakeCopy);
}

GeCurveBoundary::~GeCurveBoundary()
{
    clearData();
}

GeCurveBoundary& GeCurveBoundary::operator = (const GeCurveBoundary& crvBoundary)
{
    if (this != &crvBoundary) {
        set(
            crvBoundary.m_iNumCurves,
            const_cast<const GeEntity3d* const*>(crvBoundary.m_pArrCrv3d),
            const_cast<const GeCurve2d* const*>(crvBoundary.m_pArrCrv2d),
            crvBoundary.m_bArrOrientation3d,
            crvBoundary.m_bArrOrientation2d,
            crvBoundary.m_bMakeCopy);
    }
    return *this;
}

bool GeCurveBoundary::isDegenerate() const
{
    if (m_iNumCurves != 1 || m_pArrCrv3d == NULL || m_pArrCrv3d[0] == NULL) {
        return false;
    }
    return m_pArrCrv3d[0]->type() == Ge::kPosition3d;
}

bool GeCurveBoundary::isDegenerate(GePosition3d& degenPoint, GeCurve2d** paramCurve) const
{
    if (!isDegenerate()) {
        return false;
    }

    GePosition3d* pos = dynamic_cast<GePosition3d*>(m_pArrCrv3d[0]);
    if (pos == NULL) {
        return false;
    }

    degenPoint.set(pos->point3d());

    if (paramCurve != NULL) {
        *paramCurve = NULL;
        if (m_pArrCrv2d != NULL && m_pArrCrv2d[0] != NULL) {
            GeEntity2d* cloned = m_pArrCrv2d[0]->copy();
            *paramCurve = dynamic_cast<GeCurve2d*>(cloned);
            if (*paramCurve == NULL) {
                delete cloned;
            }
        }
    }

    return true;
}

int GeCurveBoundary::numElements() const
{
    return m_iNumCurves;
}

void GeCurveBoundary::getContour(
    int& numCurves,
    GeEntity3d*** crv3d,
    GeCurve2d*** crv2d,
    bool** orientation3d,
    bool** orientation2d) const
{
    numCurves = m_iNumCurves;

    if (crv3d != NULL) {
        *crv3d = NULL;
        if (m_iNumCurves > 0 && m_pArrCrv3d != NULL) {
            *crv3d = new GeEntity3d*[m_iNumCurves];
            for (int i = 0; i < m_iNumCurves; ++i) {
                (*crv3d)[i] = m_pArrCrv3d[i];
            }
        }
    }

    if (crv2d != NULL) {
        *crv2d = NULL;
        if (m_iNumCurves > 0 && m_pArrCrv2d != NULL) {
            *crv2d = new GeCurve2d*[m_iNumCurves];
            for (int i = 0; i < m_iNumCurves; ++i) {
                (*crv2d)[i] = m_pArrCrv2d[i];
            }
        }
    }

    if (orientation3d != NULL) {
        *orientation3d = NULL;
        if (m_iNumCurves > 0 && m_bArrOrientation3d != NULL) {
            *orientation3d = new bool[m_iNumCurves];
            for (int i = 0; i < m_iNumCurves; ++i) {
                (*orientation3d)[i] = m_bArrOrientation3d[i];
            }
        }
    }

    if (orientation2d != NULL) {
        *orientation2d = NULL;
        if (m_iNumCurves > 0 && m_bArrOrientation2d != NULL) {
            *orientation2d = new bool[m_iNumCurves];
            for (int i = 0; i < m_iNumCurves; ++i) {
                (*orientation2d)[i] = m_bArrOrientation2d[i];
            }
        }
    }
}

GeCurveBoundary& GeCurveBoundary::set(
    int numCurves,
    const GeEntity3d* const* crv3d,
    const GeCurve2d* const* crv2d,
    bool* orientation3d,
    bool* orientation2d,
    bool makeCopy)
{
    clearData();

    if (numCurves <= 0) {
        return *this;
    }

    bool has3d = (crv3d != NULL);
    bool has2d = (crv2d != NULL);
    allocateArrays(numCurves, has3d, has2d);

    m_bMakeCopy = makeCopy;

    for (int i = 0; i < m_iNumCurves; ++i) {
        if (m_pArrCrv3d != NULL) {
            m_pArrCrv3d[i] = NULL;
            if (crv3d[i] != NULL) {
                m_pArrCrv3d[i] = makeCopy ? crv3d[i]->copy() : const_cast<GeEntity3d*>(crv3d[i]);
            }
        }

        if (m_pArrCrv2d != NULL) {
            m_pArrCrv2d[i] = NULL;
            if (crv2d[i] != NULL) {
                if (makeCopy) {
                    GeEntity2d* cloned = crv2d[i]->copy();
                    m_pArrCrv2d[i] = dynamic_cast<GeCurve2d*>(cloned);
                    if (m_pArrCrv2d[i] == NULL) {
                        delete cloned;
                    }
                }
                else {
                    m_pArrCrv2d[i] = const_cast<GeCurve2d*>(crv2d[i]);
                }
            }
        }

        if (m_bArrOrientation3d != NULL) {
            m_bArrOrientation3d[i] = (orientation3d != NULL) ? orientation3d[i] : true;
        }
        if (m_bArrOrientation2d != NULL) {
            m_bArrOrientation2d[i] = (orientation2d != NULL) ? orientation2d[i] : true;
        }
    }

    return *this;
}

bool GeCurveBoundary::isOwnerOfCurves() const
{
    return m_bMakeCopy;
}

GeCurveBoundary& GeCurveBoundary::setToOwnCurves()
{
    if (m_bMakeCopy || m_iNumCurves <= 0) {
        m_bMakeCopy = true;
        return *this;
    }

    if (m_pArrCrv3d != NULL) {
        for (int i = 0; i < m_iNumCurves; ++i) {
            if (m_pArrCrv3d[i] != NULL) {
                m_pArrCrv3d[i] = m_pArrCrv3d[i]->copy();
            }
        }
    }

    if (m_pArrCrv2d != NULL) {
        for (int i = 0; i < m_iNumCurves; ++i) {
            if (m_pArrCrv2d[i] != NULL) {
                GeEntity2d* cloned = m_pArrCrv2d[i]->copy();
                m_pArrCrv2d[i] = dynamic_cast<GeCurve2d*>(cloned);
                if (m_pArrCrv2d[i] == NULL) {
                    delete cloned;
                }
            }
        }
    }

    m_bMakeCopy = true;
    return *this;
}

void GeCurveBoundary::clearData()
{
    if (m_bMakeCopy) {
        if (m_pArrCrv3d != NULL) {
            for (int i = 0; i < m_iNumCurves; ++i) {
                delete m_pArrCrv3d[i];
            }
        }
        if (m_pArrCrv2d != NULL) {
            for (int i = 0; i < m_iNumCurves; ++i) {
                delete m_pArrCrv2d[i];
            }
        }
    }

    delete[] m_pArrCrv3d;
    delete[] m_pArrCrv2d;
    delete[] m_bArrOrientation3d;
    delete[] m_bArrOrientation2d;

    m_iNumCurves = 0;
    m_pArrCrv3d = NULL;
    m_pArrCrv2d = NULL;
    m_bArrOrientation3d = NULL;
    m_bArrOrientation2d = NULL;
    m_bMakeCopy = false;
}

void GeCurveBoundary::allocateArrays(int numCurves, bool has3d, bool has2d)
{
    m_iNumCurves = numCurves;

    if (has3d) {
        m_pArrCrv3d = new GeEntity3d*[numCurves];
        m_bArrOrientation3d = new bool[numCurves];
    }

    if (has2d) {
        m_pArrCrv2d = new GeCurve2d*[numCurves];
        m_bArrOrientation2d = new bool[numCurves];
    }
}