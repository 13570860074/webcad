#ifndef ACMAP_H
#define ACMAP_H

#include <cctype>

#include "AcArray.h"
#include "AcString.h"

template<typename T1, typename T2>
class AcMapRecord {
public:
	AcMapRecord(T1 _key, T2 _obj) {
		this->index = 0;
		this->name = _key;
		this->object = _obj;
	}

	T1 name;
	T2 object;
	unsigned int index;
};

template<typename T1, typename T2>
class AcMapRecordTable :public AcArray<AcMapRecord<T1, T2>*> {
public:
	AcMapRecordTable() {
	}
	virtual ~AcMapRecordTable() {
		for (int i = this->length() - 1; i >= 0; i--) {
			auto* p = this->at(i);
			delete p;
		}
		this->removeAll();
	}
};

/**
* 1.支持AcString
* 2.支持std::string
*
* maxMapRecordTable:最大支持的记录表数量
* AcArray<AcMapRecordTable<T1, T2>*>:记录表,在初始化时创建指定数量的记录表,每个记录表种每条记录key的长度是相同的
* publicRecords:当存在key长度大于记录表数量时则将记录加入到该数组中
* records:按先后顺序将排列的记录数组
* endRecord:表示最后加入到map的一条记录
*/
template<typename T1, typename T2>
class AcMap :private AcArray<AcMapRecordTable<T1, T2>*> {
public:
	AcMap() {
		this->maxMapRecordTable = 64;
		for (int i = 0; i < this->maxMapRecordTable; i++) {
			AcMapRecordTable<T1, T2>* mapRecordTable = new AcMapRecordTable<T1, T2>();
			this->append(mapRecordTable);
		}
	}
	AcMap(int max) {
		this->m_numRecord = 0;
		this->maxMapRecordTable = max;
		for (int i = 0; i < this->maxMapRecordTable; i++) {
			AcMapRecordTable<T1, T2>* mapRecordTable = new AcMapRecordTable<T1, T2>();
			this->append(mapRecordTable);
		}
	}
	virtual ~AcMap() {
		for (int i = this->length() - 1; i >= 0; i--) {
			auto* p = this->at(i);
			delete p;
		}
		this->removeAll();
		realloc_arry_all(this->publicRecords);
	}

private:
	int maxMapRecordTable;
	AcArray<AcMapRecord<T1, T2>*> records;
	AcArray<AcMapRecord<T1, T2>*> publicRecords;

public:

	/* 添加对象 */
	void insert(T1 key, T2 val) {

		const unsigned int len = key.length();
		if (len < static_cast<unsigned int>(this->maxMapRecordTable)) {
			AcMapRecord<T1, T2>* mapRecord = new AcMapRecord<T1, T2>(key, val);
			mapRecord->index = this->records.length() + 1;
			this->records.append(mapRecord);

			AcMapRecordTable<T1, T2>* mapRecordTable = this->at(len);
			mapRecordTable->append(mapRecord);
		}
		else {
			AcMapRecord<T1, T2>* mapRecord = new AcMapRecord<T1, T2>(key, val);
			mapRecord->index = this->records.length() + 1;
			this->records.append(mapRecord);

			this->publicRecords.append(mapRecord);
		}
	}

	/* 查询对象 */
	T2 find(T1 key, const bool isIgnoreCase = false) {
		AcMapRecord<T1, T2>* record = this->findRecord(key, isIgnoreCase);
		if (record != NULL) {
			return record->object;
		}
		return NULL;
	}

	/* 查询记录 */
	AcMapRecord<T1, T2>* findRecord(T1 key, const bool isIgnoreCase = false) {
		AcMapRecord<T1, T2>* object = NULL;
		const unsigned int keyLength = key.length();

		if (keyLength < static_cast<unsigned int>(this->maxMapRecordTable)) {
			AcMapRecordTable<T1, T2>* mapRecordTable = this->at(keyLength);
			for (int i = 0; i < mapRecordTable->length(); i++) {
				if (AcString::isEqual(mapRecordTable->at(i)->name, key, isIgnoreCase) == true) {
					object = mapRecordTable->at(i);
					break;
				}
			}
		}
		else {
			for (int i = 0; i < this->publicRecords.length(); i++) {
				if (AcString::isEqual(this->publicRecords[i]->name, key, isIgnoreCase) == true) {
					object = this->publicRecords[i];
					break;
				}
			}
		}

		return object;
	}

	/* 查询所有对象 */
	void findAll(AcArray<T2>& objs) {
		for (int i = 0; i < this->records.length(); i++) {
			objs.append(this->records[i]->object);
		}
	}

	/* 记录数量 */
	int numRecord()const {
		return this->records.length();
	}

	/* 移除记录 */
	bool remove(T1 key, const bool isIgnoreCase = false) {
		AcMapRecord<T1, T2>* record = this->findRecord(key, isIgnoreCase);
		if (record == NULL) {
			return false;
		}

		this->records.removeAt(record->index - 1);
		for (int i = 0; i < this->records.length(); i++) {
			this->records[i]->index = i + 1;
		}

		const unsigned int keyLength = key.length();
		if (keyLength < static_cast<unsigned int>(this->maxMapRecordTable)) {
			AcMapRecordTable<T1, T2>* recordTable = this->at(keyLength);
			for (int i = 0; i < recordTable->length(); i++) {
				if (recordTable->at(i) == record) {
					AcMapRecord<T1, T2>* temp = recordTable->at(i);
					delete temp;
					recordTable->removeAt(i);
					break;
				}
			}
		}
		else {
			for (int i = 0; i < this->publicRecords.length(); i++) {
				if (this->publicRecords.at(i) == record) {
					AcMapRecord<T1, T2>* temp = this->publicRecords.at(i);
					delete temp;
					this->publicRecords.removeAt(i);
					break;
				}
			}
		}

		return true;
	}

	/* 查询所有记录 */
	void findAllRecord(AcArray<AcMapRecord<T1, T2>*>& records) {
		for (int i = 0; i < this->records.length(); i++) {
			records.append(this->records[i]);
		}
	}

	/* 更新索引名称 */
	void updateRecordKey(const T1& oldKey, const T1& newKey, const bool isIgnoreCase = false) {

		// 查询到记录
		AcMapRecord<T1, T2>* record = this->findRecord(oldKey, isIgnoreCase);
		if (record == NULL) {
			return;
		}

		// 如果记录在表中则从表中移除
		const unsigned int oldKeyLength = oldKey.length();
		if (oldKeyLength < static_cast<unsigned int>(this->maxMapRecordTable)) {
			unsigned int len = oldKeyLength;
			AcMapRecordTable<T1, T2>* mapRecordTable = this->at(len);
			mapRecordTable->remove(record);
		}

		// 修改记录名称并加入新的表中
		if (record != NULL) {

			record->name = newKey;

			const unsigned int len = record->name.length();
			if (len < static_cast<unsigned int>(this->maxMapRecordTable)) {
				AcMapRecordTable<T1, T2>* mapRecordTable = this->at(len);
				mapRecordTable->append(record);
			}
		}
	}
};



#endif
