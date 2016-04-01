#ifndef ATAGH
#define ATAGH

#include <string>

class CATag
{
public:
	CATag();
	~CATag();

	int SetATag(const double srctime, const std::string srcname);
	int SetEndTime(const double srctime);

	int InTime(const double srctime);

	std::string GetName();
	double GetStart();
	double GetEnd();

private:
	void InitParams();
	void DestroyObjs();

private:
	int m_validflag;
	std::string m_name;
	double m_start;
	double m_end;
};

#endif
