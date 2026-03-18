#include "AcSystem.h"
#include "export.h"
#include <thread>
#include <mutex>
#if WIN32
#include <conio.h>
#include <windows.h>
#endif
#if EMSDK
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <time.h> 
#include <unistd.h> 
#endif
#if EMSDK
#include <emscripten.h>
#endif
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"



class FTimerAssociation {
public:
	FTimerAssociation() {
		this->id = 0;
		this->param = NULL;
		this->fun = NULL;
		this->intervalId = 0;
		this->isParam = false;
	}

	void* param;
	bool isParam;
	unsigned int id;
	int intervalId;
	void* fun;
};


/** 选择本地文件实现
* 1. 调用emsdk_function_init_selectLocalFiles函数向js中注入选择文件所用到的js函数
*		(1). window.emsdk_function_selectLocalFiles为选择文件的函数
*		(2). window.emsdk_function_asyncSelectLocalFiles为异步选择文件的函数
* 4. c++中调用emsdk_function_selectLocalFiles实现等待选择文件完成
* 5. 考虑到EM_ASYNC_JS宏无法返回值,所以定义了window.emsdk_variable_files,这个全局变量用于存储选择的文件列表
* 6. c++调用push_emsdk_variable_fileState函数将选择的文件状态值转换为字符串返回
* 7. c++解析文件状态
* 8. 调用push_emsdk_variable_filedata获取文件内容
*/
EM_JS(void, emsdk_function_init_selectLocalFiles, (), {
	if (window.emsdk_function_selectLocalFiles == null) {
		window.emsdk_function_selectLocalFiles = function(_accept, fun) {
			let input = document.createElement('input');
			input.setAttribute('type', 'file');
			input.accept = _accept;
			input.click();
			input.onchange = (e) => {
				let inputElement = e.target;
				if (inputElement == null) {
					fun(null);
					return;
				}
				if (inputElement.files == null) {
					fun(null);
					return;
				}
				fun(inputElement.files);
				return;
			}
		}
	}
	if (window.emsdk_function_asyncSelectLocalFiles == null) {
		window.emsdk_function_asyncSelectLocalFiles = async function(accept) {
			return new Promise((resolve) => {
				window.emsdk_function_selectLocalFiles(accept, (files) => {
					resolve(files);
				});
			});
		}
	}
	if (window.emsdk_function_loadFileBuffer == null) {
		window.emsdk_function_loadFileBuffer = function(file, fun) {
			let reader = new FileReader();
			reader.readAsArrayBuffer(file);
			reader.onload = (e) => {
				fun(e.target.result);
			}
		}
	}
	if (window.emsdk_function_asyncLoadFileBuffer == null) {
		window.emsdk_function_asyncLoadFileBuffer = async function(file) {
			return new Promise((resolve) => {
				window.emsdk_function_loadFileBuffer(file, (data) => {
					resolve(data);
				});
			});

		}
	}
	if (window.emsdk_function_loadFileText == null) {
		window.emsdk_function_loadFileText = function(file, code, fun) {
			let reader = new FileReader();
			reader.readAsText(file, code);
			reader.onload = (e) => {
				fun(e.target.result);
			}
		}
	}
	if (window.emsdk_function_asyncLoadFileText == null) {
		window.emsdk_function_asyncLoadFileText = async function(file, code) {
			return new Promise((resolve) => {
				window.emsdk_function_loadFileText(file, code, (data) => {
					resolve(data);
				});
			});

		}
	}
	});
EM_ASYNC_JS(void, emsdk_function_selectLocalFiles, (const char* _accept, const char* _code), {
	let accept = UTF8ToString(_accept);
	let code = UTF8ToString(_code);

	window.emsdk_variable_files = [];
	window.emsdk_variable_files = await window.emsdk_function_asyncSelectLocalFiles(accept);
	if (window.emsdk_variable_files == null) {
		return;
	}

	for (let i = 0; i < window.emsdk_variable_files.length; i++) {
		if (code == "") {
			let data = await window.emsdk_function_asyncLoadFileBuffer(window.emsdk_variable_files[i]);
			window.emsdk_variable_files[i].data = data;
		}
		else {
			let data = await window.emsdk_function_asyncLoadFileText(window.emsdk_variable_files[i], code);
			window.emsdk_variable_files[i].data = data;
		}
	}
	});
