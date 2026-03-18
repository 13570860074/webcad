
#ifndef CMCOLORBASE_H
#define CMCOLORBASE_H



#include <vector>
#include "EmObject.h"
#include "CmEntityColor.h"


class CmColorBase INHERIT_EM_OBJECT
{
public:
    virtual ~CmColorBase() {}

    /** \details
        Returns the color method of the database color object as a value of the ColorMethod enumeration.

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

        \remarks
        Using of this method by third-party applications is neither supported nor recommended.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual CmEntityColor::ColorMethod colorMethod() const = 0;
  

    /** \details
        Sets the color method the database color object using a value of the ColorMethod enumeration.

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

        \remarks
        Using of this method by third-party applications is neither supported nor recommended.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual void  setColorMethod(CmEntityColor::ColorMethod colorMethod) = 0;


    /** \details
        Checks whether the color method is byColor for the database color object and 
        returns true if and only if the color method is set to _kByColor_, otherwise it returns false.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual bool isByColor() const = 0;
    
    /** \details
        Checks whether the color method is byLayer for the database color object and 
        returns true if and only if the color method is set to _kByLayer_ or was set to _kACIbyLayer_, 
        otherwise it returns false.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual bool isByLayer() const = 0;
  
    /** \details
        Checks whether the color method is byBlock for the database color object and 
        returns true if and only if the color method is set to _kByBlock_ or was set to _kACIbyBlock_, 
        otherwise it returns false.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual bool isByBlock() const = 0;

    /** \details
        Checks whether the color method is byACI for the database color object and 
        returns true if and only if the color method is set to _kByACI_, otherwise it returns false.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \remarks
        This method returns true for ACI values of 0 (ByBlock), 7 (ByForeground), 256 (ByLayer), and 257 (None).

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual bool isByACI() const = 0;

    /** \details
        Checks whether the color method is Foreground for the database color object and 
        returns true if and only if the color method is set to _kForeground_ or was set to _kACIforeground_, 
        otherwise it returns false.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual bool isForeground()   const = 0;

    /** \details
        Checks whether the color method is byDgnIndex for the database color object and 
        returns true if and only if the color method is set to _kByDgnIndex_, otherwise it returns false.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>
    */
    virtual bool isByDgnIndex()   const = 0;

    /** \details
        Returns the packed 32-bits integer value that stores the color method and color components
        of the database color object.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    virtual UInt32 color() const = 0;
  
    /** \details
        Sets the color method and color components for the database color object as an integer value.

        \param color [in]  A packed 32-bits integer value that specifies the color method and color components.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    virtual void setColor(UInt32 color) = 0;

    /** \details
        Sets the red, green, blue color components and the color method to byColor for the database color object.

        \param red [in]  Red component as an integer value in range 0 to 255.
        \param green [in]  Green component as an integer value in range 0 to 255.
        \param blue [in]  Blue component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual void setRGB(UInt8 red, UInt8 green, UInt8 blue) = 0;
                           
    /** \details
        Sets the red color component for the database color object.
    
        \param red [in]  Red component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual void setRed(UInt8 red) = 0;
    
    /** \details
        Sets the green color component for the database color object.
    
        \param green [in]  Green component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual void setGreen(UInt8 green) = 0;
    
    /** \details
        Sets the blue color component for the database color object.
    
        \param blue [in]  Blue component as an integer value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual void setBlue(UInt8 blue) = 0;
    
    /** \details
        Returns the red color component of the database color object as a value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual UInt8 red() const = 0;

    /** \details
        Returns the green color component of the database color object as a value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual UInt8 green() const = 0;

    /** \details
        Returns the blue color component of the database color object as a value in range 0 to 255.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_RGB.html, Color Functionality as an RGB-value>
    */
    virtual UInt8 blue() const = 0;

    /** \details
        Returns the color index (ACI) of the database color object.

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

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_index.html, Color Functionality as an ACI-value>
    */
    virtual UInt16 colorIndex() const = 0;

    /** \details
        Sets the color index (ACI) of a some color palette and sets the color method to byACI 
        for the database color object.

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

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_method.html, Methods of the Color Definition>

        <link cm_color_index.html, Color Functionality as an ACI-value>
    */
    virtual void setColorIndex(UInt16 colorIndex) = 0;

    /** \details
        Converts to the named color and sets the color name and book name for the database color object.
    
        \param colorName [in]  Color name as a string value.
        \param bookName [in]  Book name as a string value.

        \returns
        Returns true if and only if successful.
        If the book name is an empty string that the method sets the UNNAMED name. 
        If the color name is an empty string that the method ignores the specified values.
        If the book name and the color name are an empty string together that the method converts to the unnamed color.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_book.html, Color Functionality as a Book Name>

        <link cm_color_integer.html, Color Functionality as an Integer-value>
    */
    virtual bool setNames(const AcString& colorName, const AcString& bookName = "") = 0;
#if EMSDK
    virtual bool emsdk_setNames(const emstring& colorName) = 0;
    virtual bool emsdk_setNames(const emstring& colorName, const emstring& bookName) = 0;
#endif
    
    /** \details
        Returns the Color name of the database color object.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_book.html, Color Functionality as a Book Name>
    */
    virtual AcString colorName() const = 0;
#if EMSDK
    virtual emstring emsdk_colorName() const = 0;
#endif

    /** \details
        Returns the Book name of the database color object.

        \sa
        <link cm_color_sample_base.html, Example of Working with the Database Color>

        \sa
        <link cm_color_book.html, Color Functionality as a Book Name>
    */
    virtual AcString bookName() const = 0;
#if EMSDK
    virtual emstring emsdk_bookName() const = 0;
#endif

    static std::vector<int> converArgbToRgb(unsigned int argb) {
        std::vector<int> rgb;
        rgb.push_back((argb & 0xff0000) >> 16);
        rgb.push_back((argb & 0xff00) >> 8);
        rgb.push_back((argb & 0xff));
        return rgb;
    }

    static unsigned int converRgbToArgb(int r, int g, int b) {
        unsigned int color = ((0xFF << 24) | (r << 16) | (g << 8) | b);
        return color;
    }
};



#endif
