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

#define BS_STACK std::deque<std::pair<tw_stime, boost::any> >


//#define MEM_USAGE 


static int counter = 0;


struct BSStack
{    
    BS_STACK general_stack;

    BSStack() : index(counter++){} 
    int index;
};


static int memusage = 0;
static int store_operation = 0;
static int restore_operation = 0;


    template<class T>
inline void __store__(const T obj, tw_lp* lp)
{
#ifdef MEM_USAGE
    memusage += sizeof(obj);
    store_operation ++;
#endif

    BSStack* stk = static_cast<BSStack*>(lp->stack_pointer);
    stk->general_stack.push_back(std::make_pair(tw_now(lp), boost::any(obj)));
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

// Pass the GVT to the second parameter.
inline void clearStack(tw_stime t, tw_lp* lp)
{
    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    while (!stk->general_stack.empty())
    {
        if (stk->general_stack.front().first < t)
            stk->general_stack.pop_front();
        else
            return;
    }
}

inline void bs_fossil_collect(tw_stime GVT, tw_lp *lp)
{
	clearStack(GVT, lp);
}


#endif


