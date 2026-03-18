#include "ReCamera.h"
#include "GeMatrix3d.h"
#include "ReImpl.h"



ReCamera::ReCamera() {
	this->m_pImpl = new ReCameraImpl();
}
ReCamera::~ReCamera() {

}


const std::string ReCamera::information() {

	return "";
}


GeMatrix3d ReCamera::matrix() const {
	return RE_IMP_CAMERA(this->m_pImpl)->matrix;
}


void ReCamera::setMatrix(const GeMatrix3d& _mat) {

	RE_IMP_CAMERA(this->m_pImpl)->matrix = _mat;
}
