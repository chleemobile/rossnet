#ifndef reverse_rctypes_h
#define reverse_rctypes_h

#include <stdlib.h>
#include <assert.h>
#include <deque>
#include <queue>
#include <stack>
#include "ross-types.h"
#include <boost/any.hpp>

#include <iostream>
using std::cout;

#define __BACKSTROKE


#define MEM_USAGE 1
#define DEF_STACK(T) std::stack<T> T##_stack; 

#define DEF_GET_STACK(T) \
template<> struct StackType<T> { typedef std::stack<T> type; }; \
template<> inline StackType<T>::type& data_stack<T>(BSStack *stk) { return stk->T##_stack; } \
template<> inline T get_top<T>(const std::stack<T>& stk) { return stk.top(); } 

typedef unsigned char BS_UCHAR;
typedef unsigned short BS_USHORT;
typedef unsigned int BS_UINT;
typedef unsigned long BS_ULONG;
typedef boost::any BS_ANY;



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

    std::stack<boost::any> general_stack;
    //DEF_STACK(BS_ANY)
    
    BSStack() : index(counter++){} 
    int index;
};

template <class T>
struct StackType
{
    typedef std::stack<boost::any> type;
};

    

//static std::deque<boost::any> data_stack;
template<class T> 
inline typename StackType<T>::type& data_stack(BSStack *stk) 
{
    return stk->general_stack;
    //assert(false);
    //return std::stack<T>();
}


template<class T> 
inline T get_top(const std::stack<T>& stk) 
{
    assert(false);
    return T();
}

template<class T> 
inline T get_top(const std::stack<boost::any>& stk) 
{
    return boost::any_cast<T>(stk.top());
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
static int store_operation = 0;
static int restore_operation = 0;



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
    T val = get_top<T>(data_stack<T>(stk));
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
    if(MEM_USAGE)
    {
       memusage += sizeof(obj);
       store_operation ++;
    }

    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    push<T>(obj, stk);
}

template<class T>
inline void __restore__(T& obj, tw_lp* lp)
{
    if(MEM_USAGE)
    {
        restore_operation++;
    }
    BSStack* stk  = static_cast<BSStack*>(lp->stack_pointer);
    obj = pop<T>(stk);
}

#endif

