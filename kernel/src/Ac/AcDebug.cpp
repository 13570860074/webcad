#include <chrono>
#include "AcDebug.h"
#include "GePoint2d.h"
#include "GePoint3d.h"
#include "GeVector2d.h"
#include "GeVector3d.h"
#include "GeMatrix2d.h"
#include "GeMatrix3d.h"
#include "GeLineSeg2d.h"
#include "GeLineSeg3d.h"
#include "GeRay2d.h"
#include "GeRay3d.h"
#include "GeLine2d.h"
#include "GeLine3d.h"
using namespace std::chrono;

bool AcDebug::isOpenDebug = false;

high_resolution_clock::time_point tag_startTime_point;
bool AcDebug::isPrintDebug()
{
	return AcDebug::isOpenDebug;
}
void AcDebug::openDebug()
{
	AcDebug::isOpenDebug = true;
}
void AcDebug::closeDebug()
{
	AcDebug::isOpenDebug = false;
}

void AcDebug::startTag()
{
	tag_startTime_point = high_resolution_clock::now();
}
unsigned int AcDebug::endTag(bool _isPrint)
{
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - tag_startTime_point);
	if (_isPrint == true)
	{
		AcString str;
		str.format("程序运行时间: %d ms", duration.count());
		std::cout << str.string() << std::endl;
	}
	return duration.count();
}