EM_JS(const char*, push_emsdk_variable_fileState, (), {
	let files = [];
	for (let i = 0; i < window.emsdk_variable_files.length; i++) {
		let file = window.emsdk_variable_files[i];
		files.push({ name:file.name, size : file.size });
	}
	return allocate(intArrayFromString(JSON.stringify(files)), 'i8', ALLOC_NORMAL);
	});
EM_JS(const char*, push_emsdk_variable_filedata, (unsigned int index), {
	if (window.emsdk_variable_files == null) {
		return null;
	}
	if (window.emsdk_variable_files.length <= index) {
		return null;
	}
	const numberArray = Array.from(new Int8Array(window.emsdk_variable_files[index].data));
	return allocate(numberArray, numberArray.length);
	});



/* 定时器 */
AcArray<FTimerAssociation*>* timerAssociations = NULL;
EM_PORT_API(void) callbackTimeout(unsigned int p, unsigned int id) {
	AcArray<FTimerAssociation*>* temps = (AcArray<FTimerAssociation*>*)p;
	if (temps != NULL) {
		for (int i = 0; i < temps->length(); i++) {
			if (temps->at(i)->id == id) {
				if (temps->at(i)->isParam == true) {
					((void (*)(void*))temps->at(i)->fun)(temps->at(i)->param);
				}
				else {
					((void (*)())temps->at(i)->fun)();
				}
				realloc_at(temps, i);
				break;
			}
		}
	}
}
EM_PORT_API(void) callbackInterval(unsigned int p, unsigned int id) {
	AcArray<FTimerAssociation*>* temps = (AcArray<FTimerAssociation*>*)p;
	if (temps != NULL) {
		for (int i = 0; i < temps->length(); i++) {
			if (temps->at(i)->id == id) {
				if (temps->at(i)->isParam == true) {
					((void (*)(void*))temps->at(i)->fun)(temps->at(i)->param);
				}
				else {
					((void (*)())temps->at(i)->fun)();
				}
				break;
			}
		}
	}
}
EM_JS(int, emsdk_setTimeout, (unsigned int p, unsigned int id, unsigned int time), {
	return setTimeout(function() {
		Module._callbackTimeout(p,id);
	}, time);
	});
EM_JS(int, emsdk_setInterval, (unsigned int p, unsigned int id, unsigned int time), {
	return setInterval(function() {
		Module._callbackInterval(p,id);
	}, time);
	});
EM_JS(void, emsdk_clearTimeout, (int intervalId), {
	clearTimeout(intervalId);
	});
EM_JS(void, emsdk_clearInterval, (int intervalId), {
	clearInterval(intervalId);
	});


