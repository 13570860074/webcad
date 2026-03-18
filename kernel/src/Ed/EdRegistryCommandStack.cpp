#include "EdRegistryCommandStack.h"
#include "EdCommandIterator.h"
#include "EdRegistryCommand.h"
#include "EdRegistryCommandIterator.h"
#include "EditorReactorManager.h"
#include "GiWorldDrawManager.h"
#include "ApWorkDocument.h"
#include "arxHeaders.h"
#include "AcDebug.h"
#include "kernel.h"
#include "EdImpl.h"
#include "Ed.h"



EdRegistryCommandStack::EdRegistryCommandStack() {
	this->m_pImpl = new EdRegistryCommandStackImpl();
}
EdRegistryCommandStack::~EdRegistryCommandStack() {

}


bool EdRegistryCommandStack::isCommand() {
	return RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->isCommand;
}

// 命令开始前移除夹点
// 命令完成后取消选择集
Acad::ErrorStatus EdRegistryCommandStack::sendCommand(const AcString& str) {

	// 获得命令
	AcString command;
	if (str.length() > 1) {
		command = str.slice(0, str.length() - 1);
	}
	else {
		if (RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->command.length() > 0) {
			command = RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->command;
		}
		else {
			return Acad::ErrorStatus::eFail;
		}
	}

	// 获得全局命令以及函数
	AcString globalName = "";
	RxFunctionPtr function = NULL;
	int commandFlags = ACRX_CMD_MODAL;
	EdCommand* groupCommand = NULL;
	EdCommandIterator* pCommandIterator = this->iterator();
	for (; !pCommandIterator->done(); pCommandIterator->next()) {
		groupCommand = EdCommand::cast(pCommandIterator->command());
		if (command.isEqual(groupCommand->localName(), command.string(), true) == true ||
			command.isEqual(groupCommand->globalName(), command.string(), true) == true) {
			if (groupCommand->functionAddr() != NULL) {
				globalName = groupCommand->globalName();
				function = groupCommand->functionAddr();
				commandFlags = groupCommand->commandFlags();
			}
			break;
		}
	}
	delete pCommandIterator;

	// 设置命令提示
	AcString prompt;
	prompt.format("\n命令: %s", command.constPtr());
	//acedPrompt(prompt.constPtr());

	// 如果未找到命令或者设置了未定义状态
	if (globalName == "" || function == NULL || int(commandFlags & ACRX_CMD_UNDEFINED) == ACRX_CMD_UNDEFINED) {
		prompt.format("\n未知命令“%s”。", command.constPtr());
		//acedPrompt(prompt.constPtr());
		return Acad::ErrorStatus::eFail;
	}

	// 触发命令使用将要开始事件
	if (groupCommand != NULL) {
		for (int i = 0; i < RX_IMP_COMMAND(groupCommand->m_pImpl)->reactor.length(); i++) {
			RX_IMP_COMMAND(groupCommand->m_pImpl)->reactor[i]->commandWillStart(groupCommand, NULL);
		}
	}
	AcArray<EdCommandStackReactor*> commandStackReactors = RX_IMP_COMMANDSTACK(acedRegCmds->m_pImpl)->reactors;
	for (int i = 0; i < commandStackReactors.length(); i++) {
		commandStackReactors[i]->commandWillStart(groupCommand, NULL);
	}

	// 触发编辑反应器的命令使用开始事件
	AcArray<RxEventReactor*> editorReactors = *&ED_IMP_EDITTORREACTORMANAGER(EditorReactorManager::cast(acedEditor)->m_pImpl)->reactors;
	for (int i = 0; i < editorReactors.length(); i++) {
		EditorReactor* pReactor = EditorReactor::cast(editorReactors[i]);
		if (pReactor != NULL) {
			pReactor->commandWillStart(globalName.constPtr());
		}
	}

	// 设置正在使用命令中
	RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->isCommand = true;

	// 执行命令
	function();

	// 设置上一个命令
	if (int(commandFlags & ACRX_CMD_NOHISTORY) != ACRX_CMD_NOHISTORY) {
		RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->command = globalName;
	}

	// 设置命令使用完成
	RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->isCommand = false;

	// 设置文档被修改
	if (int(commandFlags & ACRX_CMD_NO_UNDO_MARKER) != ACRX_CMD_NO_UNDO_MARKER) {
		if (ApWorkDocument::cast(acDocManager->curDocument()) != NULL) {
			ApWorkDocument::cast(acDocManager->curDocument())->setModified(true);
		}
	}

	// 触发命令使用完成事件
	if (groupCommand != NULL) {
		for (int i = 0; i < RX_IMP_COMMAND(groupCommand->m_pImpl)->reactor.length(); i++) {
			RX_IMP_COMMAND(groupCommand->m_pImpl)->reactor[i]->commandEnded(groupCommand, NULL);
		}
	}
	for (int i = 0; i < commandStackReactors.length(); i++) {
		commandStackReactors[i]->commandEnded(groupCommand, NULL);
	}

	// 触发编辑反应器的命令使用完成事件
	for (int i = 0; i < editorReactors.length(); i++) {
		EditorReactor* pReactor = EditorReactor::cast(editorReactors[i]);
		if (pReactor != NULL) {
			pReactor->commandEnded(globalName.constPtr());
		}
	}

	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus EdRegistryCommandStack::addCommand(
	const ACHAR* cmdGroupName,
	const ACHAR* cmdGlobalName,
	const ACHAR* cmdLocalName,
	Adesk::Int32 commandFlags,
	int FunctionAddr,
	void* UIContext,
	int fcode,
	void* resourceInstance,
	EdCommand** cmdPtrRet) {

	EdRegistryCommand* command = new EdRegistryCommand();
	command->groupName(cmdGroupName);
	command->globalName(cmdGlobalName);
	command->localName(cmdLocalName);
	command->commandFlags(commandFlags);
	command->functionAddr((RxFunctionPtr)FunctionAddr);
	command->functionCode(fcode);
	RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->append(command);
	cmdPtrRet = (EdCommand**)command;
	for (int i = 0; i < RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->reactors.length(); i++) {
		RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->reactors[i]->commandAdded(command, NULL);
	}
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus EdRegistryCommandStack::addCommand(const ACHAR* cmdGroupName,
	const ACHAR* cmdGlobalName,
	const ACHAR* cmdLocalName,
	Adesk::Int32 commandFlags,
	RxFunctionPtr FunctionAddr,
	void* UIContext,
	int fcode,
	void* resourceInstance,
	EdCommand** cmdPtrRet) {

	EdRegistryCommand* command = new EdRegistryCommand();
	command->groupName(cmdGroupName);
	command->globalName(cmdGlobalName);
	command->localName(cmdLocalName);
	command->commandFlags(commandFlags);
	command->functionAddr(FunctionAddr);
	command->functionCode(fcode);
	RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->append(command);
	cmdPtrRet = (EdCommand**)command;
	for (int i = 0; i < RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->reactors.length(); i++) {
		RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->reactors[i]->commandAdded(command, NULL);
	}
	return Acad::ErrorStatus::eOk;
}

EdCommandIterator* EdRegistryCommandStack::iterator() {
	EdRegistryCommandIterator* pRegistryCommandIterator = new EdRegistryCommandIterator();
	RX_IMP_REGISTRYCOMMANDITERATOR(pRegistryCommandIterator->m_pImpl)->pRecords = RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands;
	return pRegistryCommandIterator;
}

EdCommand* EdRegistryCommandStack::lookupGlobalCmd(const ACHAR* cmdName) {
	for (int i = 0; i < RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->length(); i++) {
		if (RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->at(i)->globalName() == cmdName) {
			return RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->at(i);
		}
	}
	return NULL;
}
EdCommand* EdRegistryCommandStack::lookupLocalCmd(const ACHAR* cmdName) {
	for (int i = 0; i < RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->length(); i++) {
		if (RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->at(i)->localName() == cmdName) {
			return RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->at(i);
		}
	}
	return NULL;
}
Acad::ErrorStatus EdRegistryCommandStack::removeGroup(const ACHAR* groupName) {
	for (int i = RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->length() - 1; i >= 0; i--) {
		EdRegistryCommand* command = EdRegistryCommand::cast(RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->at(i));
		if (command != NULL && command->groupName() == groupName) {
			for (int u = 0; u < RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->reactors.length(); u++) {
				RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->reactors[u]->commandRemoved(command, NULL);
			}
			RX_IMP_REGISTRYCOMMANDSTACK(this->m_pImpl)->pCommands->removeAt(i--);
		}
	}
	return  Acad::ErrorStatus::eOk;
}