void AcDebug::print_debug(int val)
{
	if (AcDebug::isOpenDebug == true)
	{
		AcDebug::print(val);
	}
}
void AcDebug::print_debug(double val)
{
	if (AcDebug::isOpenDebug == true)
	{
		AcDebug::print(val);
	}
}
void AcDebug::print_debug(const std::string &format)
{
	if (AcDebug::isOpenDebug == true)
	{
		AcDebug::print(format);
	}
}
void AcDebug::print_debug(const ACHAR* args, ...)
{
	if (AcDebug::isOpenDebug == true)
	{
		va_list list;
		va_start(list, args);
		std::string str = AcString::_format(args, list);
		va_end(list);
		std::cout << str << std::endl;
	}
}
void AcDebug::print(const std::string& format) {
	std::cout << format << std::endl;
}
void AcDebug::print(const ACHAR* args, ...) {

	va_list list;
	va_start(list, args);
	std::string str = AcString::_format(args, list);
	va_end(list);
	std::cout << str << std::endl;
}
void AcDebug::print(int val, const AcString& prompt) {
	AcString str;
	str.format("%d", val);
	std::cout << prompt.constPtr() << ":" << str.string() << std::endl;
}
void AcDebug::print(double val, const AcString& prompt) {
	AcString str;
	str.format("%g", val);
	std::cout << prompt.constPtr() << ":" << str.string() << std::endl;
}
void AcDebug::print(const GePoint2d& pt, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("point:%.4f,%.4f", pt.x, pt.y);
	}
	else {
		AcDebug::print("%s:%.4f,%.4f", prompt.constPtr(), pt.x, pt.y);
	}
}
void AcDebug::print(const GePoint3d& pt, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("point:%.4f,%.4f,%.4f", pt.x, pt.y, pt.z);
	}
	else {
		AcDebug::print("%s:%.4f,%.4f,%.4f", prompt.constPtr(), pt.x, pt.y, pt.z);
	}
}
void AcDebug::print(const GeVector2d& ver, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("vector:%.4f,%.4f", ver.x, ver.y);
	}
	else {
		AcDebug::print("%s:%.4f,%.4f", prompt.constPtr(), ver.x, ver.y);
	}
}
void AcDebug::print(const GeVector3d& ver, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("vector:%.4f,%.4f,%.4f", ver.x, ver.y, ver.z);
	}
	else {
		AcDebug::print("%s:%.4f,%.4f,%.4f", prompt.constPtr(), ver.x, ver.y, ver.z);
	}
}
void AcDebug::print(const GeMatrix2d& ver, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("GeMatrix2d:%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f", 
			ver.entry[0][0], ver.entry[0][1], ver.entry[0][2],
			ver.entry[1][0], ver.entry[1][1], ver.entry[1][2], 
			ver.entry[2][0], ver.entry[2][1], ver.entry[2][2]);
	}
	else {
		AcDebug::print("%s:%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f", prompt.constPtr(),
			ver.entry[0][0], ver.entry[0][1], ver.entry[0][2],
			ver.entry[1][0], ver.entry[1][1], ver.entry[1][2],
			ver.entry[2][0], ver.entry[2][1], ver.entry[2][2]);
	}
}
void AcDebug::print(const GeMatrix3d& ver, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("GeMatrix2d:%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f",
			ver.entry[0][0], ver.entry[0][1], ver.entry[0][2], ver.entry[0][3],
			ver.entry[1][0], ver.entry[1][1], ver.entry[1][2], ver.entry[1][3],
			ver.entry[2][0], ver.entry[2][1], ver.entry[2][2], ver.entry[2][3],
			ver.entry[3][0], ver.entry[3][1], ver.entry[3][2], ver.entry[3][3]
		);
	}
	else {
		AcDebug::print("%s:%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f", prompt.constPtr(),
			ver.entry[0][0], ver.entry[0][1], ver.entry[0][2], ver.entry[0][3],
			ver.entry[1][0], ver.entry[1][1], ver.entry[1][2], ver.entry[1][3],
			ver.entry[2][0], ver.entry[2][1], ver.entry[2][2], ver.entry[2][3],
			ver.entry[3][0], ver.entry[3][1], ver.entry[3][2], ver.entry[3][3]
		);
	}
}
void AcDebug::print(const GeLineSeg2d& line, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("lineSeg-start:%.4f,%.4f", line.startPoint().x, line.startPoint().y);
		AcDebug::print("lineSeg-end:%.4f,%.4f", line.startPoint().x, line.startPoint().y);
	}
	else {
		AcDebug::print("%s-startPoint:%.4f,%.4f", prompt.constPtr(), line.startPoint().x, line.startPoint().y);
		AcDebug::print("%s-endPoint:%.4f,%.4f", prompt.constPtr(), line.startPoint().x, line.startPoint().y);
	}
}
void AcDebug::print(const GeLineSeg3d& line, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("lineSeg-start:%.4f,%.4f,%.4f", line.startPoint().x, line.startPoint().y, line.startPoint().z);
		AcDebug::print("lineSeg-end:%.4f,%.4f,%.4f", line.startPoint().x, line.startPoint().y, line.startPoint().z);
	}
	else {
		AcDebug::print("%s-startPoint:%.4f,%.4f,%.4f", prompt.constPtr(), line.startPoint().x, line.startPoint().y, line.startPoint().z);
		AcDebug::print("%s-endPoint:%.4f,%.4f,%.4f", prompt.constPtr(), line.startPoint().x, line.startPoint().y, line.startPoint().z);
	}
}
void AcDebug::print(const GeRay2d& line, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("ray-origin:%.4f,%.4f", line.pointOnLine().x, line.pointOnLine().y);
		AcDebug::print("ray-direction:%.4f,%.4f", line.direction().x, line.direction().y);
	}
	else {
		AcDebug::print("%s-origin:%.4f,%.4f", prompt.constPtr(), line.pointOnLine().x, line.pointOnLine().y);
		AcDebug::print("%s-direction:%.4f,%.4f", prompt.constPtr(), line.direction().x, line.direction().y);
	}
}
void AcDebug::print(const GeRay3d& line, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("ray-origin:%.4f,%.4f,%.4f", line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		AcDebug::print("ray-direction:%.4f,%.4f,%.4f", line.direction().x, line.direction().y, line.direction().z);
	}
	else {
		AcDebug::print("%s-origin:%.4f,%.4f,%.4f", prompt.constPtr(), line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		AcDebug::print("%s-direction:%.4f,%.4f,%.4f", prompt.constPtr(), line.direction().x, line.direction().y, line.direction().z);
	}
}
void AcDebug::print(const GeLine2d& line, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("line-origin:%.4f,%.4f", line.pointOnLine().x, line.pointOnLine().y);
		AcDebug::print("line-direction:%.4f,%.4f", line.direction().x, line.direction().y);
	}
	else {
		AcDebug::print("%s-origin:%.4f,%.4f", prompt.constPtr(), line.pointOnLine().x, line.pointOnLine().y);
		AcDebug::print("%s-direction:%.4f,%.4f", prompt.constPtr(), line.direction().x, line.direction().y);
	}
}
void AcDebug::print(const GeLine3d& line, const AcString& prompt) {
	if (prompt == "") {
		AcDebug::print("line-origin:%.4f,%.4f,%.4f", line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		AcDebug::print("line-direction:%.4f,%.4f,%.4f", line.direction().x, line.direction().y, line.direction().z);
	}
	else {
		AcDebug::print("%s-origin:%.4f,%.4f,%.4f", prompt.constPtr(), line.pointOnLine().x, line.pointOnLine().y, line.pointOnLine().z);
		AcDebug::print("%s-direction:%.4f,%.4f,%.4f", prompt.constPtr(), line.direction().x, line.direction().y, line.direction().z);
	}
}