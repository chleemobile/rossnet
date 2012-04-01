#ifndef reverse_rctypes_h
#define reverse_rctypes_h

#include <stdlib.h>
#include <assert.h>
#include <malloc/malloc.h>
#include <deque>
#include <queue>
#include <boost/any.hpp>
#include "ross-types.h"

#include <iostream>
using std::cout;

#define __BACKSTROKE



#define DEF_STACK(T) std::stack<T> T##_stack; 

#define DEF_GET_STACK(T) \
template<> std::stack<T>& data_stack<T>(BSStack *stk) { return stk->T##_stack; }

typedef unsigned char BS_UCHAR;
typedef unsigned short BS_USHORT;
typedef unsigned int BS_UINT;
typedef unsigned long BS_ULONG;



static int counter = 0;

struct BSStack
{    
    DEF_STACK(char)
    DEF_STACK(short)
    DEF_STACK(int)
    DEF_STACK(long)
    DEF_STACK(float)
    DEF_STACK(double)    
    
    DEF_STACK(BS_UCHAR)
    DEF_STACK(BS_USHORT)
    DEF_STACK(BS_UINT)
    DEF_STACK(BS_ULONG)
    
    BSStack() : index(counter++){} 
    int index;
};

//static std::deque<boost::any> data_stack;
template<class T> std::stack<T>& data_stack(BSStack *stk) 
{
    assert(false);
    return std::stack<T>();
}

DEF_GET_STACK(char)
DEF_GET_STACK(short)
DEF_GET_STACK(int)
DEF_GET_STACK(long)
DEF_GET_STACK(float)
DEF_GET_STACK(double)

DEF_GET_STACK(BS_UCHAR)
DEF_GET_STACK(BS_USHORT)
DEF_GET_STACK(BS_UINT)
DEF_GET_STACK(BS_ULONG)

//static std::stack<int> int_stack;

static int memusage = 0;


/*
 //! Pop from the front of a stack. Used in fossil collection
 template <class T>
 inline void pop_front()
 {
 data_stack<T>().pop_front();
 }
 */

template <class T>
inline T push(T val, BSStack* stk)
{
    data_stack<T>(stk).push(val);
    
    //cout << typeid(data_stack<T>(stk)).name() << '\n';
    //cout << "##########" << stk->index << " " << data_stack<T>(stk).size() << "\n";
    return val;
}

template <class T>
inline T pop(BSStack* stk)
{
    //cout << typeid(data_stack<T>(stk)).name() << '\n';
    //cout << "@@@@@@@@@@"  << stk->index << " " << data_stack<T>(stk).size() << "\n";
    assert(!data_stack<T>(stk).empty());
    T val = data_stack<T>(stk).top();
    data_stack<T>(stk).pop();
    return val;
}

/*
 inline void pop()
 {
 data_stack().pop_back();
 }
 */    


template<class T>
inline void __store__(T obj)
{
    //memusage += sizeof(obj);
    push(obj);
}

template<class T>
inline void __restore__(T& obj)
{
    obj = pop<T>();
}


template<class T>
inline void __store__(T obj, tw_lp* lp)
{
    //cout<<sizeof(obj)<<endl;
    //memusage += sizeof(obj);
    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    push<T>(obj, stk);
}

template<class T>
inline void __restore__(T& obj, tw_lp* lp)
{
    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    obj = pop<T>(stk);
}

#endif
