#ifndef ATAGHANDLERH
#define ATAGHANDLERH

#include <map>
#include <string>
#include <vector>

class CATag;

class CATagHandler
{
public:
	CATagHandler();
	~CATagHandler();

	int AddTag(const int animno, const double srctime, const double srcendtime, const std::string srcname);
	int SetEndTime(const int animno, const std::string srcname, const double srcend);
	int DelTag(const int animno, const std::string srcname);

	int GetCurTags(const int animno, const double srctime, std::vector<CATag>& rettag);
	unsigned int GetTagNum(const int animno);
	int GetTag(const int animno, const int srcindex, CATag* dsttag );

	int ClearAnim(const int animno);
private:
	void InitParams();
	void DestroyObjs();

private:
	std::map<int, std::map<std::string, CATag*>> m_tagmap;//<animno, <tagname, CATag*>>

};

#endif
