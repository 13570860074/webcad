#include "DbDimStyleTableRecord.h"
#include "DbBlockTableRecord.h"
#include "DbBlockTable.h"
#include "CmColor.h"
#include "DbImpl.h"



DbDimStyleTableRecord::DbDimStyleTableRecord()
{
	this->m_pImpl = new DbDimStyleTableRecordImpl();
}
DbDimStyleTableRecord::~DbDimStyleTableRecord()
{

}


Acad::ErrorStatus DbDimStyleTableRecord::getDimpost(ACHAR*& pOutput) const {
	::acutNewString(DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimpost, pOutput);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimapost(ACHAR*& pOutput) const {
	::acutNewString(DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimapost, pOutput);
	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimblk(AcString& sOutput) const {
	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (acdbOpenObject(pBlockTableRecord, DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk) == 0) {
		pBlockTableRecord->getName(sOutput);
		pBlockTableRecord->close();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimblk(ACHAR*& pOutput) const {
	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (acdbOpenObject(pBlockTableRecord, DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk) == 0) {
		pBlockTableRecord->getName(pOutput);
		pBlockTableRecord->close();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimblk1(AcString& sOutput) const {
	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (acdbOpenObject(pBlockTableRecord, DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk1) == 0) {
		pBlockTableRecord->getName(sOutput);
		pBlockTableRecord->close();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimblk1(ACHAR*& pOutput) const {
	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (acdbOpenObject(pBlockTableRecord, DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk1) == 0) {
		pBlockTableRecord->getName(pOutput);
		pBlockTableRecord->close();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimblk2(AcString& sOutput) const {
	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (acdbOpenObject(pBlockTableRecord, DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk2) == 0) {
		pBlockTableRecord->getName(sOutput);
		pBlockTableRecord->close();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::getDimblk2(ACHAR*& pOutput) const {
	DbBlockTableRecord* pBlockTableRecord = NULL;
	if (acdbOpenObject(pBlockTableRecord, DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk2) == 0) {
		pBlockTableRecord->getName(pOutput);
		pBlockTableRecord->close();
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
#if EMSDK
const emstring DbDimStyleTableRecord::emsdk_getDimpost()const {
	ACHAR* sOutput;
	this->getDimpost(sOutput);
	emstring val = sOutput;
	delete sOutput;
	return val;
}
const emstring DbDimStyleTableRecord::emsdk_getDimapost()const {
	ACHAR* sOutput;
	this->getDimapost(sOutput);
	emstring val = sOutput;
	delete sOutput;
	return val;
}
const emstring DbDimStyleTableRecord::emsdk_getDimblk()const {
	AcString sOutput;
	this->getDimblk(sOutput);
	return sOutput.constPtr();
}
const emstring DbDimStyleTableRecord::emsdk_getDimblk1()const {
	AcString sOutput;
	this->getDimblk1(sOutput);
	return sOutput.constPtr();
}
const emstring DbDimStyleTableRecord::emsdk_getDimblk2()const {
	AcString sOutput;
	this->getDimblk2(sOutput);
	return sOutput.constPtr();
}
#endif

int DbDimStyleTableRecord::dimfit() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimatfit;
}
int DbDimStyleTableRecord::dimunit() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlunit;
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimfit(int fit) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimatfit = fit;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimunit(int unit) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaunit = unit;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

bool DbDimStyleTableRecord::isModifiedForRecompute() const {
	return false;
}





Acad::ErrorStatus DbDimStyleTableRecord::dwgInFields(DbDwgFiler* pFiler) {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgInFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}
Acad::ErrorStatus DbDimStyleTableRecord::dwgOutFields(DbDwgFiler* pFiler) const {

	Acad::ErrorStatus es = DbSymbolTableRecord::dwgOutFields(pFiler);
	if (es != Acad::eOk) {
		return (es);
	}


	return Acad::ErrorStatus::eOk;
}





int DbDimStyleTableRecord::dimadec() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimadec;
}
bool DbDimStyleTableRecord::dimalt() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimalt;
}

int DbDimStyleTableRecord::dimaltd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltd;
}
double DbDimStyleTableRecord::dimaltf() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltf;
}
double DbDimStyleTableRecord::dimaltrnd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltrnd;
}
int DbDimStyleTableRecord::dimalttd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimalttd;
}
int DbDimStyleTableRecord::dimalttz() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimalttz;
}
int DbDimStyleTableRecord::dimaltu() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltu;
}
int DbDimStyleTableRecord::dimaltz() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltz;
}
const char* DbDimStyleTableRecord::dimapost() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimapost;
}
int DbDimStyleTableRecord::dimarcsym() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimarcsym;
	return 0;
}
double DbDimStyleTableRecord::dimasz() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimasz;
}
int DbDimStyleTableRecord::dimatfit() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimatfit;
}
int DbDimStyleTableRecord::dimaunit() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaunit;
}
int DbDimStyleTableRecord::dimazin() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimazin;
}
DbObjectId DbDimStyleTableRecord::dimblk() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk;
}
DbObjectId DbDimStyleTableRecord::dimblk1() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk1;
}
DbObjectId DbDimStyleTableRecord::dimblk2() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk2;
}
double DbDimStyleTableRecord::dimcen() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimcen;
}
CmColor DbDimStyleTableRecord::dimclrd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimclrd;
}
CmColor DbDimStyleTableRecord::dimclre() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimclre;
}
CmColor DbDimStyleTableRecord::dimclrt() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimclrt;
}
int DbDimStyleTableRecord::dimdec() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdec;
}
double DbDimStyleTableRecord::dimdle() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdle;
}
double DbDimStyleTableRecord::dimdli() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdli;
}
char DbDimStyleTableRecord::dimdsep() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdsep;
}
double DbDimStyleTableRecord::dimexe() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimexe;
}
double DbDimStyleTableRecord::dimexo() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimexo;
}
int DbDimStyleTableRecord::dimfrac() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimfrac;
}
double DbDimStyleTableRecord::dimgap() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimgap;
}
double DbDimStyleTableRecord::dimjogang() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimjogang;
	return 0.0;
}
int DbDimStyleTableRecord::dimjust() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimjust;
}
DbObjectId DbDimStyleTableRecord::dimldrblk() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimldrblk;
}
double DbDimStyleTableRecord::dimlfac() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlfac;
}
bool DbDimStyleTableRecord::dimlim() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlim;
}
DbObjectId DbDimStyleTableRecord::dimltex1() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimltex1;
	return NULL;
}
DbObjectId DbDimStyleTableRecord::dimltex2() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimltex2;
	return NULL;
}
DbObjectId DbDimStyleTableRecord::dimltype() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimltype;
	return NULL;
}
int DbDimStyleTableRecord::dimlunit() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlunit;
}
Db::LineWeight DbDimStyleTableRecord::dimlwd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlwd;
}
Db::LineWeight DbDimStyleTableRecord::dimlwe() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlwe;
}
const char* DbDimStyleTableRecord::dimpost() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimpost;
}
double DbDimStyleTableRecord::dimrnd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimrnd;
}
bool DbDimStyleTableRecord::dimsah() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsah;
}
double DbDimStyleTableRecord::dimscale() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimscale;
}
bool DbDimStyleTableRecord::dimsd1() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsd1;
}
bool DbDimStyleTableRecord::dimsd2() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsd2;
}
bool DbDimStyleTableRecord::dimse1() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimse1;
}
bool DbDimStyleTableRecord::dimse2() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimse2;
}
bool DbDimStyleTableRecord::dimsoxd() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsoxd;
}
int DbDimStyleTableRecord::dimtad() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtad;
}
int DbDimStyleTableRecord::dimtdec() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtdec;
}
double DbDimStyleTableRecord::dimtfac() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtfac;
}
int DbDimStyleTableRecord::dimtfill() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtfill;
	return 0;
}
CmColor DbDimStyleTableRecord::dimtfillclr() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtfillclr;
	return CmColor();
}
bool DbDimStyleTableRecord::dimtih() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtih;
}
bool DbDimStyleTableRecord::dimtix() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtix;
}
double DbDimStyleTableRecord::dimtm() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtm;
}
int DbDimStyleTableRecord::dimtmove() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtmove;
}
bool DbDimStyleTableRecord::dimtofl() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtofl;
}
bool DbDimStyleTableRecord::dimtoh() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtoh;
}
bool DbDimStyleTableRecord::dimtol() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtol;
}
int DbDimStyleTableRecord::dimtolj() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtolj;
}
double DbDimStyleTableRecord::dimtp() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtp;
}
double DbDimStyleTableRecord::dimtsz() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtsz;
}
double DbDimStyleTableRecord::dimtvp() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtvp;
}
DbObjectId DbDimStyleTableRecord::dimtxsty() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtxsty;
}
double DbDimStyleTableRecord::dimtxt() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtxt;
}
int DbDimStyleTableRecord::dimtzin() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtzin;
}
bool DbDimStyleTableRecord::dimupt() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimupt;
}
int DbDimStyleTableRecord::dimzin() const {
	return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimzin;
}

