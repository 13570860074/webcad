#include "export.h"
#include "AcArray.h"
#include "AcString.h"
#include "CmEntityColor.h"
#include "CmColorBase.h"
#include "CmColor.h"
#include "CmTransparency.h"
#include "kernel.h"




#if EMSDK



EMSCRIPTEN_BINDINGS(Export) {


	//Color
	enum_<CmEntityColor::Color>("Color")
		.value("kRed", CmEntityColor::Color::kRed)
		.value("kGreen", CmEntityColor::Color::kGreen)
		.value("kBlue", CmEntityColor::Color::kBlue);

	//ColorMethod
	enum_<CmEntityColor::ColorMethod>("ColorMethod")
		.value("kByLayer", CmEntityColor::ColorMethod::kByLayer)
		.value("kByBlock", CmEntityColor::ColorMethod::kByBlock)
		.value("kByColor", CmEntityColor::ColorMethod::kByColor)
		.value("kByACI", CmEntityColor::ColorMethod::kByACI)
		.value("kByPen", CmEntityColor::ColorMethod::kByPen)
		.value("kForeground", CmEntityColor::ColorMethod::kForeground)
		.value("kByDgnIndex", CmEntityColor::ColorMethod::kByDgnIndex)
		.value("kNone", CmEntityColor::ColorMethod::kNone);

	//ACIcolorMethod
	enum_<CmEntityColor::ACIcolorMethod>("ACIcolorMethod")
		.value("kACIbyBlock", CmEntityColor::ACIcolorMethod::kACIbyBlock)
		.value("kACIforeground", CmEntityColor::ACIcolorMethod::kACIforeground)
		.value("kACIbyLayer", CmEntityColor::ACIcolorMethod::kACIbyLayer)
		.value("kACIclear", CmEntityColor::ACIcolorMethod::kACIclear)
		.value("kACIRed", CmEntityColor::ACIcolorMethod::kACIRed)
		.value("kACIYellow", CmEntityColor::ACIcolorMethod::kACIYellow)
		.value("kACIGreen", CmEntityColor::ACIcolorMethod::kACIGreen)
		.value("kACICyan", CmEntityColor::ACIcolorMethod::kACICyan)
		.value("kACIBlue", CmEntityColor::ACIcolorMethod::kACIBlue)
		.value("kACIMagenta", CmEntityColor::ACIcolorMethod::kACIMagenta)
		.value("kACIWhite", CmEntityColor::ACIcolorMethod::kACIWhite)
		.value("kACIstandard", CmEntityColor::ACIcolorMethod::kACIstandard)
		.value("kACImaximum", CmEntityColor::ACIcolorMethod::kACImaximum)
		.value("kACInone", CmEntityColor::ACIcolorMethod::kACInone)
		.value("kACIminimum", CmEntityColor::ACIcolorMethod::kACIminimum);

	//CmEntityColor
	emscripten::class_<CmEntityColor, base<EmObject>>("CmEntityColor")
		.constructor<>()
		.constructor<const CmEntityColor&>()
		.constructor<UInt8, UInt8, UInt8>()
		.function("setColorMethod", &CmEntityColor::setColorMethod)
		.function("colorMethod", &CmEntityColor::colorMethod)
		.function("setColor", &CmEntityColor::setColor)
		.function("color", &CmEntityColor::color)
		.function("setColorIndex", &CmEntityColor::setColorIndex)
		.function("colorIndex", &CmEntityColor::colorIndex)
		.function("setRGB", &CmEntityColor::setRGB)
		.function("setRed", &CmEntityColor::setRed)
		.function("setGreen", &CmEntityColor::setGreen)
		.function("setBlue", &CmEntityColor::setBlue)
		.function("red", &CmEntityColor::red)
		.function("green", &CmEntityColor::green)
		.function("blue", &CmEntityColor::blue)
		.function("isByColor", &CmEntityColor::isByColor)
		.function("isByLayer", &CmEntityColor::isByLayer)
		.function("isByBlock", &CmEntityColor::isByBlock)
		.function("isByACI", &CmEntityColor::isByACI)
		.function("isForeground", &CmEntityColor::isForeground)
		.function("isByDgnIndex", &CmEntityColor::isByDgnIndex)
		.function("isNone", &CmEntityColor::isNone);

	//CmColorBase
	emscripten::class_<CmColorBase, base<EmObject>>("CmColorBase")
		.function("colorMethod", &CmColorBase::colorMethod)
		.function("setColorMethod", &CmColorBase::setColorMethod)
		.function("isByColor", &CmColorBase::isByColor)
		.function("isByLayer", &CmColorBase::isByLayer)
		.function("isByBlock", &CmColorBase::isByBlock)
		.function("isByACI", &CmColorBase::isByACI)
		.function("isForeground", &CmColorBase::isForeground)
		.function("isByDgnIndex", &CmColorBase::isByDgnIndex)
		.function("color", &CmColorBase::color)
		.function("setColor", &CmColorBase::setColor)
		.function("setRGB", &CmColorBase::setRGB)
		.function("setRed", &CmColorBase::setRed)
		.function("setGreen", &CmColorBase::setGreen)
		.function("setBlue", &CmColorBase::setBlue)
		.function("red", &CmColorBase::red)
		.function("green", &CmColorBase::green)
		.function("blue", &CmColorBase::blue)
		.function("colorIndex", &CmColorBase::colorIndex)
		.function("setColorIndex", &CmColorBase::setColorIndex)
		.function("setNames", select_overload<bool(const emstring&)>(&CmColorBase::emsdk_setNames), pure_virtual())
		.function("setNames", select_overload<bool(const emstring&, const emstring&)>(&CmColorBase::emsdk_setNames), pure_virtual())
		.function("colorName", &CmColorBase::emsdk_colorName, pure_virtual())
		.function("bookName", &CmColorBase::emsdk_bookName, pure_virtual());

	//CmColor
	emscripten::class_<CmColor, base<CmColorBase>>("CmColor")
		.constructor<>()
		.constructor<const CmColor>()
		.function("isNone", &CmColor::isNone)
		.function("entityColor", &CmColor::entityColor)
		.function("setNames", select_overload<bool(const emstring&)>(&CmColor::emsdk_setNames))
		.function("setNames", select_overload<bool(const emstring&, const emstring&)>(&CmColor::emsdk_setNames))
		.function("colorName", &CmColor::emsdk_colorName)
		.function("bookName", &CmColor::emsdk_bookName);

	//transparencyMethod
	enum_<CmTransparency::transparencyMethod>("transparencyMethod")
		.value("kByLayer", CmTransparency::transparencyMethod::kByLayer)
		.value("kByBlock", CmTransparency::transparencyMethod::kByBlock)
		.value("kByAlpha", CmTransparency::transparencyMethod::kByAlpha)
		.value("kErrorValue", CmTransparency::transparencyMethod::kErrorValue);

	//CmTransparency
	emscripten::class_<CmTransparency, base<EmObject>>("CmTransparency")
		.constructor()
		.constructor<UInt8>()
		.function("setAlpha", &CmTransparency::setAlpha)
		.function("alpha", &CmTransparency::alpha)
		.function("setAlphaPercent", &CmTransparency::setAlphaPercent)
		.function("method", &CmTransparency::method)
		.function("setMethod", &CmTransparency::setMethod)
		.function("isByAlpha", &CmTransparency::isByAlpha)
		.function("isByBlock", &CmTransparency::isByBlock)
		.function("isByLayer", &CmTransparency::isByLayer)
		.function("isInvalid", &CmTransparency::isInvalid)
		.function("isClear", &CmTransparency::isClear)
		.function("isSolid", &CmTransparency::isSolid)
		.function("serializeOut", &CmTransparency::serializeOut)
		.function("serializeIn", &CmTransparency::serializeIn);

}

#endif
