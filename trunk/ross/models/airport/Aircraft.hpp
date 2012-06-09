#include "iostream"
#include "assert.h"

using namespace std;

class Aircraft
{
public:
	Aircraft();
	virtual ~Aircraft();
	Aircraft& operator=( Aircraft &src);

	void set_dest(const int dest);
	void set_wclock(const double now);
	void reset_delays();
	void increase_sdelay();
	void calculate_wdelay(const double now);
	double get_wdelay();
	int get_sdelay();
	int get_id();
	int get_dest();
	
private:
	static int m_id_generator;

	int m_id;
	int m_dest;
	double m_wclock;	
	double m_wdelay;
	int 	m_sdelay;
	

};
