#ifndef BACKSTROKE_RAND
#define BACKSTROKE_RAND

#include <stack>
#include "../ross-types.h"
#include "assert.h"

const int bs_rand_lim = 32749;

static std::stack<int> rn_stack = std::stack<int>();

inline double bs_rand_unif(int& a)
{
    a = (((a * 32719) + 3) % bs_rand_lim);
    return (a+1)  / (double)(bs_rand_lim+1);
}

inline long bs_rand_integer(int& a, long low, long high)
{
	if (high < low)
		return (0);
	else
		return (low + (long)(bs_rand_unif(a) * (high + 1 - low)));
}

inline double bs_rand_exponential(int& a, double Lambda)
{
	return (-Lambda * log(bs_rand_unif(a)));
}

inline double bs_rand_unif2(int& a, tw_lp* lp)
{
    __store__(a, lp);
    //rn_stack.push(a);
    a = (((a * 32719) + 3) % bs_rand_lim);
    return (a+1) / (double)(bs_rand_lim+1);
}

inline void bs_rand_rvs(int& a, tw_lp* lp)
{
    __restore__(a, lp);
    //a = rn_stack.top();
    //rn_stack.pop();
}

inline long bs_rand_integer2(int& a, long low, long high, tw_lp* lp)
{
	if (high < low)
		return (0);
	else
		return (low + (long)(bs_rand_unif2(a, lp) * (high + 1 - low)));
}

inline double bs_rand_exponential2(int& a, double Lambda, tw_lp* lp)
{
	return (-Lambda * log(bs_rand_unif2(a, lp)));
}
//
//
//inline int bs_rand_size_of_stack(){
//    return rn_stack.size();
//}



/*
 store to rn seed to each LP
 */

//inline double bs_rand_unif3(tw_lp *lp, int a)
//{
//    //push(lp, seed);
//    a = (((a * 32719) + 3) % bs_rand_lim);
//    return a / (double)bs_rand_lim;
//}
//
//
//inline void bs_rand_rvs3(tw_lp *lp, int &seed)
//{
//    
//}
//
//inline long bs_rand_integer3(int& a, long low, long high)
//{
//	if (high < low)
//		return (0);
//	else
//		return (low + (long)(bs_rand_unif2(a) * (high + 1 - low)));
//}
//
//inline double bs_rand_exponential3(int& a, double Lambda)
//{
//	return (-Lambda * log(bs_rand_unif2(a)));
//}
//
//inline void push(tw_lp *lp, int seed)
//{
//    rng_seed_node *node = (rng_seed_node*)malloc(sizeof(rng_seed_node));
//    node->seed = seed;
//    
//    if(lp->seed_head == NULL){
//        lp->seed_head = node;
//        lp->seed_tail = node;
//        node->prev = lp->seed_head;
//        node->next = NULL;
//    }
//    else{
//        lp->seed_tail->next = node;
//        node->prev = lp->seed_tail;
//        lp->seed_tail = node;
//    }
//}
//
//inline void pop(tw_lp *lp, int &seed)
//{
//    assert(lp->seed_head != NULL);
//    rng_seed_node *target = lp->seed_tail;
//    lp->seed_tail = target->prev;
//    lp->seed_tail->next = NULL;
//    seed = target->seed;
//    free(target);
//    if(lp->seed_head == lp->seed_tail)
//    {
//        lp->seed_head = NULL;
//        lp->seed_tail = NULL;
//    }
//}

/*
uint32_t xor128(void) {
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;
 
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}
*/

#endif
