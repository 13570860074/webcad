#ifndef CMTRANSPARENCY_H
#define CMTRANSPARENCY_H


#include "EmObject.h"
#include "PlatformSettings.h"



/** \details
    <group OdCm_Classes>

    This class implements the Transparency object, which provides the transparency information about OdGiDrawable objects.
   
    \sa
    <link cm_transparency_sample.html, Example of Working with Transparency>

    \sa
    <link cm_overview.html, Overview of Classes that Implement Color and Transparency>

    <link cm_transparency_method.html, Methods of the Transparency Definition>
*/
class CmTransparency INHERIT_EM_OBJECT
{
public:
    enum transparencyMethod 
    {
        kByLayer    = 0, // Use the setting for the layer.
        kByBlock    = 1, // Use the setting for the block.
        kByAlpha    = 2, // Use the Alpha-value in this object.     
        kErrorValue = 3  // Error value.
    };
  
    /** \remarks
        The default transparency method is _kByLayer_.
    */
    CmTransparency() { setMethod(kByLayer); }
    CmTransparency(transparencyMethod method) { setMethod(method); }
    CmTransparency(UInt8 alpha) { setAlpha(alpha); }
    CmTransparency(double alphaPercent) { setAlphaPercent(alphaPercent); }
    CmTransparency(const CmTransparency& transparency) { m_AM = transparency.m_AM; }
    ~CmTransparency() {}
  
    CmTransparency& operator=(const CmTransparency& transparency) { m_AM = transparency.m_AM; return *this; }

    /** \details
        Compares two transparency objects as integer values and returns true when their values are equal, 
        or false when their values are not equal.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
    */
    bool operator==(const CmTransparency& transparency) const { return (m_AM == transparency.m_AM); }

    /** \details
        Compares two transparency objects as integer values and returns true when their values are not equal, 
        or false when their values are equal.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
    */
    bool operator!=(const CmTransparency& transparency) const { return (m_AM != transparency.m_AM); }
  
    /** \details
        Sets the alpha value and switches the transparency method to _kByAlpha_ for the transparency object.
    
        \param alpha [in]  Alpha as an integer value in range 0 to 255.

        \remarks
        An alpha value defines the degree of transparency.
        Alpha == 0 means totally clarity. Alpha == 255 means totally opaque.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
    */
    void setAlpha(UInt8 alpha){
        m_AM = ((UInt32)kByAlpha << 24) | alpha;
    }

    /** \details
        Returns the alpha value in range 0 to 255 of the transparency object.

        \remarks
        An alpha value defines the degree of transparency.
        Alpha == 0 means totally clarity. Alpha == 255 means totally opaque.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
    */
    UInt8 alpha() const
    {
        if (!isByAlpha())
        {
            return 255;
        }
        return UInt8(m_AM & 0xFF);
    }

    /** \details
        Sets the alpha value in percent and switches the transparency method to _kByAlpha_ for the transparency object.
    
        \param alphaPercent [in]  Alpha as a double value in range 0.0 (=0) to 1.0 (=255).

        \remarks
        An alpha value defines the degree of transparency.
        Alpha == 0.0 means totally clarity. Alpha == 1.0 means totally opaque.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
    */
    void setAlphaPercent(double alphaPercent)
    {
        if (alphaPercent < 0.0)
        {
            setAlpha(0);
        }
        else if (alphaPercent > 1.0)
        {
            setAlpha(0xFF);
        }
        else
        {
            setAlpha((UInt8)(alphaPercent * 255));
        }
    }

    /** \details
        Returns the alpha value in range 0.0 to 1.0 of the transparency object.

        \remarks
        An alpha value defines the degree of transparency.
        Alpha == 0.0 means totally clarity. Alpha == 1.0 means totally opaque.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
    */
    double alphaPercent() const
    {
        return (double)alpha() / 255;
    }


    /** \details
        Returns the transparency method of the transparency object as a value of the transparencyMethod enumeration.
    
        \remarks
        The transparency method can be a one of the following:
    
        <table>
        Name        Value   Description
        _kByLayer_    0       Transparency is specified by the layer object to which the object is resided.
        _kByBlock_    1       Transparency is specified by the block object in which the object is contained.
        _kByAlpha_    2       Transparency is specified by an Alpha-value.
        </table>

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    transparencyMethod method() const
    {
        return transparencyMethod(UInt8(m_AM >> 24));
    }

    /** \details
        Sets the transparency method for the transparency object.
    
        \param method [in]  Transparency method as the transparencyMethod enumeration.
    
        \remarks
        The transparency method can be a one of the following:
    
        <table>
        Name        Value   Description
        _kByLayer_    0       Transparency is specified by the layer object to which the object is resided.
        _kByBlock_    1       Transparency is specified by the block object in which the object is contained.
        _kByAlpha_    2       Transparency is specified by an Alpha-value.
        </table>

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    void setMethod(transparencyMethod method)
    {
        m_AM = (UInt32)method << 24;
    }

    /** \details
        Checks whether the transparency method is byAlpha for the transparency object and 
        returns true if and only if the transparency method is set to _kByAlpha_, otherwise it returns false.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    bool isByAlpha() const { return (method() == kByAlpha); }

    /** \details
        Checks whether the transparency method is byBlock for the transparency object and 
        returns true if and only if the transparency method is set to _kByBlock_, otherwise it returns false.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    bool isByBlock() const { return (method() == kByBlock); }
  
    /** \details
        Checks whether the transparency method is byLayer for the transparency object and 
        returns true if and only if the transparency method is set to _kByLayer_, otherwise it returns false.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    bool isByLayer() const { return (method() == kByLayer); }

    /** \details
        Checks whether the transparency method is error value for the transparency object and 
        returns true if and only if the transparency method is set to _kErrorValuer_, otherwise it returns false.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    bool isInvalid() const { return (method() == kErrorValue); }

    /** \details
        Checks whether the transparency method is byAlpha and object is totally clarity for the transparency object.
        It returns true if and only if the transparency method is set to _kByAlpha_ and Alpha == 0, 
        otherwise it returns false.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>

        <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
    */
    bool isClear() const   { return (method() == kByAlpha) && (alpha() == 0);}
  
    /** \details
        Checks whether the transparency method is byAlpha and object is totally opaque for the transparency object.
        It returns true if and only if the transparency method is set to _kByAlpha_ and Alpha == 255, 
        otherwise it returns false.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>

        <link cm_transparency_alpha.html, Transparency Functionality as an Alpha-value>
    */
    bool isSolid() const   { return (method() == kByAlpha) && (alpha() == 255);}
  

    /** \details
        Returns the packed 32-bits integer value that stores the transparency method and transparency components.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>

        <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
    */
    UInt32 serializeOut() const { return m_AM; }

    /** \details
        Sets the transparency method and transparency components as an integer value for the transparency object.

        \param transparency [in]  A packed 32-bits integer value that specifies the transparency method and transparency components.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>

        <link cm_transparency_integer.html, Transparency Functionality as an Integer-value>
    */
    void serializeIn(UInt32 transparency) { m_AM = transparency; }


private: 
    UInt32 m_AM;
};


#endif