unsigned int AcSystem::setInterval(void(*fun)(), unsigned int time) {
#if EMSDK
	if (timerAssociations == NULL) {
		timerAssociations = new AcArray<FTimerAssociation*>();
	}
	FTimerAssociation* p = new FTimerAssociation();
	p->id = timerAssociations->length() + 1;
	p->fun = (void*)fun;
	p->isParam = false;
	timerAssociations->append(p);
	int intervalId = emsdk_setInterval((unsigned int)timerAssociations, p->id, time);
	p->intervalId = intervalId;
	return p->id;
#endif
	return 0;
}
unsigned int AcSystem::setTimeout(void(*fun)(), unsigned int time) {
#if EMSDK
	if (timerAssociations == NULL) {
		timerAssociations = new AcArray<FTimerAssociation*>();
	}
	FTimerAssociation* p = new FTimerAssociation();
	p->id = timerAssociations->length() + 1;
	p->fun = (void*)fun;
	p->isParam = false;
	timerAssociations->append(p);
	int intervalId = emsdk_setInterval((unsigned int)timerAssociations, p->id, time);
	p->intervalId = intervalId;
	return p->id;
#endif
	return 0;
}
unsigned int AcSystem::setInterval(void(*fun)(void*), unsigned int time,void* param) {
#if EMSDK
	if (timerAssociations == NULL) {
		timerAssociations = new AcArray<FTimerAssociation*>();
	}
	FTimerAssociation* p = new FTimerAssociation();
	p->id = timerAssociations->length() + 1;
	p->fun = (void*)fun;
	p->param = param;
	p->isParam = true;
	timerAssociations->append(p);
	int intervalId = emsdk_setInterval((unsigned int)timerAssociations, p->id, time);
	p->intervalId = intervalId;
	return p->id;
#endif
	return 0;
}
unsigned int AcSystem::setTimeout(void(*fun)(void*), unsigned int time,void* param) {
#if EMSDK
	if (timerAssociations == NULL) {
		timerAssociations = new AcArray<FTimerAssociation*>();
	}
	FTimerAssociation* p = new FTimerAssociation();
	p->id = timerAssociations->length() + 1;
	p->fun = (void*)fun;
	p->param = param;
	timerAssociations->append(p);
	int intervalId = emsdk_setTimeout((unsigned int)timerAssociations, p->id, time);
	p->intervalId = intervalId;
	return p->id;
#endif
	return 0;
}
void AcSystem::clearInterval(unsigned int id) {
	if (timerAssociations == NULL) {
		return;
	}
	for (int i = 0; i < timerAssociations->length(); i++) {
		if (timerAssociations->at(i)->id = id) {
			int intervalId = timerAssociations->at(i)->intervalId;
			realloc_at(timerAssociations, i);
#if EMSDK
			emsdk_clearInterval(intervalId);
#endif
			return;
		}
	}
}
void AcSystem::clearTimeout(unsigned int id) {
	if (timerAssociations == NULL) {
		return;
	}
	for (int i = 0; i < timerAssociations->length(); i++) {
		if (timerAssociations->at(i)->id = id) {
			int intervalId = timerAssociations->at(i)->intervalId;
			realloc_at(timerAssociations, i);
#if EMSDK
			emsdk_clearTimeout(intervalId);
#endif
			return;
		}
	}
}

void AcSystem::sleep(unsigned int time) {
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void AcSystem::selectLoadFile(const AcString& accept, AcArray<AcFile*>& files, const AcFile::ECode code) {


#if EMSDK

	/* 注入js函数 */
	emsdk_function_init_selectLocalFiles();

	/* 选择文件 */
	if (code == AcFile::ECode::utf8) {
		emsdk_function_selectLocalFiles(accept.constPtr(), "utf8");
	}
	else if (code == AcFile::ECode::binary) {
		emsdk_function_selectLocalFiles(accept.constPtr(), "");
	}

	/* 获得文件状态 */
	const char* result = push_emsdk_variable_fileState();

	/* 解析文件状态 */
	rapidjson::GenericDocument<rapidjson::UTF8<char>>* document = new rapidjson::GenericDocument<rapidjson::UTF8<char>>();
	document->Parse(result);
	for (int i = 0; i < document->GetArray().Size(); i++) {
		AcFile* file = new AcFile();
		rapidjson::GenericObject<false, rapidjson::Value> fileObj = document->GetArray()[i].GetObject();
		for (rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstMemberIterator fileItem = fileObj.begin(); fileItem != fileObj.end(); ++fileItem)
		{
			if (fileItem->name == "name")
			{
				file->name = fileItem->value.GetString();
			}
			else if (fileItem->name == "size")
			{
				file->size = fileItem->value.GetUint();
			}
			else if (fileItem->name == "lastModified")
			{
				file->lastModified = fileItem->value.GetUint64();
			}
		}
		files.append(file);
	}
	delete document;

	delete[]result;

	/* 获取文件内容 */
	for (int i = 0; i < files.length(); i++) {
		files[i]->data = push_emsdk_variable_filedata(i);
	}

#endif

#if WIN32

#endif
}
