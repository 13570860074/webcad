#ifndef CMCOLOR_H
#define CMCOLOR_H

#include "AcString.h"
#include "CmColorBase.h"



class CmColor : public CmColorBase
{
public:
    CmColor() { CmEntityColor::static_setColorMethod(&this->m_RGBM, CmEntityColor::ColorMethod::kByLayer); }
    CmColor(const CmColor& color) { this->m_RGBM = color.color(); }
    CmColor(const CmColorBase& color) { this->m_RGBM = color.color(); }
    CmColor(CmEntityColor::ColorMethod color) { CmEntityColor::static_setColorMethod(&this->m_RGBM, color); }
    CmColor& operator=(const CmColor& color) { this->m_RGBM = color.color(); return *this; }
    CmColor& operator=(const CmColorBase& color) { this->m_RGBM = color.color(); return *this; }
    ~CmColor() {}


    bool operator ==(const CmColor& color) const {
        if (this->m_RGBM == color.m_RGBM)
        {
            return true;
        }
        return false;
    }
    bool operator ==(const CmColorBase& color) const
    {
        if (this->m_RGBM == color.color())
        {
            return true;
        }
        return false;
    }

    bool operator !=(const CmColor& color) const
    {
        if (this->m_RGBM != color.m_RGBM)
        {
            return true;
        }
        return false;
    }
    bool operator !=(const CmColorBase& color) const
    {
        if (this->m_RGBM != color.color())
        {
            return true;
        }
        return false;
    }
  

    virtual CmEntityColor::ColorMethod colorMethod() const
    {
        return CmEntityColor::static_colorMethod(&this->m_RGBM);
    }
    virtual void setColorMethod(CmEntityColor::ColorMethod colorMethod)
    {
        CmEntityColor::static_setColorMethod(&this->m_RGBM, colorMethod);
    }

    virtual bool isByColor() const
    {
        return CmEntityColor::static_isByColor(&this->m_RGBM);
    }
    virtual bool isByLayer() const
    {
        return CmEntityColor::static_isByLayer(&this->m_RGBM);
    }
    virtual bool isByBlock() const
    {
        return CmEntityColor::static_isByBlock(&this->m_RGBM);
    }
    virtual bool isByACI() const
    {
        return CmEntityColor::static_isByACI(&this->m_RGBM);
    }

    virtual bool isForeground() const
    {
        return CmEntityColor::static_isForeground(&this->m_RGBM);
    }
    virtual bool isByDgnIndex() const
    {
        return CmEntityColor::static_isByDgnIndex(&this->m_RGBM);
    }

    bool isNone() const
    {
        return CmEntityColor::static_isNone(&this->m_RGBM);
    }

    virtual UInt32 color() const
    {
        return CmEntityColor::static_color(&this->m_RGBM);
    }
    virtual void setColor(UInt32 color)
    {
        CmEntityColor::static_setColor(&this->m_RGBM, color);
    }

    virtual void setRGB(UInt8 red, UInt8 green, UInt8 blue)
    {
        CmEntityColor::static_setRGB(&this->m_RGBM, red, green, blue);
    }
    virtual void setRed(UInt8 red)
    {
        CmEntityColor::static_setRed(&this->m_RGBM, red);
    }
    virtual void setGreen(UInt8 green)
    {
        CmEntityColor::static_setGreen(&this->m_RGBM, green);
    }
    virtual void setBlue(UInt8 blue)
    {
        CmEntityColor::static_setBlue(&this->m_RGBM, blue);
    }
    virtual UInt8 red() const
    {
        return CmEntityColor::static_red(&this->m_RGBM);
    }
    virtual UInt8 green() const
    {
        return CmEntityColor::static_green(&this->m_RGBM);
    }
    virtual UInt8 blue() const
    {
        return CmEntityColor::static_blue(&this->m_RGBM);
    }

    virtual UInt16 colorIndex() const
    {
        return CmEntityColor::static_colorIndex(&this->m_RGBM);
    }
    virtual void setColorIndex(UInt16 colorIndex)
    {
        CmEntityColor::static_setColorIndex(&this->m_RGBM, colorIndex);
    }

    virtual bool setNames(const AcString& colorName, const AcString& bookName = "")
    {
        this->m_colorName = colorName;
        this->m_bookName = bookName;
        return true;
    }
#if EMSDK
    virtual bool emsdk_setNames(const emstring& colorName) {
        return this->setNames(colorName.c_str());
    }
    virtual bool emsdk_setNames(const emstring& colorName, const emstring& bookName) {
        return this->setNames(colorName.c_str(), bookName.c_str());
    }
#endif

    virtual AcString colorName() const
    {
        if (this->m_colorName != "") {
            return this->m_colorName;
        }

        AcString name;
        if (this->colorMethod() == CmEntityColor::ColorMethod::kByLayer) {
            name = "ByLayer";
        }
        else if (this->colorMethod() == CmEntityColor::ColorMethod::kByBlock) {
            name = "ByBlock";
        }
        else if (this->colorMethod() == CmEntityColor::ColorMethod::kByACI) {
            if (this->colorIndex() == 0) {
                name = "ByBlock";
            }
            else if (this->colorIndex() == 256) {
                name = "ByLayer";
            }
            else if (this->colorIndex() == 1) {
                name = "红";
            }
            else if (this->colorIndex() == 2) {
                name = "黄";
            }
            else if (this->colorIndex() == 3) {
                name = "绿";
            }
            else if (this->colorIndex() == 4) {
                name = "青";
            }
            else if (this->colorIndex() == 5) {
                name = "蓝";
            }
            else if (this->colorIndex() == 6) {
                name = "洋红";
            }
            else if (this->colorIndex() == 7) {
                name = "白";
            }
            else {
                name.format("%d", this->colorIndex());
            }
        }
        else if (this->colorMethod() == CmEntityColor::ColorMethod::kByColor) {
            name.format("%d,%d,%d", this->red(), this->green(), this->blue());
        }
        return name;
    }
#if EMSDK
    virtual emstring emsdk_colorName()const {
        emstring name = this->colorName().constPtr();
        return name;
    }
#endif

    virtual AcString bookName() const
    {
        return this->m_bookName;
    }
#if EMSDK
    virtual emstring emsdk_bookName()const {
        emstring name = this->bookName().constPtr();
        return name;
    }
#endif



    CmEntityColor entityColor() const
    {
        CmEntityColor color;
        color.setColor(m_RGBM);
        return color;
    }


private:
    enum NameFlags
    {    
        kNoNames      = 0,
        kHasColorName = 1,
        kHasBookName  = 2
    };

    CmEntityColor::RGBM m_RGBM;
    AcString m_colorName;
    AcString m_bookName;
 };



#endif
