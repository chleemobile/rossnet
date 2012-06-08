#include "iostream"

using namespace std;

class Aircraft
{
public:
	Aircraft();
	virtual ~Aircraft();
	int get_id();
	int get_dest();
	void set_dest(const int dest);
	Aircraft& operator=( Aircraft &src);

private:
	static int m_id_generator;
	int m_id;
	int m_dest;
	double m_delay_time;
	
};
