#ifndef EDKEYBOARDEVENT_H
#define EDKEYBOARDEVENT_H


#include "EdUserInputEvent.h"

class EdKeyboardEvent :public EdUserInputEvent {
public:
	EdKeyboardEvent();
	virtual ~EdKeyboardEvent();

public:
	bool altKey;
	bool ctrlKey;
	bool shiftKey;
	int keyCode;

public:
	/** 转字符串 */
	static std::string stringify(const EdKeyboardEvent _data);

	/** 解析json数据 */
	static EdKeyboardEvent parse(const std::string& _data);

	/** 克隆数据 */
	virtual EdKeyboardEvent* clone() const;
};

#endif