
#ifndef CMENTITYCOLOR_H
#define CMENTITYCOLOR_H

#include "export.h"
#include "EmObject.h"
#include "PlatformSettings.h"
#include <math.h>

/** \details
    <group OdCm_Classes>

    This class implements the entity color object that represents the unnamed colors specified by
    the byLayer, byBlock, byColor, byACI, byPen, Foreground, byDgnIndex, or None color methods.

    \remarks
    The entity color object can specify any of the following color methods:

    <table>
    Name         Value    Description
    _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
    _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
    _kByColor_     0xC2     Color is specified by an RGB-value.
    _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
    _kByPen_       0xC4     Color is specified by an index into a pen color table.
    _kForeground_  0xC5     Color is foreground.
    _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
    _kNone_        0xC8     color is absent (object is clarity).
    </table>

    An entity has no color from the time it is first instantiated until it is assigned one or added
    to a database. The ColorMethod enumeration defines the constants for color methods.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_overview.html, Overview of Classes that Implement Color and Transparency>

    <link cm_color_method.html, Methods of the Color Definition>

    The OdCmColorBase class,  the OdCmColor class
*/
class CmEntityColor INHERIT_EM_OBJECT
{
public:
    enum Color
    {
        kRed,
        kGreen,
        kBlue
    };

    /** \details
        Represents color method.
    */

    enum ColorMethod
    {
        /**Color of the layer on which it resides.*/
        kByLayer = 0xC0,
        /**Color of the block reference in which it is contained.*/
        kByBlock = 0xC1,
        /**Color specified by RGB value.*/
        kByColor = 0xC2,
        /**Color specified by an index (ACI) into a color palette.*/
        kByACI = 0xC3,
        /**Color specified by an index into a pen color table.*/
        kByPen = 0xC4,
        /**Editor foreground Color.*/
        kForeground = 0xC5,
        /**Color specified by an index into a dgn color table.*/
        kByDgnIndex = 0xC7,
        /**No Color.*/
        kNone = 0xC8
    };

    enum ACIcolorMethod
    {
        kACIbyBlock = 0,    // byBlock
        kACIforeground = 7, // Foreground
        kACIbyLayer = 256,  // byLayer

        kACIclear = 0, // Clear

        kACIRed = 1,     // Red
        kACIYellow = 2,  // Yellow
        kACIGreen = 3,   // Green
        kACICyan = 4,    // Cyan
        kACIBlue = 5,    // Blue
        kACIMagenta = 6, // Magenta
        kACIWhite = 7,   // White

        kACIstandard = 7,
        kACImaximum = 255,
        kACInone = 257, // None
        kACIminimum = -255
    };

    typedef UInt32 RGBM;

    /** \param colorMethod [in]  color method.
        \param red [in]  Red component.
        \param green [in]  Green component.
        \param blue [in]  Blue component.

        \remarks
        Default ColorMethod is kByColor.
    */
    CmEntityColor() : m_RGBM(0) { setColorMethod(kByColor); }
    CmEntityColor(const CmEntityColor &color) : m_RGBM(color.m_RGBM) {}
    CmEntityColor(ColorMethod colorMethod) : m_RGBM(0) { setColorMethod(colorMethod); }
    CmEntityColor(UInt8 red, UInt8 green, UInt8 blue)
    {
        setColorMethod(kByColor);
        setRGB(red, green, blue);
    }

    /** \details
        Copies an entity color object specified by right into the entity color object specified by left.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>
    */
    CmEntityColor &operator=(const CmEntityColor &color)
    {
        m_RGBM = color.m_RGBM;
        return *this;
    }

    /** \details
        Compares two entity color objects as integer values and returns true when their values are equal,
        or false when their values are not equal.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    bool operator==(const CmEntityColor &color) const
    {
        return m_RGBM == color.m_RGBM;
    }

    /** \details
        Compares two entity color objects as integer values and returns true when their values are not equal,
        or false when their values are equal.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    bool operator!=(const CmEntityColor &color) const
    {
        return m_RGBM != color.m_RGBM;
    }

    /** \details
        Sets the color method for the entity color object.

        \param colorMethod [in]  color method as the ColorMethod enumeration.

        \remarks
        The color method can be a one of the following:

        <table>
        Name         Value    Description
        _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
        _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_     0xC2     Color is specified by an RGB-value.
        _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
        _kByPen_       0xC4     Color is specified by an index into a pen color table.
        _kForeground_  0xC5     Color is foreground.
        _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
        _kNone_        0xC8     color is absent (object is clarity).
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    void setColorMethod(ColorMethod colorMethod)
    {
        CmEntityColor::static_setColorMethod(&m_RGBM, colorMethod);
    }

    /** \details
        Returns the color method of the entity color object as a value of the ColorMethod enumeration.

        \remarks
        The color method can be a one of the following:

        <table>
        Name         Value    Description
        _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
        _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_     0xC2     Color is specified by an RGB-value.
        _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
        _kByPen_       0xC4     Color is specified by an index into a pen color table.
        _kForeground_  0xC5     Color is foreground.
        _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
        _kNone_        0xC8     color is absent (object is clarity).
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    ColorMethod colorMethod() const
    {
        return static_colorMethod(&m_RGBM);
    }

    /** \details
        Sets the color method and color components for the entity color object as an integer value.

        \param color [in]  A packed 32-bits integer value that specifies the color method and color components.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    void setColor(UInt32 color) { this->m_RGBM = color; }

    /** \details
        Returns the packed 32-bits integer value that stores the color method and color components
        of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
      */
    UInt32 color() const { return this->m_RGBM; }

    /** \details
        Sets the color index (ACI) of a some color palette and sets the color method to byACI
        for the entity color object.

        \param colorIndex [in]  An integer value that is the index of the color in a some palette.

        \remarks
        The color index can be a one of the following:

        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red.
        _kACIYellow_        2       Yellow.
        _kACIGreen_         3       Green.
        _kACICyan_          4       Cyan.
        _kACIBlue_          5       Blue.
        _kACIMagenta_       6       Magenta.
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer.
        _kACInone_          257     Sets the color method to None.
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_index.html, Color Functionality as an ACI-value>
    */
    void setColorIndex(Int16 colorIndex) { static_setColorIndex(&m_RGBM, colorIndex); }

    /** \details
        Returns the color index (ACI) of the entity color object.

        \remarks
        The color index can be a one of the following:

        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red.
        _kACIYellow_        2       Yellow.
        _kACIGreen_         3       Green.
        _kACICyan_          4       Cyan.
        _kACIBlue_          5       Blue.
        _kACIMagenta_       6       Magenta.
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer.
        _kACInone_          257     Sets the color method to None.
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_index.html, Color Functionality as an ACI-value>
    */
    Int16 colorIndex() const { return CmEntityColor::static_colorIndex(&m_RGBM); }

