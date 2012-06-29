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

#define BS_STACK std::deque


#define MEM_USAGE 1
#define DEF_STACK(T) BS_STACK<T> T##_stack; 

#define DEF_GET_STACK(T) \
template<> struct StackType<T> { typedef BS_STACK<T> type; }; \
template<> inline StackType<T>::type& data_stack<T>(BSStack *stk) { return stk->T##_stack; } \
template<> inline T get_top<T>(const BS_STACK<T>& stk) { return stk.back(); } 

typedef unsigned char BS_UCHAR;
typedef unsigned short BS_USHORT;
typedef unsigned int BS_UINT;
typedef unsigned long BS_ULONG;
typedef boost::any BS_ANY;



static int counter = 0;

struct BSStackPosition
{
    BS_STACK<int>::iterator intPos;
    BS_STACK<float>::iterator floatPos;
    BS_STACK<boost::any>::iterator anyPos;
};


struct BSStack
{    
    //DEF_STACK(char)
    //DEF_STACK(short)
    DEF_STACK(int)
    //DEF_STACK(long)
    DEF_STACK(float)
    //DEF_STACK(double)    
    
    //DEF_STACK(BS_UCHAR)
    //DEF_STACK(BS_USHORT)
    //DEF_STACK(BS_UINT)
    //DEF_STACK(BS_ULONG)

    BS_STACK<boost::any> general_stack;
    //DEF_STACK(BS_ANY)
    
    BSStack() : index(counter++){} 
    int index;
};

template <class T>
struct StackType
{
    typedef BS_STACK<boost::any> type;
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
inline T get_top(const BS_STACK<T>& stk) 
{
    assert(false);
    return T();
}

template<class T> 
inline T get_top(const BS_STACK<boost::any>& stk) 
{
    return boost::any_cast<T>(stk.back());
}



//DEF_GET_STACK(char)
//DEF_GET_STACK(short)
DEF_GET_STACK(int)
//DEF_GET_STACK(long)
DEF_GET_STACK(float)
//DEF_GET_STACK(double)

//DEF_GET_STACK(BS_UCHAR)
//DEF_GET_STACK(BS_USHORT)
//DEF_GET_STACK(BS_UINT)
//DEF_GET_STACK(BS_ULONG)




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
    data_stack<T>(stk).push_back(val);
    
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
    data_stack<T>(stk).pop_back();
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


/**/

inline BSStackPosition* getStackPosition(tw_lp* lp)
{
    BSStackPosition* pos = new BSStackPosition;
    BSStack* p = (BSStack*)lp->stack_pointer;
    pos->intPos = p->int_stack.end();
    pos->floatPos = p->float_stack.end();
    pos->anyPos = p->general_stack.end();
    return pos;
}

inline void deleteStackPosition(void* stackPosition)
{
    BSStackPosition* pos = (BSStackPosition*)stackPosition;
    delete pos;
}

inline void clearStack(tw_lp* lp, void* stackPosition)
{
    BSStackPosition* pos = (BSStackPosition*)stackPosition;
    BSStack* p = (BSStack*)lp->stack_pointer;

	cout<<"before clear size of stacks "<<p->int_stack.size()<<","<<p->float_stack.size()<<","<<p->general_stack.size()<<endl;

    p->int_stack.erase(p->int_stack.begin(), pos->intPos);

	p->float_stack.erase(p->float_stack.begin(), pos->floatPos);
	
    p->general_stack.erase(p->general_stack.begin(), pos->anyPos);
	
	cout<<"after clear size of stacks "<<p->int_stack.size()<<","<<p->float_stack.size()<<","<<p->general_stack.size()<<endl;
	
}


typedef struct gvt_pair gvt_pair;

struct gvt_pair
{
	gvt_pair(tw_stime in_GVT, BSStackPosition *in_pos):ts(in_GVT), pos(in_pos) {}
	~gvt_pair(){ }

	tw_stime ts;
	BSStackPosition *pos;
};

inline vector<gvt_pair*>* get_gvt_vector()
{
	vector<gvt_pair*> *ret_v = new vector<gvt_pair*>();
	return ret_v;
}

inline void bs_record_stack_position(tw_stime GVT, tw_lp *lp)
{
	BSStackPosition *pos = getStackPosition(lp);
	gvt_pair *pair = new gvt_pair(GVT, pos);
	vector<gvt_pair*> *vtr = static_cast<vector<gvt_pair*>*>(lp->vector_pointer);
	vtr->push_back(pair);
}

inline void bs_fossil_collect(tw_stime GVT, tw_lp *lp)
{
	tw_stime min_difference = 99999999;
	tw_stime next_max_GVT = -1;
	BSStackPosition *delete_pos = NULL;

	vector<gvt_pair*> *vtr = static_cast<vector<gvt_pair*>*>(lp->vector_pointer);	
	
	if(vtr->size() > 0)
	{
		for(vector<gvt_pair*>::iterator itr = vtr->begin(); itr != vtr->end();itr++)
		{
			tw_stime iTS = (*itr)->ts;
			BSStackPosition *pos = (*itr)->pos;

			if(iTS < GVT)
			{
				tw_stime difference = GVT - iTS;
				assert(difference >= 0);

				if(difference <= min_difference)
				{
					min_difference = difference;
					next_max_GVT = iTS;
					delete_pos =  (*itr)->pos;
				}

				/*

				 Will implement deallocation

				 */
				//deleteStackPosition(pos);
				//itr = vtr->erase(itr);
			}
			else
			{
				//++itr;
			}
		}

		
		if(delete_pos != NULL)
		{
			clearStack(lp, delete_pos);
		}
		
	}

	//cout<<"vector size"<<vector_size<<endl;


	cout<<lp->gid<<", "<<vtr->size()<<", "<<next_max_GVT<<", "<<GVT<<endl;
}
	

	



#endif

