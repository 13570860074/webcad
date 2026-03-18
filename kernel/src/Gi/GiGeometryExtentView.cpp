#include "GiGeometryExtentView.h"




GiGeometryExtentView::GiGeometryExtentView() {
	this->m_viewHeight = 0.0;
	this->m_bRefresh = true;
}
GiGeometryExtentView::~GiGeometryExtentView() {

}

DbObjectId GiGeometryExtentView::blockId()const {
	return this->m_blockId;
}
void GiGeometryExtentView::setBlockId(const DbObjectId& id) {
	this->m_blockId = id;
}

DbObjectId GiGeometryExtentView::viewportId()const {
	return this->m_viewportId;
}
void GiGeometryExtentView::setViewportId(const DbObjectId& id) {
	this->m_viewportId = id;
}

GePoint2d GiGeometryExtentView::viewCenter()const {
	return this->m_viewCenter;
}
void GiGeometryExtentView::setViewCenter(const GePoint2d& pt) {
	this->m_viewCenter = pt;
}

double GiGeometryExtentView::viewHeight()const {
	return this->m_viewHeight;
}
void GiGeometryExtentView::setViewHeight(const double v) {
	this->m_viewHeight = v;
}
bool GiGeometryExtentView::isRefresh()const {
	return this->m_bRefresh;
}
void GiGeometryExtentView::markRefresh(const bool bRefresh) {
	this->m_bRefresh = bRefresh;
}
AcArray<GiEntityGeometry*>* GiGeometryExtentView::geometrys() {
	return &this->m_geometrys;
}