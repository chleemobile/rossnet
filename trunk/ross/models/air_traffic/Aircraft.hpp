#ifndef __AIRCRAFT__
#define __AIRCRAFT__

class Aircraft
{
private:
    int m_type;
    int m_current_region;
    int m_dest_region;
    int m_dest_airport;
    int m_id;

    /*
     Due to the restriction of Backstroke,
     we can not use arrays (consecutive data structure).
     
     */

    
public:
    Aircraft();
    ~Aircraft();
    static int m_id_generator;
    
    void set_route(int source, int end);
    void update_next_region();
    
    void set_dest_airport(int dest_airport);
    void set_dest_region(int dest_region);
    
    int get_dest_airport();
    int get_dest_region();
    int get_id();
    int get_next_region();

};

#endif