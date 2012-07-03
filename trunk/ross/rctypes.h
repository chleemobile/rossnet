#ifndef reverse_rctypes_h
#define reverse_rctypes_h

#include <stdlib.h>
#include <assert.h>
#include <deque>
#include <queue>
#include <stack>
#include <vector>
#include "ross-types.h"
#include <boost/any.hpp>

#include <iostream>
using std::cout;

#define __BACKSTROKE

#define BS_GENERIC_STACK std::deque<std::pair<tw_stime, boost::any> >
#define BS_INT_STACK std::deque<std::pair<tw_stime, int> >
#define BS_DOUBLE_STACK std::deque<std::pair<tw_stime, double> >

#define BS_GENEROC_ITR std::deque<std::pair<tw_stime, boost::any> >::iterator
#define BS_INT_ITR std::deque<std::pair<tw_stime, int> >::iterator
#define BS_DOUBLE_ITR std::deque<std::pair<tw_stime, double> >::iterator

//#define MEM_USAGE 


static int counter = 0;


struct BSStack
{    
    BS_GENERIC_STACK 	general_stack;
    BS_INT_STACK		int_stack;
	BS_DOUBLE_STACK 	double_stack;

    BSStack() : index(counter++){} 
    int index;
};


static int memusage = 0;
static int store_operation = 0;
static int restore_operation = 0;


template<class T>
inline void __store__(const T &obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    memusage += sizeof(obj);
    store_operation ++;
#endif

    BSStack* stk = static_cast<BSStack*>(lp->stack_pointer);
    stk->general_stack.push_back(std::make_pair(tw_now(lp), boost::any(obj)));
}

inline void __store__(const int obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    memusage += sizeof(obj);
    store_operation ++;
#endif

    BSStack* stk = static_cast<BSStack*>(lp->stack_pointer);
    stk->int_stack.push_back(std::make_pair(tw_now(lp), obj));
}

inline void __store__(const double obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    memusage += sizeof(obj);
    store_operation ++;
#endif

    BSStack* stk = static_cast<BSStack*>(lp->stack_pointer);
    stk->double_stack.push_back(std::make_pair(tw_now(lp), obj));
}


template<class T>
inline void __restore__(T& obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    restore_operation++;
#endif

    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    assert(!stk->general_stack.empty());
    obj = boost::any_cast<T>(stk->general_stack.back().second);
    stk->general_stack.pop_back();
}

inline void __restore__(int& obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    restore_operation++;
#endif

    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    assert(!stk->int_stack.empty());
    obj = stk->int_stack.back().second;
    stk->int_stack.pop_back();
}

inline void __restore__(double& obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    restore_operation++;
#endif

    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    assert(!stk->double_stack.empty());
    obj = stk->double_stack.back().second;
    stk->double_stack.pop_back();
}


// Pass the GVT to the second parameter.
inline void clearStack(tw_stime t, tw_lp *lp)
{
    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);

	//cout<<"size of general stack before clear:"<<stk->general_stack.size()<<endl;
	BS_GENEROC_ITR generic_itr;
    for(generic_itr = stk->general_stack.begin(); generic_itr != stk->general_stack.end();)
    {
        if (stk->general_stack.front().first < t)
            generic_itr = stk->general_stack.erase(generic_itr);
        else
            ++generic_itr;
    }
	//cout<<"size of general stack after clear:"<<stk->general_stack.size()<<endl;

	//cout<<"size of int stack before clear:"<<stk->int_stack.size()<<endl;
	BS_INT_ITR int_itr;
    for(int_itr = stk->int_stack.begin(); int_itr != stk->int_stack.end();)
    {
        if (stk->int_stack.front().first < t)
            int_itr = stk->int_stack.erase(int_itr);
        else
            ++int_itr;
    }

	//cout<<"size of int stack after clear:"<<stk->int_stack.size()<<endl;

	//cout<<"size of double stack before clear:"<<stk->double_stack.size()<<endl;
	BS_DOUBLE_ITR double_itr;
    for(double_itr = stk->double_stack.begin(); double_itr != stk->double_stack.end();)
    {
        if (stk->double_stack.front().first < t)
            double_itr = stk->double_stack.erase(double_itr);
        else
            ++double_itr;
    }
	//cout<<"size of double stack after clear:"<<stk->double_stack.size()<<endl;
	
}

inline void bs_fossil_collect(tw_stime GVT, tw_lp *lp)
{
	clearStack(GVT, lp);
}


#endif


