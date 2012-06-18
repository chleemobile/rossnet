#include "iostream"
#include "assert.h"
#include <deque>

using namespace std;

class Aircraft
{
public:
	Aircraft();
	virtual ~Aircraft();
	//Aircraft& operator=( Aircraft &src);
	bool operator<(const Aircraft &rhs) const;
	void print_path() const;
	
	static int m_id_generator;

	int 	m_id;
	int 	m_dep_time;
	int 	m_src_airport;
	int		m_src_region;
	int 	m_dest_region;
	int 	m_dest_airport;

	double  m_max_speed;
	double	m_speed;
	double  m_remaining_dist;
	
	//deque<int> m_path;


	//simulaiton time
	double  m_clock;	

	//queueing delay in simulaiton time
	double  m_delay;

	//how many times delay calculated
	int 	m_cdelay;

};
