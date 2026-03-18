#include "ReOrthogonalCamera.h"
#include "ReImpl.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"


ReOrthogonalCamera::ReOrthogonalCamera() {
	this->m_pImpl = new ReOrthogonalCameraImpl();
}
ReOrthogonalCamera::~ReOrthogonalCamera() {

}

const std::string ReOrthogonalCamera::information() {

	rapidjson::GenericStringBuffer<rapidjson::UTF8<char>> StrJsonBuff;
	rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>>* pJson = new rapidjson::Writer<rapidjson::GenericStringBuffer<rapidjson::UTF8<char>>>(StrJsonBuff);

	pJson->StartObject();

	pJson->Key("matrix");
	pJson->StartArray();
	for (int i = 0; i < 4; i++) {
		for (int u = 0; u < 4; u++) {
			pJson->Double(this->matrix().get(i, u));
		}
	}
	pJson->EndArray();

	pJson->Key("top");
	pJson->Double(this->top());

	pJson->Key("bottom");
	pJson->Double(this->bottom());

	pJson->Key("left");
	pJson->Double(this->left());

	pJson->Key("right");
	pJson->Double(this->right());

	pJson->Key("lowerLeftCorner");
	pJson->StartObject();
	pJson->Key("x");
	pJson->Double(this->lowerLeftCorner().x);
	pJson->Key("y");
	pJson->Double(this->lowerLeftCorner().y);
	pJson->EndObject();

	pJson->Key("upperRightCorner");
	pJson->StartObject();
	pJson->Key("x");
	pJson->Double(this->upperRightCorner().x);
	pJson->Key("y");
	pJson->Double(this->upperRightCorner().y);
	pJson->EndObject();

	pJson->EndObject();

	const char* pString = StrJsonBuff.GetString();
	delete pJson;

	return pString;
}


double ReOrthogonalCamera::top() {
	return RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->top;
}
double ReOrthogonalCamera::bottom() {
	return RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->bottom;
}
double ReOrthogonalCamera::left() {
	return RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->left;
}
double ReOrthogonalCamera::right() {
	return RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->right;
}

//设置范围
void ReOrthogonalCamera::setTop(double _v) {
	RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->top = _v;
}
void ReOrthogonalCamera::setBottom(double _v) {
	RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->bottom = _v;
}
void ReOrthogonalCamera::setLeft(double _v) {
	RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->left = _v;
}
void ReOrthogonalCamera::setRight(double _v) {
	RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->right = _v;
}

GePoint2d ReOrthogonalCamera::lowerLeftCorner() const {
	return RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->lowerLeftCorner;
}
GePoint2d ReOrthogonalCamera::upperRightCorner() const {
	return RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->upperRightCorner;
}
void ReOrthogonalCamera::setLowerLeftCorner(const GePoint2d& _point) {
	RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->lowerLeftCorner = _point;
}
void ReOrthogonalCamera::setUpperRightCorner(const GePoint2d& _point) {
	RE_IMP_ORTHOGONALCAMERA(this->m_pImpl)->upperRightCorner = _point;
}
