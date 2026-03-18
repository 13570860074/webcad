#ifndef DBTEXTSTYLETABLERECORD_H
#define DBTEXTSTYLETABLERECORD_H


#include "Db.h"
#include "GiTextStyle.h"
#include "DbSymbolTableRecord.h"


class DbTextStyleTableRecord : public  DbSymbolTableRecord
{
public:
    DbTextStyleTableRecord();
    virtual ~DbTextStyleTableRecord();
    DB_DECLARE_MEMBERS(DbTextStyleTableRecord);


    typedef DbTextStyleTable TableType;


    Adesk::Boolean isShapeFile() const;
    void setIsShapeFile(Adesk::Boolean shape);

    Adesk::Boolean isVertical() const;
    void setIsVertical(Adesk::Boolean vertical);

    double textSize() const;
    Acad::ErrorStatus setTextSize(double size);

    double xScale() const;
    Acad::ErrorStatus setXScale(double xScale);

    double obliquingAngle() const;
    Acad::ErrorStatus setObliquingAngle(double obliquingAngle);

    Adesk::UInt8 flagBits() const;
    void setFlagBits(Adesk::UInt8 flagBits);

    double priorSize() const;
    Acad::ErrorStatus setPriorSize(double priorSize);

    Acad::ErrorStatus fileName(AcString& sFileName) const;
    Acad::ErrorStatus fileName(ACHAR*& fileName) const; // deprecated
    Acad::ErrorStatus fileName(const ACHAR*& fileName) const;
    Acad::ErrorStatus setFileName(const ACHAR* fileName);

    Acad::ErrorStatus bigFontFileName(AcString& sFileName) const;
    Acad::ErrorStatus bigFontFileName(ACHAR*& fileName) const;  // deprecated
    Acad::ErrorStatus bigFontFileName(const ACHAR*& fileName) const;
    Acad::ErrorStatus setBigFontFileName(const ACHAR* fileName);

#if EMSDK
    const emstring emskd_fileName()const;
    const emstring emskd_bigFontFileName()const;
    Acad::ErrorStatus emskd_setFileName(const emstring& fileName);
    Acad::ErrorStatus emskd_setBigFontFileName(const emstring& fileName);
#endif

    /// <summary>
    /// This method sets the TextStyleTableRecord to use the Windows font as specified by the passed in arguments. 
    /// If pTypeFace == NULL or points to an empty string, and the other arguments are Adesk::kFalse or 0 (as appropriate),
    /// then all existing Windows font information in this TextStyleTableRecord is removed from the record.
    /// WARNING: This method should not be used to set an old style SHX font. To set the TextStyleTableRecord to use
    /// an SHX font, you need to use the AcDbTextStyleTableRecord::setFileName() method. 
    /// </summary>
    /// <param name="pTypeface">Input pointer to the font file name string of the desired font</param>
    /// <param name="bold">Input Boolean indicating whether or not the font is to be bold</param>
    /// <param name="italic">Input Boolean indicating whether or not the font is to be italic</param>
    /// <param name="charset">Input integer that is the Windows character set identifier </param>
    /// <param name="pitch">Input indicating the pitch being used</param>
    /// <param name="family">Input indicating the family being used</param>
    /// <param name="bAllowMissingFont">Input bool indicating whether or not to allow the set to succeed if the font is not installed on the computer </param>
    /// <returns>Returns Acad::eOk if successful and Acad::eInvalidInput if bAllowMissingFont is false and the font
    /// is not installed on the system.</returns>
    Acad::ErrorStatus setFont(const ACHAR* pTypeface, Adesk::Boolean bold,
        Adesk::Boolean italic, Charset charset,
        FontUtils::FontPitch pitch,
        FontUtils::FontFamily family,
        bool bAllowMissingFont = false);

    /// <summary>
    /// This method returns the font definition data from the text style record. The returned values are the typeface,
    /// the bold and italics style attributes, the character set, and the pitch and family attributes. 
    ///
    /// This function allocates the memory for the typeface string and then sets pTypeface to point it.
    /// It is the caller's responsibility to free the memory that pTypeface is set to point to. 
    ///
    /// If the TextStyleTableRecord is not using a Windows font (that is, if it's using an SHX font),
    /// then pTypeface is NULL, bold and italic are Adesk::kFalse, and charset and pitchAndFamily are 0. 
    /// <summary>
    /// <param name="pTypeface">Input uninitialized pointer to char; output pointer to font file name string</param>
    /// <param name="bold">Output Boolean indicating whether or not the font is bold</param>
    /// <param name="italic">Output Boolean indicating whether or not the font is italic</param>
    /// <param name="charset">Output integer that is the Windows character set identifier</param>
    /// <param name="pitch">Output the pitch being used </param>
    /// <param name="family">Output the family being used </param>
    /// <returns>Returns Acad::eOk if successful. Returns Acad::eOutOfMemory if there is insufficient memory for pTypeFace.</returns>
    Acad::ErrorStatus font(AcString& sTypeface,
        bool& bold,
        bool& italic,
        Charset& charset,
        FontUtils::FontPitch& pitch,
        FontUtils::FontFamily& family) const;
    Acad::ErrorStatus                 font(ACHAR*& pTypeface,        // deprecated
        Adesk::Boolean& bold,
        Adesk::Boolean& italic,
        Charset& charset,
        FontUtils::FontPitch& pitch,
        FontUtils::FontFamily& family) const;
};


#endif