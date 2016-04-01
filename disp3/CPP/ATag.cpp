#include "stdafx.h"

#include <math.h>
#include <stdio.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include <Commdlg.h>

#include <ATag.h>

#include <crtdbg.h>


using namespace std;

CATag::CATag()
{
	InitParams();
}
CATag::~CATag()
{
	DestroyObjs();
}

void CATag::InitParams()
{
	m_name = "";
	m_start = 0.0;
	m_end = 0.0;
	m_validflag = 0;
}
void CATag::DestroyObjs()
{

}


int CATag::SetATag(const double srctime, const string srcname)
{
	m_name = srcname;
	m_start = srctime;
	m_end = m_start + 100.0;
	m_validflag = 1;
	return 0;
}

int CATag::SetEndTime(const double srctime)
{
	m_end = srctime;
	return 0;
}

std::string CATag::GetName()
{
	return m_name;
}

int CATag::InTime(const double srctime)
{
	if((srctime >= m_start) && (srctime <= m_end)){
	//if(srctime >= m_start){
		return 1;
	}else{
		return 0;
	}
}

double CATag::GetStart()
{
	return m_start;
}
double CATag::GetEnd()
{
	return m_end;
}