    /** \details
        Sets the red, green, blue color components and the color method to byColor for the entity color object.

        \param red [in]  Red component as an integer value in range 0 to 255.
        \param green [in]  Green component as an integer value in range 0 to 255.
        \param blue [in]  Blue component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    void setRGB(UInt8 red, UInt8 green, UInt8 blue)
    {
        static_setRGB(&m_RGBM, red, green, blue);
    }

    /** \details
        Sets the red color component for the entity color object.

        \param red [in]  Red component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    void setRed(UInt8 red) { static_setRed(&m_RGBM, red); }

    /** \details
        Sets the green color component for the entity color object.

        \param green [in]  Green component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    void setGreen(UInt8 green) { static_setGreen(&m_RGBM, green); }

    /** \details
        Sets the blue color component for the entity color object.

        \param blue [in]  Blue component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    void setBlue(UInt8 blue) { static_setBlue(&m_RGBM, blue); }

    /** \details
    Returns the red color component of the entity color object as a value in range 0 to 255.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    UInt8 red() const { return static_red(&m_RGBM); }

    /** \details
        Returns the green color component of the entity color object as a value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    UInt8 green() const { return static_green(&m_RGBM); }

    /** \details
        Returns the blue color component of the entity color object as a value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    UInt8 blue() const { return static_blue(&m_RGBM); }

    /** \details
        Checks whether the color method is byColor for the entity color object and
        returns true if and only if the color method is set to _kByColor_, otherwise it returns false.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isByColor() const { return static_isByColor(&m_RGBM); }

    /** \details
        Checks whether the color method is byLayer for the entity color object and
        returns true if and only if the color method is set to _kByLayer_ or was set to _kACIbyLayer_,
        otherwise it returns false.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isByLayer() const { return static_isByLayer(&m_RGBM); }

    /** \details
        Checks whether the color method is byBlock for the entity color object and
        returns true if and only if the color method is set to _kByBlock_ or was set to _kACIbyBlock_,
        otherwise it returns false.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isByBlock() const { return static_isByBlock(&m_RGBM); }

    /** \details
        Checks whether the color method is byACI for the entity color object and
        returns true if and only if the color method is set to _kByACI_, otherwise it returns false.

        \remarks
        This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isByACI() const { return static_isByACI(&m_RGBM); }

    /** \details
        Checks whether the color method is Foreground for the entity color object and
        returns true if and only if the color method is set to _kForeground_ or was set to _kACIforeground_,
        otherwise it returns false.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isForeground() const { return static_isForeground(&m_RGBM); }

    /** \details
        Checks whether the color method is byDgnIndex for the entity color object and
        returns true if and only if the color method is set to _kByDgnIndex_, otherwise it returns false.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isByDgnIndex() const { return static_isByDgnIndex(&m_RGBM); }

    /** \details
        Checks whether the color method is None (invisible) for the entity color object and
        returns true if and only if the color method is set to _kNone_ or was set to _kACInone_,
        otherwise it returns false.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    bool isNone() const { return static_isNone(&m_RGBM); }

    /** \details
        Sets the color method for the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param colorMethod [in]  color method as the ColorMethod enumeration.

        \remarks
        The color method can be a one of the following:

        <table>
        Name         Value    Description
        _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
        _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_     0xC2     Color is specified by an RGB-value.
        _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
        _kByPen_       0xC4     Color is specified by an index into a pen color table.
        _kForeground_  0xC5     Color is foreground.
        _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
        _kNone_        0xC8     color is absent (object is clarity).
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static void static_setColorMethod(RGBM *pRGBM, ColorMethod colorMethod)
    {
        *pRGBM = (colorMethod << 24) | 0xFF;
    }

    /** \details
        Returns the color method of the entity color object as a value of the ColorMethod enumeration.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \remarks
        The color method can be a one of the following:

        <table>
        Name         Value    Description
        _kByLayer_     0xC0     Color is specified by the layer object to which the object is resided.
        _kByBlock_     0xC1     Color is specified by the block object in which the object is contained.
        _kByColor_     0xC2     Color is specified by an RGB-value.
        _kByACI_       0xC3     Color is specified by an index (ACI) of a some color palette.
        _kByPen_       0xC4     Color is specified by an index into a pen color table.
        _kForeground_  0xC5     Color is foreground.
        _kByDgnIndex_  0xC7     Color is specified by an index into a dgn color table.
        _kNone_        0xC8     color is absent (object is clarity).
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static ColorMethod static_colorMethod(const RGBM *pRGBM)
    {
        return CmEntityColor::ColorMethod((*pRGBM >> 24) & 0xFF);
    }

    /** \details
        Sets the color method and color components for the entity color object as an integer value.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param color [in]  A packed 32-bits integer value that specifies the color method and color components.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    static void static_setColor(RGBM *pRGBM, UInt32 color)
    {
        *pRGBM = color;
    }

    /** \details
        Returns the packed 32-bits integer value that stores the color method and color components
        of the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    static UInt32 static_color(const RGBM *pRGBM) { return *pRGBM; }

    /** \details
        Sets the color index (ACI) of a some color palette and sets the color method to byACI
        for the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param colorIndex [in]  An integer value that is the index of the color in a some palette.

        \remarks
        The color index can be a one of the following:

        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red.
        _kACIYellow_        2       Yellow.
        _kACIGreen_         3       Green.
        _kACICyan_          4       Cyan.
        _kACIBlue_          5       Blue.
        _kACIMagenta_       6       Magenta.
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                  8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer.
        _kACInone_          257     Sets the color method to None.
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_index.html, Color Functionality as an ACI-value>
    */
    static void static_setColorIndex(RGBM *pRGBM, Int16 colorIndex)
    {
        if (colorIndex == 0)
        {
            static_setColorMethod(pRGBM, CmEntityColor::ColorMethod::kByBlock);
        }
        else if (colorIndex == 256)
        {
            static_setColorMethod(pRGBM, CmEntityColor::ColorMethod::kByLayer);
        }
        else if (colorIndex > 256)
        {
            static_setColorMethod(pRGBM, CmEntityColor::ColorMethod::kNone);
        }
        else
        {
            *pRGBM = (CmEntityColor::ColorMethod::kByACI << 24) | colorIndex;
        }
    }

    /** \details
        Returns the color index (ACI) of the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \remarks
        The color index can be a one of the following:

        <table>
        Name              Value   Description
        _kACIbyBlock_       0       Sets the color method to byBlock.
        _kACIRed_           1       Red.
        _kACIYellow_        2       Yellow.
        _kACIGreen_         3       Green.
        _kACICyan_          4       Cyan.
        _kACIBlue_          5       Blue.
        _kACIMagenta_       6       Magenta.
        _kACIforeground_    7       Sets the color method to Foreground.
        ..                  8-255   Defined by display device.
        _kACIbyLayer_       256     Sets the color method to byLayer.
        _kACInone_          257     Sets the color method to None.
        </table>

        An entity does not have a color from the time it is first instantiated until it is assigned one
        or added to a database.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_index.html, Color Functionality as an ACI-value>
    */
    static Int16 static_colorIndex(const RGBM *pRGBM)
    {
        ColorMethod method = static_colorMethod(pRGBM);
        if (method == ColorMethod::kByColor)
        {
            int r = static_red(pRGBM);
            int g = static_green(pRGBM);
            int b = static_blue(pRGBM);
            return rgbToColorIndex(r, g, b);
        }
        else if (method == ColorMethod::kByBlock)
        {
            return 0;
        }
        else if (method == ColorMethod::kByLayer)
        {
            return 256;
        }
        else if (method == ColorMethod::kByACI)
        {
            return *pRGBM & 255;
        }
        return 7;
    }

    /** \details
        Sets the red, green, blue color components and the color method to byColor for the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param red [in]  Red component as an integer value in range 0 to 255.
        \param green [in]  Green component as an integer value in range 0 to 255.
        \param blue [in]  Blue component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static void static_setRGB(RGBM *pRGBM, UInt8 red, UInt8 green, UInt8 blue)
    {
        static_setColorMethod(pRGBM, kByColor);
        static_setRed(pRGBM, red);
        static_setGreen(pRGBM, green);
        static_setBlue(pRGBM, blue);
    }

    /** \details
        Sets the red color component for the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param red [in]  Red component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static void static_setRed(RGBM *pRGBM, UInt8 red)
    {
        if (static_colorMethod(pRGBM) == kByColor)
        {
            *pRGBM &= 0xFF00FFFF;
            *pRGBM |= RGBM(((UInt32)red << 16) & 0x00FF0000);
        }
    }

    /** \details
        Sets the green color component for the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param green [in]  Green component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static void static_setGreen(RGBM *pRGBM, UInt8 green)
    {
        if (static_colorMethod(pRGBM) == kByColor)
        {
            *pRGBM &= 0xFFFF00FF;
            *pRGBM |= RGBM(((UInt32)green << 8) & 0x0000FF00);
        }
    }

    /** \details
        Sets the blue color component for the entity color object.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \param blue [in]  Blue component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static void static_setBlue(RGBM *pRGBM, UInt8 blue)
    {
        if (static_colorMethod(pRGBM) == kByColor)
        {
            *pRGBM &= 0xFFFFFF00;
            *pRGBM |= RGBM((blue)&0x000000FF);
        }
    }

    /** \details
        Returns the red color component of the entity color object as a value in range 0 to 255.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static UInt8 static_red(const RGBM *pRGBM)
    {
        ColorMethod method = static_colorMethod(pRGBM);
        if (method == ColorMethod::kByColor)
        {
            return UInt8((*pRGBM >> 16) & 0xFF);
        }
        else if (method == ColorMethod::kByACI)
        {
            int colorIndex = static_colorIndex(pRGBM);

            int r, g, b;
            colorIndextoRGB(colorIndex, r, g, b);
            return r;
        }

        return 0;
    }

    /** \details
        Returns the green color component of the entity color object as a value in range 0 to 255.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static UInt8 static_green(const RGBM *pRGBM)
    {
        ColorMethod method = static_colorMethod(pRGBM);
        if (method == ColorMethod::kByColor)
        {
            return UInt8((*pRGBM >> 8) & 0xFF);
        }
        else if (method == ColorMethod::kByACI)
        {
            int colorIndex = static_colorIndex(pRGBM);

            int r, g, b;
            colorIndextoRGB(colorIndex, r, g, b);
            return g;
        }

        return 0;
    }

    /** \details
        Returns the blue color component of the entity color object as a value in range 0 to 255.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    static UInt8 static_blue(const RGBM *pRGBM)
    {
        ColorMethod method = static_colorMethod(pRGBM);
        if (method == ColorMethod::kByColor)
        {
            return UInt8(*pRGBM & 0xFF);
        }
        else if (method == ColorMethod::kByACI)
        {
            int colorIndex = static_colorIndex(pRGBM);

            int r, g, b;
            colorIndextoRGB(colorIndex, r, g, b);
            return b;
        }

        return 0;
    }

    /** \details
        Checks whether the color method is byColor for the entity color object and
        returns true if and only if the color method is set to _kByColor_, otherwise it returns false.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static bool static_isByColor(const RGBM *pRGBM) { return static_colorMethod(pRGBM) == kByColor; }

    /** \details
    Checks whether the color method is byLayer for the entity color object and
    returns true if and only if the color method is set to _kByLayer_ or was set to _kACIbyLayer_,
    otherwise it returns false.

    \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

    \sa
    <link cm_color_sample_entity.html, Example of Working with the Entity Color>

    \sa
    <link cm_color_method.html, Methods of the Color Definition>
    */
    static bool static_isByLayer(const RGBM *pRGBM)
    {
        return (static_colorMethod(pRGBM) == kByLayer || (static_colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIbyLayer));
    }

