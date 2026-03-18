#include "GeLibVersion.h"




GeLibVersion::GeLibVersion()
{
    for (int i = 0; i < 10; i++)
    {
        this->mVersion[i] = 0;
    }
}
GeLibVersion::GeLibVersion(const GeLibVersion& source)
{
    for (int i = 0; i < 10; i++)
    {
        this->mVersion[i] = source.mVersion[i];
    }
 }
GeLibVersion::GeLibVersion(unsigned char major,unsigned char minor,unsigned char corrective,unsigned char schema)
{
    this->mVersion[0] = major;
    this->mVersion[1] = minor;
    this->mVersion[2] = corrective;
    this->mVersion[3] = schema;
}

/** \details
Returns the major version of GeLib.
*/
unsigned char GeLibVersion::majorVersion() const
{
    return this->mVersion[0];
}

/** \details
Returns the minor version of GeLib.
*/
unsigned char GeLibVersion::minorVersion() const
{
    return this->mVersion[1];
}

/** \details
Returns the corrective version of GeLib.
*/
unsigned char GeLibVersion::correctiveVersion() const
{
    return this->mVersion[2];
}
   
/** \details
Returns the schema version of GeLib.
*/
unsigned char GeLibVersion::schemaVersion() const
{
    return this->mVersion[3];
}

/** \details
Sets the major version of GeLib.

\param major [in]  Major version.
*/
GeLibVersion& GeLibVersion::setMajorVersion(unsigned char major)
{
    this->mVersion[0] = major;
    return *this;
}

/** \details
Sets the minor version of GeLib.

\param minor [in]  Minor version.
*/
GeLibVersion& GeLibVersion::setMinorVersion(unsigned char minor)
{
    this->mVersion[1] = minor;
    return *this;
}

/** \details
Sets the corrective version of GeLib.

\param corrective [in]  Corrective version.
*/
GeLibVersion& GeLibVersion::setCorrectiveVersion(unsigned char corrective)
{
    this->mVersion[2] = corrective;
    return *this;
}

/** \details
Sets the schema version of GeLib.

\param schema [in]  Schema version.
*/
GeLibVersion& GeLibVersion::setSchemaVersion(unsigned char schema)
{
    this->mVersion[3] = schema;
    return *this;
}

bool GeLibVersion::operator == (const GeLibVersion& libVersion) const
{
    if (this->majorVersion() != libVersion.majorVersion())
    {
        return false;
    }
    else if (this->minorVersion() != libVersion.minorVersion())
    {
        return false;
    }
    else if (this->correctiveVersion() != libVersion.correctiveVersion())
    {
        return false;
    }
    else if (this->schemaVersion() != libVersion.schemaVersion())
    {
        return false;
    }
    return true;
}

bool GeLibVersion::operator != (const GeLibVersion& libVersion) const
{
    return !(libVersion == *this);
}

bool GeLibVersion::operator < (const GeLibVersion& libVersion) const
{
    if (this->majorVersion() < libVersion.majorVersion())
    {
        return true;
    }
    return false;
}

bool GeLibVersion::operator <= (const GeLibVersion& libVersion) const
{
    if (this->majorVersion() <= libVersion.majorVersion())
    {
        return true;
    }
    return false;
}

bool GeLibVersion::operator > (const GeLibVersion& libVersion) const
{
    if (this->majorVersion() > libVersion.majorVersion())
    {
        return true;
    }
    return false;
}

bool GeLibVersion::operator >= (const GeLibVersion& libVersion) const
{
    if (this->majorVersion() >= libVersion.majorVersion())
    {
        return true;
    }
    return false;
}

const GeLibVersion GeLibVersion::kRelease0_95 = GeLibVersion();
const GeLibVersion GeLibVersion::kReleaseSed = GeLibVersion();
const GeLibVersion GeLibVersion::kReleaseTah = GeLibVersion();