bool DbDimStyleTableRecord::dimfxlenOn() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimfxlenOn;
	return false;
}
double DbDimStyleTableRecord::dimfxlen() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimfxlen;
	return 0.0;
}

bool DbDimStyleTableRecord::dimtxtdirection() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtxtdirection;
	return false;
}
double DbDimStyleTableRecord::dimmzf() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimmzf;
	return 0.0;
}
const char* DbDimStyleTableRecord::dimmzs() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimmzs;
	return "";
}
double DbDimStyleTableRecord::dimaltmzf() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltmzf;
	return 0.0;
}
const char* DbDimStyleTableRecord::dimaltmzs() const {
	//return DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltmzs;
	return "";
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimadec(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimadec = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimalt(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimalt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltd(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltf(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltf = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltmzf(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltmzf = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltmzs(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltmzs = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltrnd(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltrnd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimalttd(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimalttd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimalttz(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimalttz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltu(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltu = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaltz(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaltz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimapost(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimapost = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimarcsym(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimarcsym = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimasz(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimasz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimatfit(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimatfit = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimaunit(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimaunit = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimazin(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimazin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimblk(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimblk1(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimblk2(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimcen(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimcen = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimclrd(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimclrd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimclre(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimclre = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimclrt(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimclrt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimdec(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdec = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimdle(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdle = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimdli(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdli = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimdsep(char  v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimdsep = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimexe(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimexe = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimexo(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimexo = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimfrac(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimfrac = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimgap(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimgap = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimjogang(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimjogang = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimjust(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimjust = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimldrblk(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimldrblk = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimlfac(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlfac = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimlim(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlim = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimltex1(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimltex1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimltex2(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimltex2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimltype(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimltype = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimlunit(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlunit = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimlwd(Db::LineWeight v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlwd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimlwe(Db::LineWeight v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimlwe = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimmzf(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimmzf = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimmzs(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimmzs = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimpost(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimpost = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimrnd(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimrnd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimsah(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsah = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimscale(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimscale = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimsd1(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsd1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimsd2(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsd2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimse1(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimse1 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimse2(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimse2 = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimsoxd(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimsoxd = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtad(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtad = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtdec(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtdec = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtfac(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtfac = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtfill(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtfill = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtfillclr(const CmColor& v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtfillclr = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtih(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtih = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtix(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtix = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtm(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtm = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtmove(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtmove = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtofl(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtofl = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtoh(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtoh = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtol(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtol = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtolj(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtolj = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtp(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtp = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtsz(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtsz = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtvp(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtvp = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtxsty(DbObjectId v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtxsty = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtxt(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtxt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtxtdirection(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtxtdirection = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimtzin(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimtzin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimupt(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimupt = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}
Acad::ErrorStatus DbDimStyleTableRecord::setDimzin(int v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimzin = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimblk(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimblk1(const char* v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk1 = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimblk2(const char* v) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimblk2 = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimldrblk(const char* v) {

	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		if (this->database() != NULL) {

			DbBlockTable* pBlockTable = NULL;
			this->database()->getBlockTable(pBlockTable);

			if (pBlockTable != NULL) {
				DbObjectId objectId = NULL;
				pBlockTable->getAt(v, objectId);
				if (objectId != NULL) {
					DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimldrblk = objectId;
				}
				pBlockTable->close();
			}
		}
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


Acad::ErrorStatus DbDimStyleTableRecord::setDimfxlenOn(bool v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimfxlenOn = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}

Acad::ErrorStatus DbDimStyleTableRecord::setDimfxlen(double v) {
	if (this->isWriteEnabled() == true || this->isNotifyEnabled() == true) {
		//DB_IMP_DIMSTYLETABLERECORD(this->m_pImpl)->dimfxlen = v;
		return Acad::ErrorStatus::eOk;
	}
	return Acad::ErrorStatus::eFail;
}


