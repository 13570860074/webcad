#include "acutads.h"
#include "adscodes.h"
#include "acutmem.h"
#include <stdarg.h>
#include "Db.h"

int acutWcMatch(const ACHAR *string, const ACHAR *pattern)
{
	if (AcString::isEqual(std::string(string), std::string(pattern)) == true)
	{
		return RTNORM;
	}
	return RTERROR;
}

int acutRbDup(const struct resbuf *source, struct resbuf **result)
{
	(*result)->rbnext = NULL;
	(*result)->restype = source->restype;
	(*result)->resval = source->resval;
	if (source->rbnext != NULL)
	{
		(*result)->rbnext = new resbuf();
		acutRbDup(source->rbnext, &(*result)->rbnext);
	}
	return RTNORM;
}

resbuf *acutNewRb(int v)
{
	resbuf *variable = new resbuf();
	variable->restype = v;
	variable->rbnext = NULL;
	return variable;
}

int acutRelRb(struct resbuf *rb)
{
	if (rb->rbnext != NULL)
	{
		acutRelRb(rb->rbnext);
		rb->rbnext = NULL;
	}
	delete rb;
	rb = NULL;

	return RTNORM;
}

resbuf *acutBuildList(int rtype, ...)
{

	resbuf *variable = new resbuf();
	variable->rbnext = NULL;
	variable->restype = rtype;
	resbuf *rbnext = variable;

	va_list args;
	va_start(args, rtype);

	int type = rtype;
	while (true)
	{
		if (type == RTSTR || type == Db::kDwgText ||  (type >= Db::kDxfXdAsciiString && type <= Db::kDxfXdHandle) || type == Db::kDxfXTextString)
		{
			char *str = va_arg(args, char *);
			::acutNewString(str, rbnext->resval.rstring);
		}
		else if (
			type == RTREAL || type == Db::kDwgReal || type == Db::kDxfReal || type == Db::kDxfXReal || type == Db::kDxfXdReal)
		{
			double read = va_arg(args, double);
			rbnext->resval.rreal = read;
		}
		else if (
			type == RTSHORT || type == Db::kDwgInt16 || type == Db::kDxfInt16 || type == Db::kDxfXInt16 ||
			type == Db::kDxfXdInteger16 || type == Db::kDxfXXInt16 || type == Db::kDxfXXXInt16)
		{
			short val = va_arg(args, short);
			rbnext->resval.rint = val;
		}
		else if (
			type == RTLONG || type == Db::kDwgInt32 || type == Db::kDxfInt32 || type == Db::kDxfXdInteger32)
		{
			int val = va_arg(args, int);
			rbnext->resval.rlong = val;
		}
		else if (
			type == RTPOINT || type == Db::kDwg3Real || type == Db::kDxfXCoord)
		{
			ads_real *val = va_arg(args, ads_real *);
			rbnext->resval.rpoint[0] = val[0];
			rbnext->resval.rpoint[1] = val[1];
			rbnext->resval.rpoint[2] = val[2];
		}


		type = va_arg(args, int);
		if (type == 0 || type == RTNONE)
		{
			break;
		}
		rbnext->rbnext = new resbuf();
		rbnext = rbnext->rbnext;
		rbnext->restype = type;
	}

	va_end(args);

	return variable;
}

int acutIsAlpha(int c)
{
	if (ispunct(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsUpper(int c)
{
	if (isupper(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsLower(int c)
{
	if (islower(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsDigit(int c)
{
	if (isdigit(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsXDigit(int c)
{
	if (isxdigit(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsSpace(int c)
{
	if (isspace(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsPunct(int c)
{
	if (ispunct(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsAlNum(int c)
{
	if (isalnum(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsPrint(int c)
{
	if (isprint(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsGraph(int c)
{
	if (isgraph(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutIsCntrl(int c)
{
	if (iscntrl(c))
	{
		return RTNORM;
	}
	return RTERROR;
}
int acutToUpper(int c)
{
	return toupper(c);
}
int acutToLower(int c)
{
	return tolower(c);
}