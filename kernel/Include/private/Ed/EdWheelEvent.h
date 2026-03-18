#ifndef EDWHEELEVENT_H
#define EDWHEELEVENT_H

#include "EdMouseEvent.h"

class EdWheelEvent :public EdMouseEvent {
public:
	EdWheelEvent();
	virtual ~EdWheelEvent();

public:
	int wheelDelta;

protected:

	/** 克隆数据 */
	void clone(EdWheelEvent*& wheelEvent) const;

public:
	/** 转字符串 */
	static std::string stringify(const EdWheelEvent _data);

	/** 解析json数据 */
	static EdWheelEvent parse(const std::string& _data);


	/** 类名 */
	virtual std::string className() const;

	/** 克隆数据 */
	virtual EdWheelEvent* clone() const;
};

#endif