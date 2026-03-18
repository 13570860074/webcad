#ifndef EDSELECTIONSETINDEX_H
#define EDSELECTIONSETINDEX_H

#include "acedads.h"

class EdSelectionSetIndex
{
public:
	EdSelectionSetIndex();
	virtual ~EdSelectionSetIndex();

private:
	unsigned int m_size;
	bool m_isEffective;

public:
	int index;
	int length;
	ads_name *selectionSet;

private:

	/* 扩容 */
	void expansion();

public:

	/* 初始化 */
	void init(const unsigned int len = 0);

	/* 是否有效 */
	bool isEffective() const;

	/* 容量 */
	unsigned int capacity() const;

	/* 添加成员 */
	void add(const ads_name name);

	/* 释放 */
	void release();
};

#endif