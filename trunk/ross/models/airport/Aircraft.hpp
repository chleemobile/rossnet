#include "iostream"

using namespace std;

class Aircraft
{
public:
	Aircraft();
	virtual ~Aircraft();
	int get_id();

private:
	static int m_id_generator;
	int m_id;
	int m_destination;
	double m_wait_time;
	
};
