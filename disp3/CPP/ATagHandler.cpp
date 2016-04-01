#include "stdafx.h"

#include <math.h>
#include <stdio.h>

#include <coef.h>
#define DBGH
#include <dbg.h>

#include <Commdlg.h>

#include <ATagHandler.h>
#include <ATag.h>

#include <crtdbg.h>

using namespace std;

CATagHandler::CATagHandler()
{
	InitParams();
}

CATagHandler::~CATagHandler()
{
	DestroyObjs();
}

void CATagHandler::InitParams()
{
	m_tagmap.clear();
}

void CATagHandler::DestroyObjs()
{
	map<int, map<string, CATag*>>::iterator itrtagmap;
	for(itrtagmap = m_tagmap.begin(); itrtagmap != m_tagmap.end(); itrtagmap++){
		map<string, CATag*>::iterator itrtag;
		for(itrtag = itrtagmap->second.begin(); itrtag != itrtagmap->second.end(); itrtag++){
			CATag* curtag = itrtag->second;
			if(curtag){
				delete curtag;
			}
		}
	}
	m_tagmap.clear();
}

int CATagHandler::AddTag(const int animno, const double srctime, const double srcendtime, const string srcname)
{
	map<string, CATag*> curtagmap;
	curtagmap = m_tagmap[animno];
	CATag* curtag = curtagmap[srcname];
	if(!curtag){
		curtag = new CATag();
		if(!curtag){
			_ASSERT(0);
			return 1;
		}
	}
	curtag->SetATag(srctime, srcname);
	curtag->SetEndTime(srcendtime);

	curtagmap[srcname] = curtag;

	m_tagmap[animno] = curtagmap;

	return 0;
}
int CATagHandler::SetEndTime(const int animno, const string srcname, const double srcend)
{
	map<string, CATag*> curtagmap;
	curtagmap = m_tagmap[animno];
	if(!curtagmap.empty()){
		CATag* curtag = curtagmap[srcname];
		if(!curtag){
			_ASSERT(0);
			return 1;
		}
		curtag->SetEndTime(srcend);
	}
	m_tagmap[animno] = curtagmap;

	return 0;
}
int CATagHandler::DelTag(const int animno, const string srcname)
{
	map<string, CATag*> curtagmap;
	curtagmap = m_tagmap[animno];
	if(!curtagmap.empty()){
		map<string, CATag*>::iterator itrfind;
		itrfind = curtagmap.find(srcname);
		if(itrfind != curtagmap.end()){
			CATag* deltag = itrfind->second;
			if(deltag){
				delete deltag;
			}
			curtagmap.erase(itrfind);
		}
	}
	m_tagmap[animno] = curtagmap;

	return 0;
}

int CATagHandler::GetCurTags(const int animno, const double srctime, std::vector<CATag>& rettag)
{
	rettag.clear();

	map<string, CATag*> curtagmap;
	curtagmap = m_tagmap[animno];
	if(!curtagmap.empty()){
		map<string, CATag*>::iterator itrtag;
		for(itrtag = curtagmap.begin(); itrtag != curtagmap.end(); itrtag++){
			CATag* curtag = itrtag->second;
			if(curtag){
				int isin = curtag->InTime(srctime);
				if(isin){
					CATag addtag;
					addtag = *curtag;
					rettag.push_back(addtag);
					//::MessageBox( NULL, "getcurtags push_back", "taghandler", MB_OK );
				}
			}
		}
	}

	return 0;
}

unsigned int CATagHandler::GetTagNum(const int animno)
{
	std::map<std::string, CATag*> curtagmap;
	curtagmap = m_tagmap[animno];
	return curtagmap.size();
}

int CATagHandler::GetTag(const int animno, const int srcindex, CATag* dsttag )
{
	std::map<std::string, CATag*> curtagmap;
	curtagmap = m_tagmap[animno];

	std::map<std::string, CATag*>::iterator itrtagmap = curtagmap.begin();
	int tagi;
	for( tagi = 0; tagi < srcindex; tagi++ ){
		if( itrtagmap == curtagmap.end() ){
			_ASSERT( 0 );
			return 1;
		}
		itrtagmap++;
	}

	if( itrtagmap->second ){
		*dsttag = *(itrtagmap->second);
	}else{
		_ASSERT( 0 );
		return 1;
	}

	return 0;
}

