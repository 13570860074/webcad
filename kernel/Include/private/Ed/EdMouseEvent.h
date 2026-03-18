#ifndef EDMOUSEEVENT_H
#define EDMOUSEEVENT_H

#include <string>
#include "EdUserInputEvent.h"

class EdMouseEvent :public EdUserInputEvent {
public:
	EdMouseEvent();
	virtual ~EdMouseEvent();

public:
	bool altKey;
	int button;			// 0表示左键，1表示中键，2表示右键
	double clientX;
	double clientY;
	bool ctrlKey;
	bool shiftKey;

protected:

	/** 克隆数据 */
	void clone(EdMouseEvent*& mouseEvent) const;

public:

	/** 转字符串 */
	static std::string stringify(const EdMouseEvent _data);

	/** 解析json数据 */
	static EdMouseEvent parse(const std::string& _data);

	/** 类名 */
	virtual std::string className() const;

	/** 克隆数据 */
	virtual EdMouseEvent* clone() const;
};

#endif