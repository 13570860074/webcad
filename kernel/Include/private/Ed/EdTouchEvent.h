#ifndef EDTOUCHEVENT_H
#define EDTOUCHEVENT_H


#include "EdTouchArray.h"
#include "EdUserInputEvent.h"

class EdTouchEvent :public EdUserInputEvent {
public:
	EdTouchEvent();
	virtual ~EdTouchEvent();

private:
	EdTouchArray *m_touches;
	EdTouchArray *m_changedTouches;

public:
	bool altKey;
	bool ctrlKey;
	bool shiftKey;
	EdTouchArray* touches() const;
	EdTouchArray* changedTouches() const;

public:
	// 转字符串
	static std::string stringify(const EdTouchEvent _data);

	// 解析json数据
	static EdTouchEvent parse(const std::string& _data);

	/** 克隆 */
	EdTouchEvent* clone() const;
};

#endif