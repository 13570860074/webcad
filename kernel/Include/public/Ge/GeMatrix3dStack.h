#ifndef GEMATRIX3DSTACK_H
#define GEMATRIX3DSTACK_H

#include "AcArray.h"
#include "GeMatrix3d.h"

class GeMatrix3dStack
{
public:
    GeMatrix3dStack() {}

    bool empty() const
    {
        return m_data.length() == 0;
    }

    int size() const
    {
        return m_data.length();
    }

    void clear()
    {
        m_data.removeAll();
    }

    void push(const GeMatrix3d& value)
    {
        m_data.append(value);
    }

    GeMatrix3d pop()
    {
        if (m_data.length() == 0) {
            return GeMatrix3d::kIdentity;
        }

        const unsigned int lastIndex = static_cast<unsigned int>(m_data.length() - 1);
        GeMatrix3d value = m_data.at(lastIndex);
        m_data.removeAt(lastIndex);
        return value;
    }

    GeMatrix3d& top()
    {
        if (m_data.length() == 0) {
            m_data.append(GeMatrix3d::kIdentity);
        }
        return m_data.at(static_cast<unsigned int>(m_data.length() - 1));
    }

    const GeMatrix3d& top() const
    {
        if (m_data.length() == 0) {
            return GeMatrix3d::kIdentity;
        }
        return m_data.at(static_cast<unsigned int>(m_data.length() - 1));
    }

private:
    AcArray<GeMatrix3d> m_data;
};

#endif