    /** \details
        Checks whether the transparency method is byBlock for the transparency object and
        returns true if and only if the transparency method is set to _kByBlock_, otherwise it returns false.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_transparency_sample.html, Example of Working with Transparency>

        \sa
        <link cm_transparency_method.html, Methods of the Transparency Definition>
    */
    static bool static_isByBlock(const RGBM *pRGBM)
    {
        return (static_colorMethod(pRGBM) == kByBlock || (static_colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIbyBlock));
    }

    /** \details
        Checks whether the color method is byACI for the entity color object and
        returns true if and only if the color method is set to _kByACI_, otherwise it returns false.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.


        \remarks
        This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static bool static_isByACI(const RGBM *pRGBM)
    {
        return static_colorMethod(pRGBM) == kByACI;
    }

    /** \details
        Checks whether the color method is Foreground for the entity color object and
        returns true if and only if the color method is set to _kForeground_ or was set to _kACIforeground_,
        otherwise it returns false.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static bool static_isForeground(const RGBM *pRGBM)
    {
        return (static_colorMethod(pRGBM) == kForeground || (static_colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACIforeground));
    }

    /** \details
        Checks whether the color method is byDgnIndex for the entity color object and
        returns true if and only if the color method is set to _kByDgnIndex_, otherwise it returns false.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.
        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static bool static_isByDgnIndex(const RGBM *pRGBM)
    {
        return static_colorMethod(pRGBM) == kByDgnIndex;
    }

    /** \details
        Checks whether the color method is None (invisible) for the entity color object and
        returns true if and only if the color method is set to _kNone_ or was set to _kACInone_,
        otherwise it returns false.

        \param pRGBM [in]  Pointer to the m_RGBM member of the entity color object.

        \sa
        <link cm_color_sample_entity.html, Example of Working with the Entity Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    static bool static_isNone(const RGBM *pRGBM)
    {
        return (static_colorMethod(pRGBM) == kNone || (static_colorMethod(pRGBM) == kByACI && indirect(pRGBM) == kACInone));
    }

public:
    static void colorIndextoRGB(int _colorIndex, int &_r, int &_g, int &_b)
    {
        int r = 0;
        int g = 0;
        int b = 0;
        switch (_colorIndex)
        {
        case 1:
            r = 255;
            g = 0;
            b = 0;
            break;
        case 2:
            r = 255;
            g = 255;
            b = 0;
            break;
        case 3:
            r = 0;
            g = 255;
            b = 0;
            break;
        case 4:
            r = 0;
            g = 255;
            b = 255;
            break;
        case 5:
            r = 0;
            g = 0;
            b = 255;
            break;
        case 6:
            r = 255;
            g = 0;
            b = 255;
            break;
        case 7:
            r = 255;
            g = 255;
            b = 255;
            break;
        case 8:
            r = 128;
            g = 128;
            b = 128;
            break;
        case 9:
            r = 192;
            g = 192;
            b = 192;
            break;
        case 10:
            r = 255;
            g = 0;
            b = 0;
            break;
        case 11:
            r = 255;
            g = 127;
            b = 127;
            break;
        case 12:
            r = 204;
            g = 0;
            b = 0;
            break;
        case 13:
            r = 204;
            g = 102;
            b = 102;
            break;
        case 14:
            r = 153;
            g = 0;
            b = 0;
            break;
        case 15:
            r = 153;
            g = 76;
            b = 76;
            break;
        case 16:
            r = 127;
            g = 0;
            b = 0;
            break;
        case 17:
            r = 127;
            g = 63;
            b = 63;
            break;
        case 18:
            r = 76;
            g = 0;
            b = 0;
            break;
        case 19:
            r = 76;
            g = 38;
            b = 38;
            break;
        case 20:
            r = 255;
            g = 63;
            b = 0;
            break;
        case 21:
            r = 255;
            g = 159;
            b = 127;
            break;
        case 22:
            r = 204;
            g = 51;
            b = 0;
            break;
        case 23:
            r = 204;
            g = 127;
            b = 102;
            break;
        case 24:
            r = 153;
            g = 38;
            b = 0;
            break;
        case 25:
            r = 153;
            g = 95;
            b = 76;
            break;
        case 26:
            r = 127;
            g = 31;
            b = 0;
            break;
        case 27:
            r = 127;
            g = 79;
            b = 63;
            break;
        case 28:
            r = 76;
            g = 19;
            b = 0;
            break;
        case 29:
            r = 76;
            g = 47;
            b = 38;
            break;
        case 30:
            r = 255;
            g = 127;
            b = 0;
            break;
        case 31:
            r = 255;
            g = 191;
            b = 127;
            break;
        case 32:
            r = 204;
            g = 102;
            b = 0;
            break;
        case 33:
            r = 204;
            g = 153;
            b = 102;
            break;
        case 34:
            r = 153;
            g = 76;
            b = 0;
            break;
        case 35:
            r = 153;
            g = 114;
            b = 76;
            break;
        case 36:
            r = 127;
            g = 63;
            b = 0;
            break;
        case 37:
            r = 127;
            g = 95;
            b = 63;
            break;
        case 38:
            r = 76;
            g = 38;
            b = 0;
            break;
        case 39:
            r = 76;
            g = 57;
            b = 38;
            break;
        case 40:
            r = 255;
            g = 191;
            b = 0;
            break;
        case 41:
            r = 255;
            g = 223;
            b = 127;
            break;
        case 42:
            r = 204;
            g = 153;
            b = 0;
            break;
        case 43:
            r = 204;
            g = 178;
            b = 102;
            break;
        case 44:
            r = 153;
            g = 114;
            b = 0;
            break;
        case 45:
            r = 153;
            g = 133;
            b = 76;
            break;
        case 46:
            r = 127;
            g = 95;
            b = 0;
            break;
        case 47:
            r = 127;
            g = 111;
            b = 63;
            break;
        case 48:
            r = 76;
            g = 57;
            b = 0;
            break;
        case 49:
            r = 76;
            g = 66;
            b = 38;
            break;
        case 50:
            r = 255;
            g = 255;
            b = 0;
            break;
        case 51:
            r = 255;
            g = 255;
            b = 127;
            break;
        case 52:
            r = 204;
            g = 204;
            b = 0;
            break;
        case 53:
            r = 204;
            g = 204;
            b = 102;
            break;
        case 54:
            r = 153;
            g = 153;
            b = 0;
            break;
        case 55:
            r = 153;
            g = 153;
            b = 76;
            break;
        case 56:
            r = 127;
            g = 127;
            b = 0;
            break;
        case 57:
            r = 127;
            g = 127;
            b = 63;
            break;
        case 58:
            r = 76;
            g = 76;
            b = 0;
            break;
        case 59:
            r = 76;
            g = 76;
            b = 38;
            break;
        case 60:
            r = 191;
            g = 255;
            b = 0;
            break;
        case 61:
            r = 223;
            g = 255;
            b = 127;
            break;
        case 62:
            r = 153;
            g = 204;
            b = 0;
            break;
        case 63:
            r = 178;
            g = 204;
            b = 102;
            break;
        case 64:
            r = 114;
            g = 153;
            b = 0;
            break;
        case 65:
            r = 133;
            g = 153;
            b = 76;
            break;
        case 66:
            r = 95;
            g = 127;
            b = 0;
            break;
        case 67:
            r = 111;
            g = 127;
            b = 63;
            break;
        case 68:
            r = 57;
            g = 76;
            b = 0;
            break;
        case 69:
            r = 66;
            g = 76;
            b = 38;
            break;
        case 70:
            r = 127;
            g = 255;
            b = 0;
            break;
        case 71:
            r = 191;
            g = 255;
            b = 127;
            break;
        case 72:
            r = 102;
            g = 204;
            b = 0;
            break;
        case 73:
            r = 153;
            g = 204;
            b = 102;
            break;
        case 74:
            r = 76;
            g = 153;
            b = 0;
            break;
        case 75:
            r = 114;
            g = 153;
            b = 76;
            break;
        case 76:
            r = 63;
            g = 127;
            b = 0;
            break;
        case 77:
            r = 95;
            g = 127;
            b = 63;
            break;
        case 78:
            r = 38;
            g = 76;
            b = 0;
            break;
        case 79:
            r = 57;
            g = 76;
            b = 38;
            break;
        case 80:
            r = 63;
            g = 255;
            b = 0;
            break;
        case 81:
            r = 159;
            g = 255;
            b = 127;
            break;
        case 82:
            r = 51;
            g = 204;
            b = 0;
            break;
        case 83:
            r = 127;
            g = 204;
            b = 102;
            break;
        case 84:
            r = 38;
            g = 153;
            b = 0;
            break;
        case 85:
            r = 95;
            g = 153;
            b = 76;
            break;
        case 86:
            r = 31;
            g = 127;
            b = 0;
            break;
        case 87:
            r = 79;
            g = 127;
            b = 63;
            break;
        case 88:
            r = 19;
            g = 76;
            b = 0;
            break;
        case 89:
            r = 47;
            g = 76;
            b = 38;
            break;
        case 90:
            r = 0;
            g = 255;
            b = 0;
            break;
        case 91:
            r = 127;
            g = 255;
            b = 127;
            break;
        case 92:
            r = 0;
            g = 204;
            b = 0;
            break;
        case 93:
            r = 102;
            g = 204;
            b = 102;
            break;
        case 94:
            r = 0;
            g = 153;
            b = 0;
            break;
        case 95:
            r = 76;
            g = 153;
            b = 76;
            break;
        case 96:
            r = 0;
            g = 127;
            b = 0;
            break;
        case 97:
            r = 63;
            g = 127;
            b = 63;
            break;
        case 98:
            r = 0;
            g = 76;
            b = 0;
            break;
        case 99:
            r = 38;
            g = 76;
            b = 38;
            break;
        case 100:
            r = 0;
            g = 255;
            b = 63;
            break;
        case 101:
            r = 127;
            g = 255;
            b = 159;
            break;
        case 102:
            r = 0;
            g = 204;
            b = 51;
            break;
        case 103:
            r = 102;
            g = 204;
            b = 127;
            break;
        case 104:
            r = 0;
            g = 153;
            b = 38;
            break;
        case 105:
            r = 76;
            g = 153;
            b = 95;
            break;
        case 106:
            r = 0;
            g = 127;
            b = 31;
            break;
        case 107:
            r = 63;
            g = 127;
            b = 79;
            break;
        case 108:
            r = 0;
            g = 76;
            b = 19;
            break;
        case 109:
            r = 38;
            g = 76;
            b = 47;
            break;
        case 110:
            r = 0;
            g = 255;
            b = 127;
            break;
        case 111:
            r = 127;
            g = 255;
            b = 191;
            break;
        case 112:
            r = 0;
            g = 204;
            b = 102;
            break;
        case 113:
            r = 102;
            g = 204;
            b = 153;
            break;
        case 114:
            r = 0;
            g = 153;
            b = 76;
            break;
        case 115:
            r = 76;
            g = 153;
            b = 114;
            break;
        case 116:
            r = 0;
            g = 127;
            b = 63;
            break;
        case 117:
            r = 63;
            g = 127;
            b = 95;
            break;
        case 118:
            r = 0;
            g = 76;
            b = 38;
            break;
        case 119:
            r = 38;
            g = 76;
            b = 57;
            break;
        case 120:
            r = 0;
            g = 255;
            b = 191;
            break;
        case 121:
            r = 127;
            g = 255;
            b = 223;
            break;
        case 122:
            r = 0;
            g = 204;
            b = 153;
            break;
        case 123:
            r = 102;
            g = 204;
            b = 178;
            break;
        case 124:
            r = 0;
            g = 153;
            b = 114;
            break;
        case 125:
            r = 76;
            g = 153;
            b = 133;
            break;
        case 126:
            r = 0;
            g = 127;
            b = 95;
            break;
        case 127:
            r = 63;
            g = 127;
            b = 111;
            break;
        case 128:
            r = 0;
            g = 76;
            b = 57;
            break;
        case 129:
            r = 38;
            g = 76;
            b = 66;
            break;
        case 130:
            r = 0;
            g = 255;
            b = 255;
            break;
        case 131:
            r = 127;
            g = 255;
            b = 255;
            break;
        case 132:
            r = 0;
            g = 204;
            b = 204;
            break;
        case 133:
            r = 102;
            g = 204;
            b = 204;
            break;
        case 134:
            r = 0;
            g = 153;
            b = 153;
            break;
        case 135:
            r = 76;
            g = 153;
            b = 153;
            break;
        case 136:
            r = 0;
            g = 127;
            b = 127;
            break;
        case 137:
            r = 63;
            g = 127;
            b = 127;
            break;
        case 138:
            r = 0;
            g = 76;
            b = 76;
            break;
        case 139:
            r = 38;
            g = 76;
            b = 76;
            break;
        case 140:
            r = 0;
            g = 191;
            b = 255;
            break;
        case 141:
            r = 127;
            g = 223;
            b = 255;
            break;
        case 142:
            r = 0;
            g = 153;
            b = 204;
            break;
        case 143:
            r = 102;
            g = 178;
            b = 204;
            break;
        case 144:
            r = 0;
            g = 114;
            b = 153;
            break;
        case 145:
            r = 76;
            g = 133;
            b = 153;
            break;
        case 146:
            r = 0;
            g = 95;
            b = 127;
            break;
        case 147:
            r = 63;
            g = 111;
            b = 127;
            break;
        case 148:
            r = 0;
            g = 57;
            b = 76;
            break;
        case 149:
            r = 38;
            g = 66;
            b = 76;
            break;
        case 150:
            r = 0;
            g = 127;
            b = 255;
            break;
        case 151:
            r = 127;
            g = 191;
            b = 255;
            break;
        case 152:
            r = 0;
            g = 102;
            b = 204;
            break;
        case 153:
            r = 102;
            g = 153;
            b = 204;
            break;
        case 154:
            r = 0;
            g = 76;
            b = 153;
            break;
        case 155:
            r = 76;
            g = 114;
            b = 153;
            break;
        case 156:
            r = 0;
            g = 63;
            b = 127;
            break;
        case 157:
            r = 63;
            g = 95;
            b = 127;
            break;
        case 158:
            r = 0;
            g = 38;
            b = 76;
            break;
        case 159:
            r = 38;
            g = 57;
            b = 76;
            break;
        case 160:
            r = 0;
            g = 63;
            b = 255;
            break;
        case 161:
            r = 127;
            g = 159;
            b = 255;
            break;
        case 162:
            r = 0;
            g = 51;
            b = 204;
            break;
        case 163:
            r = 102;
            g = 127;
            b = 204;
            break;
        case 164:
            r = 0;
            g = 38;
            b = 153;
            break;
        case 165:
            r = 76;
            g = 95;
            b = 153;
            break;
        case 166:
            r = 0;
            g = 31;
            b = 127;
            break;
        case 167:
            r = 63;
            g = 79;
            b = 127;
            break;
        case 168:
            r = 0;
            g = 19;
            b = 76;
            break;
        case 169:
            r = 38;
            g = 47;
            b = 76;
            break;
        case 170:
            r = 0;
            g = 0;
            b = 255;
            break;
        case 171:
            r = 127;
            g = 127;
            b = 255;
            break;
        case 172:
            r = 0;
            g = 0;
            b = 204;
            break;
        case 173:
            r = 102;
            g = 102;
            b = 204;
            break;
        case 174:
            r = 0;
            g = 0;
            b = 153;
            break;
        case 175:
            r = 76;
            g = 76;
            b = 153;
            break;
        case 176:
            r = 0;
            g = 0;
            b = 127;
            break;
        case 177:
            r = 63;
            g = 63;
            b = 127;
            break;
        case 178:
            r = 0;
            g = 0;
            b = 76;
            break;
        case 179:
            r = 38;
            g = 38;
            b = 76;
            break;
        case 180:
            r = 63;
            g = 0;
            b = 255;
            break;
        case 181:
            r = 159;
            g = 127;
            b = 255;
            break;
        case 182:
            r = 51;
            g = 0;
            b = 204;
            break;
        case 183:
            r = 127;
            g = 102;
            b = 204;
            break;
        case 184:
            r = 38;
            g = 0;
            b = 153;
            break;
        case 185:
            r = 95;
            g = 76;
            b = 153;
            break;
        case 186:
            r = 31;
            g = 0;
            b = 127;
            break;
        case 187:
            r = 79;
            g = 63;
            b = 127;
            break;
        case 188:
            r = 19;
            g = 0;
            b = 76;
            break;
        case 189:
            r = 47;
            g = 38;
            b = 76;
            break;
        case 190:
            r = 127;
            g = 0;
            b = 255;
            break;
        case 191:
            r = 191;
            g = 127;
            b = 255;
            break;
        case 192:
            r = 102;
            g = 0;
            b = 204;
            break;
        case 193:
            r = 153;
            g = 102;
            b = 204;
            break;
        case 194:
            r = 76;
            g = 0;
            b = 153;
            break;
        case 195:
            r = 114;
            g = 76;
            b = 153;
            break;
        case 196:
            r = 63;
            g = 0;
            b = 127;
            break;
        case 197:
            r = 95;
            g = 63;
            b = 127;
            break;
        case 198:
            r = 38;
            g = 0;
            b = 76;
            break;
        case 199:
            r = 57;
            g = 38;
            b = 76;
            break;
        case 200:
            r = 191;
            g = 0;
            b = 255;
            break;
        case 201:
            r = 223;
            g = 127;
            b = 255;
            break;
        case 202:
            r = 153;
            g = 0;
            b = 204;
            break;
        case 203:
            r = 178;
            g = 102;
            b = 204;
            break;
        case 204:
            r = 114;
            g = 0;
            b = 153;
            break;
        case 205:
            r = 133;
            g = 76;
            b = 153;
            break;
        case 206:
            r = 95;
            g = 0;
            b = 127;
            break;
        case 207:
            r = 111;
            g = 63;
            b = 127;
            break;
        case 208:
            r = 57;
            g = 0;
            b = 76;
            break;
        case 209:
            r = 66;
            g = 38;
            b = 76;
            break;
        case 210:
            r = 255;
            g = 0;
            b = 255;
            break;
        case 211:
            r = 255;
            g = 127;
            b = 255;
            break;
        case 212:
            r = 204;
            g = 0;
            b = 204;
            break;
        case 213:
            r = 204;
            g = 102;
            b = 204;
            break;
        case 214:
            r = 153;
            g = 0;
            b = 153;
            break;
        case 215:
            r = 153;
            g = 76;
            b = 153;
            break;
        case 216:
            r = 127;
            g = 0;
            b = 127;
            break;
        case 217:
            r = 127;
            g = 63;
            b = 127;
            break;
        case 218:
            r = 76;
            g = 0;
            b = 76;
            break;
        case 219:
            r = 76;
            g = 38;
            b = 76;
            break;
        case 220:
            r = 255;
            g = 0;
            b = 191;
            break;
        case 221:
            r = 255;
            g = 127;
            b = 223;
            break;
        case 222:
            r = 204;
            g = 0;
            b = 153;
            break;
        case 223:
            r = 204;
            g = 102;
            b = 178;
            break;
        case 224:
            r = 153;
            g = 0;
            b = 114;
            break;
        case 225:
            r = 153;
            g = 76;
            b = 133;
            break;
        case 226:
            r = 127;
            g = 0;
            b = 95;
            break;
        case 227:
            r = 127;
            g = 63;
            b = 111;
            break;
        case 228:
            r = 76;
            g = 0;
            b = 57;
            break;
        case 229:
            r = 76;
            g = 38;
            b = 66;
            break;
        case 230:
            r = 255;
            g = 0;
            b = 127;
            break;
        case 231:
            r = 255;
            g = 127;
            b = 191;
            break;
        case 232:
            r = 204;
            g = 0;
            b = 102;
            break;
        case 233:
            r = 204;
            g = 102;
            b = 153;
            break;
        case 234:
            r = 153;
            g = 0;
            b = 76;
            break;
        case 235:
            r = 153;
            g = 76;
            b = 114;
            break;
        case 236:
            r = 127;
            g = 0;
            b = 63;
            break;
        case 237:
            r = 127;
            g = 63;
            b = 95;
            break;
        case 238:
            r = 76;
            g = 0;
            b = 38;
            break;
        case 239:
            r = 76;
            g = 38;
            b = 57;
            break;
        case 240:
            r = 255;
            g = 0;
            b = 63;
            break;
        case 241:
            r = 255;
            g = 127;
            b = 159;
            break;
        case 242:
            r = 204;
            g = 0;
            b = 51;
            break;
        case 243:
            r = 204;
            g = 102;
            b = 127;
            break;
        case 244:
            r = 153;
            g = 0;
            b = 38;
            break;
        case 245:
            r = 153;
            g = 76;
            b = 95;
            break;
        case 246:
            r = 127;
            g = 0;
            b = 31;
            break;
        case 247:
            r = 127;
            g = 63;
            b = 79;
            break;
        case 248:
            r = 76;
            g = 0;
            b = 19;
            break;
        case 249:
            r = 76;
            g = 38;
            b = 47;
            break;
        case 250:
            r = 51;
            g = 51;
            b = 51;
            break;
        case 251:
            r = 91;
            g = 91;
            b = 91;
            break;
        case 252:
            r = 132;
            g = 132;
            b = 132;
            break;
        case 253:
            r = 173;
            g = 173;
            b = 173;
            break;
        case 254:
            r = 214;
            g = 214;
            b = 214;
            break;
        case 255:
            r = 255;
            g = 255;
            b = 255;
            break;
        }

        _r = r;
        _g = g;
        _b = b;
    }
    static int rgbToColorIndex(int r, int g, int b)
    {

        int colorIndex = 0;
        if (r == 255 && g == 0 && b == 0)
        {
            colorIndex = 1;
        }
        else if (r == 255 && g == 255 && b == 0)
        {
            colorIndex = 2;
        }
        else if (r == 0 && g == 255 && b == 0)
        {
            colorIndex = 3;
        }
        else if (r == 0 && g == 255 && b == 255)
        {
            colorIndex = 4;
        }
        else if (r == 0 && g == 0 && b == 255)
        {
            colorIndex = 5;
        }
        else if (r == 255 && g == 0 && b == 255)
        {
            colorIndex = 6;
        }
        else if (r == 255 && g == 255 && b == 255)
        {
            colorIndex = 7;
        }
        else if (r == 128 && g == 128 && b == 128)
        {
            colorIndex = 8;
        }
        else if (r == 192 && g == 192 && b == 192)
        {
            colorIndex = 9;
        }
        else if (r == 255 && g == 0 && b == 0)
        {
            colorIndex = 10;
        }
        else if (r == 255 && g == 127 && b == 127)
        {
            colorIndex = 11;
        }
        else if (r == 204 && g == 0 && b == 0)
        {
            colorIndex = 12;
        }
        else if (r == 204 && g == 102 && b == 102)
        {
            colorIndex = 13;
        }
        else if (r == 153 && g == 0 && b == 0)
        {
            colorIndex = 14;
        }
        else if (r == 153 && g == 76 && b == 76)
        {
            colorIndex = 15;
        }
        else if (r == 127 && g == 0 && b == 0)
        {
            colorIndex = 16;
        }
        else if (r == 127 && g == 63 && b == 63)
        {
            colorIndex = 17;
        }
        else if (r == 76 && g == 0 && b == 0)
        {
            colorIndex = 18;
        }
        else if (r == 76 && g == 38 && b == 38)
        {
            colorIndex = 19;
        }
        else if (r == 255 && g == 63 && b == 0)
        {
            colorIndex = 20;
        }
        else if (r == 255 && g == 159 && b == 127)
        {
            colorIndex = 21;
        }
        else if (r == 204 && g == 51 && b == 0)
        {
            colorIndex = 22;
        }
        else if (r == 204 && g == 127 && b == 102)
        {
            colorIndex = 23;
        }
        else if (r == 153 && g == 38 && b == 0)
        {
            colorIndex = 24;
        }
        else if (r == 153 && g == 95 && b == 76)
        {
            colorIndex = 25;
        }
        else if (r == 127 && g == 31 && b == 0)
        {
            colorIndex = 26;
        }
        else if (r == 127 && g == 79 && b == 63)
        {
            colorIndex = 27;
        }
        else if (r == 76 && g == 19 && b == 0)
        {
            colorIndex = 28;
        }
        else if (r == 76 && g == 47 && b == 38)
        {
            colorIndex = 29;
        }
        else if (r == 255 && g == 127 && b == 0)
        {
            colorIndex = 30;
        }
        else if (r == 255 && g == 191 && b == 127)
        {
            colorIndex = 31;
        }
        else if (r == 204 && g == 102 && b == 0)
        {
            colorIndex = 32;
        }
        else if (r == 204 && g == 153 && b == 102)
        {
            colorIndex = 33;
        }
        else if (r == 153 && g == 76 && b == 0)
        {
            colorIndex = 34;
        }
        else if (r == 153 && g == 114 && b == 76)
        {
            colorIndex = 35;
        }
        else if (r == 127 && g == 63 && b == 0)
        {
            colorIndex = 36;
        }
        else if (r == 127 && g == 95 && b == 63)
        {
            colorIndex = 37;
        }
        else if (r == 76 && g == 38 && b == 0)
        {
            colorIndex = 38;
        }
        else if (r == 76 && g == 57 && b == 38)
        {
            colorIndex = 39;
        }
        else if (r == 255 && g == 191 && b == 0)
        {
            colorIndex = 40;
        }
        else if (r == 255 && g == 223 && b == 127)
        {
            colorIndex = 41;
        }
        else if (r == 204 && g == 153 && b == 0)
        {
            colorIndex = 42;
        }
        else if (r == 204 && g == 178 && b == 102)
        {
            colorIndex = 43;
        }
        else if (r == 153 && g == 114 && b == 0)
        {
            colorIndex = 44;
        }
        else if (r == 153 && g == 133 && b == 76)
        {
            colorIndex = 45;
        }
        else if (r == 127 && g == 95 && b == 0)
        {
            colorIndex = 46;
        }
        else if (r == 127 && g == 111 && b == 63)
        {
            colorIndex = 47;
        }
        else if (r == 76 && g == 57 && b == 0)
        {
            colorIndex = 48;
        }
        else if (r == 76 && g == 66 && b == 38)
        {
            colorIndex = 49;
        }
        else if (r == 255 && g == 255 && b == 0)
        {
            colorIndex = 50;
        }
        else if (r == 255 && g == 255 && b == 127)
        {
            colorIndex = 51;
        }
        else if (r == 204 && g == 204 && b == 0)
        {
            colorIndex = 52;
        }
        else if (r == 204 && g == 204 && b == 102)
        {
            colorIndex = 53;
        }
        else if (r == 153 && g == 153 && b == 0)
        {
            colorIndex = 54;
        }
        else if (r == 153 && g == 153 && b == 76)
        {
            colorIndex = 55;
        }
        else if (r == 127 && g == 127 && b == 0)
        {
            colorIndex = 56;
        }
        else if (r == 127 && g == 127 && b == 63)
        {
            colorIndex = 57;
        }
        else if (r == 76 && g == 76 && b == 0)
        {
            colorIndex = 58;
        }
        else if (r == 76 && g == 76 && b == 38)
        {
            colorIndex = 59;
        }
        else if (r == 191 && g == 255 && b == 0)
        {
            colorIndex = 60;
        }
        else if (r == 223 && g == 255 && b == 127)
        {
            colorIndex = 61;
        }
        else if (r == 153 && g == 204 && b == 0)
        {
            colorIndex = 62;
        }
        else if (r == 178 && g == 204 && b == 102)
        {
            colorIndex = 63;
        }
        else if (r == 114 && g == 153 && b == 0)
        {
            colorIndex = 64;
        }
        else if (r == 133 && g == 153 && b == 76)
        {
            colorIndex = 65;
        }
        else if (r == 95 && g == 127 && b == 0)
        {
            colorIndex = 66;
        }
        else if (r == 111 && g == 127 && b == 63)
        {
            colorIndex = 67;
        }
        else if (r == 57 && g == 76 && b == 0)
        {
            colorIndex = 68;
        }
        else if (r == 66 && g == 76 && b == 38)
        {
            colorIndex = 69;
        }
        else if (r == 127 && g == 255 && b == 0)
        {
            colorIndex = 70;
        }
        else if (r == 191 && g == 255 && b == 127)
        {
            colorIndex = 71;
        }
        else if (r == 102 && g == 204 && b == 0)
        {
            colorIndex = 72;
        }
        else if (r == 153 && g == 204 && b == 102)
        {
            colorIndex = 73;
        }
        else if (r == 76 && g == 153 && b == 0)
        {
            colorIndex = 74;
        }
        else if (r == 114 && g == 153 && b == 76)
        {
            colorIndex = 75;
        }
        else if (r == 63 && g == 127 && b == 0)
        {
            colorIndex = 76;
        }
        else if (r == 95 && g == 127 && b == 63)
        {
            colorIndex = 77;
        }
        else if (r == 38 && g == 76 && b == 0)
        {
            colorIndex = 78;
        }
        else if (r == 57 && g == 76 && b == 38)
        {
            colorIndex = 79;
        }
        else if (r == 63 && g == 255 && b == 0)
        {
            colorIndex = 80;
        }
        else if (r == 159 && g == 255 && b == 127)
        {
            colorIndex = 81;
        }
        else if (r == 51 && g == 204 && b == 0)
        {
            colorIndex = 82;
        }
        else if (r == 127 && g == 204 && b == 102)
        {
            colorIndex = 83;
        }
        else if (r == 38 && g == 153 && b == 0)
        {
            colorIndex = 84;
        }
        else if (r == 95 && g == 153 && b == 76)
        {
            colorIndex = 85;
        }
        else if (r == 31 && g == 127 && b == 0)
        {
            colorIndex = 86;
        }
        else if (r == 79 && g == 127 && b == 63)
        {
            colorIndex = 87;
        }
        else if (r == 19 && g == 76 && b == 0)
        {
            colorIndex = 88;
        }
        else if (r == 47 && g == 76 && b == 38)
        {
            colorIndex = 89;
        }
        else if (r == 0 && g == 255 && b == 0)
        {
            colorIndex = 90;
        }
        else if (r == 127 && g == 255 && b == 127)
        {
            colorIndex = 91;
        }
        else if (r == 0 && g == 204 && b == 0)
        {
            colorIndex = 92;
        }
        else if (r == 102 && g == 204 && b == 102)
        {
            colorIndex = 93;
        }
        else if (r == 0 && g == 153 && b == 0)
        {
            colorIndex = 94;
        }
        else if (r == 76 && g == 153 && b == 76)
        {
            colorIndex = 95;
        }
        else if (r == 0 && g == 127 && b == 0)
        {
            colorIndex = 96;
        }
        else if (r == 63 && g == 127 && b == 63)
        {
            colorIndex = 97;
        }
        else if (r == 0 && g == 76 && b == 0)
        {
            colorIndex = 98;
        }
        else if (r == 38 && g == 76 && b == 38)
        {
            colorIndex = 99;
        }
        if (r == 0 && g == 255 && b == 63)
        {
            colorIndex = 100;
        }
        else if (r == 127 && g == 255 && b == 159)
        {
            colorIndex = 101;
        }
        else if (r == 0 && g == 204 && b == 51)
        {
            colorIndex = 102;
        }
        else if (r == 102 && g == 204 && b == 127)
        {
            colorIndex = 103;
        }
        else if (r == 0 && g == 153 && b == 38)
        {
            colorIndex = 104;
        }
        else if (r == 76 && g == 153 && b == 95)
        {
            colorIndex = 105;
        }
        else if (r == 0 && g == 127 && b == 31)
        {
            colorIndex = 106;
        }
        else if (r == 63 && g == 127 && b == 79)
        {
            colorIndex = 107;
        }
        else if (r == 0 && g == 76 && b == 19)
        {
            colorIndex = 108;
        }
        else if (r == 38 && g == 76 && b == 47)
        {
            colorIndex = 109;
        }
        else if (r == 0 && g == 255 && b == 127)
        {
            colorIndex = 110;
        }
        else if (r == 127 && g == 255 && b == 191)
        {
            colorIndex = 111;
        }
        else if (r == 0 && g == 204 && b == 102)
        {
            colorIndex = 112;
        }
        else if (r == 102 && g == 204 && b == 153)
        {
            colorIndex = 113;
        }
        else if (r == 0 && g == 153 && b == 76)
        {
            colorIndex = 114;
        }
        else if (r == 76 && g == 153 && b == 114)
        {
            colorIndex = 115;
        }
        else if (r == 0 && g == 127 && b == 63)
        {
            colorIndex = 116;
        }
        else if (r == 63 && g == 127 && b == 95)
        {
            colorIndex = 117;
        }
        else if (r == 0 && g == 76 && b == 38)
        {
            colorIndex = 118;
        }
        else if (r == 38 && g == 76 && b == 57)
        {
            colorIndex = 119;
        }
        else if (r == 0 && g == 255 && b == 191)
        {
            colorIndex = 120;
        }
        else if (r == 127 && g == 255 && b == 223)
        {
            colorIndex = 121;
        }
        else if (r == 0 && g == 204 && b == 153)
        {
            colorIndex = 122;
        }
        else if (r == 102 && g == 204 && b == 178)
        {
            colorIndex = 123;
        }
        else if (r == 0 && g == 153 && b == 114)
        {
            colorIndex = 124;
        }
        else if (r == 76 && g == 153 && b == 133)
        {
            colorIndex = 125;
        }
        else if (r == 0 && g == 127 && b == 95)
        {
            colorIndex = 126;
        }
        else if (r == 63 && g == 127 && b == 111)
        {
            colorIndex = 127;
        }
        else if (r == 0 && g == 76 && b == 57)
        {
            colorIndex = 128;
        }
        else if (r == 38 && g == 76 && b == 66)
        {
            colorIndex = 129;
        }
        else if (r == 0 && g == 255 && b == 255)
        {
            colorIndex = 130;
        }
        else if (r == 127 && g == 255 && b == 255)
        {
            colorIndex = 131;
        }
        else if (r == 0 && g == 204 && b == 204)
        {
            colorIndex = 132;
        }
        else if (r == 102 && g == 204 && b == 204)
        {
            colorIndex = 133;
        }
        else if (r == 0 && g == 153 && b == 153)
        {
            colorIndex = 134;
        }
        else if (r == 76 && g == 153 && b == 153)
        {
            colorIndex = 135;
        }
        else if (r == 0 && g == 127 && b == 127)
        {
            colorIndex = 136;
        }
        else if (r == 63 && g == 127 && b == 127)
        {
            colorIndex = 137;
        }
        else if (r == 0 && g == 76 && b == 76)
        {
            colorIndex = 138;
        }
        else if (r == 38 && g == 76 && b == 76)
        {
            colorIndex = 139;
        }
        else if (r == 0 && g == 191 && b == 255)
        {
            colorIndex = 140;
        }
        else if (r == 127 && g == 223 && b == 255)
        {
            colorIndex = 141;
        }
        else if (r == 0 && g == 153 && b == 204)
        {
            colorIndex = 142;
        }
        else if (r == 102 && g == 178 && b == 204)
        {
            colorIndex = 143;
        }
        else if (r == 0 && g == 114 && b == 153)
        {
            colorIndex = 144;
        }
        else if (r == 76 && g == 133 && b == 153)
        {
            colorIndex = 145;
        }
        else if (r == 0 && g == 95 && b == 127)
        {
            colorIndex = 146;
        }
        else if (r == 63 && g == 111 && b == 127)
        {
            colorIndex = 147;
        }
        else if (r == 0 && g == 57 && b == 76)
        {
            colorIndex = 148;
        }
        else if (r == 38 && g == 66 && b == 76)
        {
            colorIndex = 149;
        }
        else if (r == 0 && g == 127 && b == 255)
        {
            colorIndex = 150;
        }
        else if (r == 127 && g == 191 && b == 255)
        {
            colorIndex = 151;
        }
        else if (r == 0 && g == 102 && b == 204)
        {
            colorIndex = 152;
        }
        else if (r == 102 && g == 153 && b == 204)
        {
            colorIndex = 153;
        }
        else if (r == 0 && g == 76 && b == 153)
        {
            colorIndex = 154;
        }
        else if (r == 76 && g == 114 && b == 153)
        {
            colorIndex = 155;
        }
        else if (r == 0 && g == 63 && b == 127)
        {
            colorIndex = 156;
        }
        else if (r == 63 && g == 95 && b == 127)
        {
            colorIndex = 157;
        }
        else if (r == 0 && g == 38 && b == 76)
        {
            colorIndex = 158;
        }
        else if (r == 38 && g == 57 && b == 76)
        {
            colorIndex = 159;
        }
        else if (r == 0 && g == 63 && b == 255)
        {
            colorIndex = 160;
        }
        else if (r == 127 && g == 159 && b == 255)
        {
            colorIndex = 161;
        }
        else if (r == 0 && g == 51 && b == 204)
        {
            colorIndex = 162;
        }
        else if (r == 102 && g == 127 && b == 204)
        {
            colorIndex = 163;
        }
        else if (r == 0 && g == 38 && b == 153)
        {
            colorIndex = 164;
        }
        else if (r == 76 && g == 95 && b == 153)
        {
            colorIndex = 165;
        }
        else if (r == 0 && g == 31 && b == 127)
        {
            colorIndex = 166;
        }
        else if (r == 63 && g == 79 && b == 127)
        {
            colorIndex = 167;
        }
        else if (r == 0 && g == 19 && b == 76)
        {
            colorIndex = 168;
        }
        else if (r == 38 && g == 47 && b == 76)
        {
            colorIndex = 169;
        }
        else if (r == 0 && g == 0 && b == 255)
        {
            colorIndex = 170;
        }
        else if (r == 127 && g == 127 && b == 255)
        {
            colorIndex = 171;
        }
        else if (r == 0 && g == 0 && b == 204)
        {
            colorIndex = 172;
        }
        else if (r == 102 && g == 102 && b == 204)
        {
            colorIndex = 173;
        }
        else if (r == 0 && g == 0 && b == 153)
        {
            colorIndex = 174;
        }
        else if (r == 76 && g == 76 && b == 153)
        {
            colorIndex = 175;
        }
        else if (r == 0 && g == 0 && b == 127)
        {
            colorIndex = 176;
        }
        else if (r == 63 && g == 63 && b == 127)
        {
            colorIndex = 177;
        }
        else if (r == 0 && g == 0 && b == 76)
        {
            colorIndex = 178;
        }
        else if (r == 38 && g == 38 && b == 76)
        {
            colorIndex = 179;
        }
        else if (r == 63 && g == 0 && b == 255)
        {
            colorIndex = 180;
        }
        else if (r == 159 && g == 127 && b == 255)
        {
            colorIndex = 181;
        }
        else if (r == 51 && g == 0 && b == 204)
        {
            colorIndex = 182;
        }
        else if (r == 127 && g == 102 && b == 204)
        {
            colorIndex = 183;
        }
        else if (r == 38 && g == 0 && b == 153)
        {
            colorIndex = 184;
        }
        else if (r == 95 && g == 76 && b == 153)
        {
            colorIndex = 185;
        }
        else if (r == 31 && g == 0 && b == 127)
        {
            colorIndex = 186;
        }
        else if (r == 79 && g == 63 && b == 127)
        {
            colorIndex = 187;
        }
        else if (r == 19 && g == 0 && b == 76)
        {
            colorIndex = 188;
        }
        else if (r == 47 && g == 38 && b == 76)
        {
            colorIndex = 189;
        }
        else if (r == 127 && g == 0 && b == 255)
        {
            colorIndex = 190;
        }
        else if (r == 191 && g == 127 && b == 255)
        {
            colorIndex = 191;
        }
        else if (r == 102 && g == 0 && b == 204)
        {
            colorIndex = 192;
        }
        else if (r == 153 && g == 102 && b == 204)
        {
            colorIndex = 193;
        }
        else if (r == 76 && g == 0 && b == 153)
        {
            colorIndex = 194;
        }
        else if (r == 114 && g == 76 && b == 153)
        {
            colorIndex = 195;
        }
        else if (r == 63 && g == 0 && b == 127)
        {
            colorIndex = 196;
        }
        else if (r == 95 && g == 63 && b == 127)
        {
            colorIndex = 197;
        }
        else if (r == 38 && g == 0 && b == 76)
        {
            colorIndex = 198;
        }
        else if (r == 57 && g == 38 && b == 76)
        {
            colorIndex = 199;
        }
        else if (r == 191 && g == 0 && b == 255)
        {
            colorIndex = 200;
        }
        if (r == 223 && g == 127 && b == 255)
        {
            colorIndex = 201;
        }
        else if (r == 153 && g == 0 && b == 204)
        {
            colorIndex = 202;
        }
        else if (r == 178 && g == 102 && b == 204)
        {
            colorIndex = 203;
        }
        else if (r == 114 && g == 0 && b == 153)
        {
            colorIndex = 204;
        }
        else if (r == 133 && g == 76 && b == 153)
        {
            colorIndex = 205;
        }
        else if (r == 95 && g == 0 && b == 127)
        {
            colorIndex = 206;
        }
        else if (r == 111 && g == 63 && b == 127)
        {
            colorIndex = 207;
        }
        else if (r == 57 && g == 0 && b == 76)
        {
            colorIndex = 208;
        }
        else if (r == 66 && g == 38 && b == 76)
        {
            colorIndex = 209;
        }
        else if (r == 255 && g == 0 && b == 255)
        {
            colorIndex = 210;
        }
        else if (r == 255 && g == 127 && b == 255)
        {
            colorIndex = 211;
        }
        else if (r == 204 && g == 0 && b == 204)
        {
            colorIndex = 212;
        }
        else if (r == 204 && g == 102 && b == 204)
        {
            colorIndex = 213;
        }
        else if (r == 153 && g == 0 && b == 153)
        {
            colorIndex = 214;
        }
        else if (r == 153 && g == 76 && b == 153)
        {
            colorIndex = 215;
        }
        else if (r == 127 && g == 0 && b == 127)
        {
            colorIndex = 216;
        }
        else if (r == 127 && g == 63 && b == 127)
        {
            colorIndex = 217;
        }
        else if (r == 76 && g == 0 && b == 76)
        {
            colorIndex = 218;
        }
        else if (r == 76 && g == 38 && b == 76)
        {
            colorIndex = 219;
        }
        else if (r == 255 && g == 0 && b == 191)
        {
            colorIndex = 220;
        }
        else if (r == 255 && g == 127 && b == 223)
        {
            colorIndex = 221;
        }
        else if (r == 204 && g == 0 && b == 153)
        {
            colorIndex = 222;
        }
        else if (r == 204 && g == 102 && b == 178)
        {
            colorIndex = 223;
        }
        else if (r == 153 && g == 0 && b == 114)
        {
            colorIndex = 224;
        }
        else if (r == 153 && g == 76 && b == 133)
        {
            colorIndex = 225;
        }
        else if (r == 127 && g == 0 && b == 95)
        {
            colorIndex = 226;
        }
        else if (r == 127 && g == 63 && b == 111)
        {
            colorIndex = 227;
        }
        else if (r == 76 && g == 0 && b == 57)
        {
            colorIndex = 228;
        }
        else if (r == 76 && g == 38 && b == 66)
        {
            colorIndex = 229;
        }
        else if (r == 255 && g == 0 && b == 127)
        {
            colorIndex = 230;
        }
        else if (r == 255 && g == 127 && b == 191)
        {
            colorIndex = 231;
        }
        else if (r == 204 && g == 0 && b == 102)
        {
            colorIndex = 232;
        }
        else if (r == 204 && g == 102 && b == 153)
        {
            colorIndex = 233;
        }
        else if (r == 153 && g == 0 && b == 76)
        {
            colorIndex = 234;
        }
        else if (r == 153 && g == 76 && b == 114)
        {
            colorIndex = 235;
        }
        else if (r == 127 && g == 0 && b == 63)
        {
            colorIndex = 236;
        }
        else if (r == 127 && g == 63 && b == 95)
        {
            colorIndex = 237;
        }
        else if (r == 76 && g == 0 && b == 38)
        {
            colorIndex = 238;
        }
        else if (r == 76 && g == 38 && b == 57)
        {
            colorIndex = 239;
        }
        else if (r == 255 && g == 0 && b == 63)
        {
            colorIndex = 240;
        }
        else if (r == 255 && g == 127 && b == 159)
        {
            colorIndex = 241;
        }
        else if (r == 204 && g == 0 && b == 51)
        {
            colorIndex = 242;
        }
        else if (r == 204 && g == 102 && b == 127)
        {
            colorIndex = 243;
        }
        else if (r == 153 && g == 0 && b == 38)
        {
            colorIndex = 244;
        }
        else if (r == 153 && g == 76 && b == 95)
        {
            colorIndex = 245;
        }
        else if (r == 127 && g == 0 && b == 31)
        {
            colorIndex = 246;
        }
        else if (r == 127 && g == 63 && b == 79)
        {
            colorIndex = 247;
        }
        else if (r == 76 && g == 0 && b == 19)
        {
            colorIndex = 248;
        }
        else if (r == 76 && g == 38 && b == 47)
        {
            colorIndex = 249;
        }
        else if (r == 51 && g == 51 && b == 51)
        {
            colorIndex = 250;
        }
        else if (r == 91 && g == 91 && b == 91)
        {
            colorIndex = 251;
        }
        else if (r == 132 && g == 132 && b == 132)
        {
            colorIndex = 252;
        }
        else if (r == 173 && g == 173 && b == 173)
        {
            colorIndex = 253;
        }
        else if (r == 214 && g == 214 && b == 214)
        {
            colorIndex = 254;
        }
        else if (r == 255 && g == 255 && b == 255)
        {
            colorIndex = 255;
        }

        if (colorIndex == 0)
        {

            // 获得最接近
            double proximity = 10086;
            for (int i = 1; i < 256; i++)
            {

                // colorIndex转rgb
                int tempR, tempG, tempB;
                CmEntityColor::colorIndextoRGB(i, tempR, tempG, tempB);

                double distance = sqrt(((double)r - (double)tempR) * ((double)r - (double)tempR) + ((double)g - (double)tempG) * ((double)g - (double)tempG) + ((double)b - (double)tempB) * ((double)b - (double)tempB));
                if (distance < proximity)
                {
                    proximity = distance;
                    colorIndex = i;
                }
            }
        }

        return colorIndex;
    }

    em_protected : static Int16 indirect(const RGBM *pRGBM)
    {
        if (static_colorMethod(pRGBM) != kByColor)
        {
            return Int16((*pRGBM) & 0x0000FFFF);
        }
        return 0;
    }

    /*!DOM*/
    static void setIndirect(RGBM *pRGBM, Int16 indirect)
    {
        if (static_colorMethod(pRGBM) != kByColor)
        {
            *pRGBM = ((*pRGBM & 0xFF000000) | (UInt32(indirect) & 0x0000FFFF));
        }
    }

    /*!DOM*/
    Int16 indirect() const { return indirect(&m_RGBM); }

    /*!DOM*/
    void setIndirect(Int16 indirect) { setIndirect(&m_RGBM, indirect); }

    RGBM m_RGBM;
};

#endif
