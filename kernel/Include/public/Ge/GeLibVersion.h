#ifndef GELIBVERSION_H
#define GELIBVERSION_H

#include "GeExport.h"


#define IMAGE_MAJOR_VER 2           /*!DOM*/
#define IMAGE_MINOR_VER 0           /*!DOM*/
#define IMAGE_CORRECTIVE_VER 0      /*!DOM*/
#define IMAGE_INTERNAL_VER 0        /*!DOM*/

/** \details
    This class provides management of GeLib versions.

    Corresponding C++ library: TD_Ge

    <group Ge_Classes> 
*/
class GeLibVersion
{
public:
  /** \param major [in]  Major version.
    \param minor [in]  Minor version.
    \param corrective [in]  Corrective version.
    \param schema [in]  Schema version.
  */
  GeLibVersion ();
  GeLibVersion (const GeLibVersion& source);
  GeLibVersion (
    unsigned char major, 
    unsigned char minor,
    unsigned char corrective,
    unsigned char schema);

  /** \details
    Returns the major version of GeLib.
  */
  unsigned char majorVersion () const; 

  /** \details
    Returns the minor version of GeLib.
  */
  unsigned char minorVersion () const; 

  /** \details
    Returns the corrective version of GeLib.
  */
  unsigned char correctiveVersion () const;
   
  /** \details
    Returns the schema version of GeLib.
  */
  unsigned char schemaVersion () const;

  /** \details
    Sets the major version of GeLib.

    \param major [in]  Major version.
  */
  GeLibVersion& setMajorVersion (
    unsigned char major); 

  /** \details
    Sets the minor version of GeLib.

    \param minor [in]  Minor version.
  */
  GeLibVersion& setMinorVersion (
    unsigned char minor); 

  /** \details
    Sets the corrective version of GeLib.

    \param corrective [in]  Corrective version.
  */
  GeLibVersion& setCorrectiveVersion (
    unsigned char corrective); 

  /** \details
    Sets the schema version of GeLib.

    \param schema [in]  Schema version.
  */
  GeLibVersion& setSchemaVersion (
    unsigned char schema);

  bool operator == (
      const GeLibVersion& libVersion) const;

  bool operator != (
    const GeLibVersion& libVersion) const; 

  bool operator < (
    const GeLibVersion& libVersion) const;

  bool operator <= (
    const GeLibVersion& libVersion) const; 

  bool operator > (
    const GeLibVersion& libVersion) const; 

  bool operator >= (
    const GeLibVersion& libVersion) const;

  static const GeLibVersion kRelease0_95; // GeLib release 0.

  static const GeLibVersion kReleaseSed; // GeLib 14.0 release. 

  static const GeLibVersion kReleaseTah; // GeLib 15.0 release.

private:
  unsigned char mVersion[10];
};



#endif 

