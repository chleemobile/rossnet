#include <queue>
#include <vector>
#include <set>
#include <list>
#include <deque>




/******************************************************************************************************/
// Begin of priority queue


template <class T, class Sequence, class Compare>
inline void bs_priority_queue_pop_forward(std::priority_queue<T, Sequence, Compare>& q, tw_lp* lp)
{
    __store__(q.top(), lp);
    q.pop();
}

template <class T, class Sequence, class Compare>
inline void bs_priority_queue_pop_reverse(std::priority_queue<T, Sequence, Compare>& q, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    q.push(t);
}

template <class T, class Sequence, class Compare>
inline void bs_priority_queue_push_forward(std::priority_queue<T, Sequence, Compare>& q, const T& obj, tw_lp* lp)
{
    __store__(obj, lp);
    q.push(obj);
}

template <class T, class Sequence, class Compare>
inline void bs_priority_queue_push_reverse(std::priority_queue<T, Sequence, Compare>& q, tw_lp* lp)
{
    T obj;
    __restore__(obj, lp);
    std::vector<T> vec;
    Compare comp;
    while (!q.empty())
    {
        T t = q.top();
        q.pop();
        if (comp(obj, t)) 
            vec.push_back(t);
        else
            break;
    }

    for (size_t i = 0, s = vec.size(); i < s; ++i)
        q.push(vec[i]);
}


// End of priority queue
/******************************************************************************************************/




/******************************************************************************************************/
// Begin of vector


template <class T>
inline void bs_vector_push_back_forward(std::vector<T>& v, const T& t, tw_lp* lp)
{
    v.push_back(t);
}
    
template <class T>
inline void bs_vector_push_back_reverse(std::vector<T>& v, tw_lp* lp)
{
    v.pop_back();
}



template <class T>
inline void bs_vector_pop_back_forward(std::vector<T>& v, tw_lp* lp)
{
    __store__(v.back(), lp);
    v.pop_back();
}
    
template <class T>
inline void bs_vector_pop_back_reverse(std::vector<T>& v, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    v.push_back(t);
}



template <class T>
inline typename std::vector<T>::iterator 
bs_vector_erase_forward(std::vector<T>& v, typename std::vector<T>::iterator iter, tw_lp* lp)
{
    size_t pos = iter - v.begin();
    __store__(pos, lp);
    __store__(*iter, lp);
    return v.erase(iter);
}
    
template <class T>
inline void bs_vector_erase_reverse(std::vector<T>& v, tw_lp* lp)
{
    size_t pos;
    T t;
    __restore__(t, lp);
    __restore__(pos, lp);
    v.insert(v.begin() + pos, t);
}


template <class T>
inline typename std::vector<T>::iterator 
bs_vector_insert_forward(std::vector<T>& v, typename std::vector<T>::iterator iter, const T& t, tw_lp* lp)
{
    size_t pos = iter - v.begin();
    __store__(pos, lp);
    return v.insert(iter, t);
}
    
template <class T>
inline void bs_vector_insert_reverse(std::vector<T>& v, tw_lp* lp)
{
    size_t pos;
    __restore__(pos, lp);
    v.erase(v.begin() + pos);
}


template <class T>
inline typename std::vector<T>::iterator 
bs_vector_begin_forward(std::vector<T>& v, tw_lp* lp)
{
    return v.begin();
}
    
template <class T>
inline void bs_vector_begin_reverse(std::vector<T>& v, tw_lp* lp)
{
}


template <class T>
inline typename std::vector<T>::iterator bs_vector_end_forward(std::vector<T>& v, tw_lp* lp)
{
    return v.end();
}
    
template <class T>
inline void bs_vector_end_reverse(std::vector<T>& v, tw_lp* lp)
{
}


// End of vector
/******************************************************************************************************/




/******************************************************************************************************/
// Begin of deque


template <class T>
inline void bs_deque_push_back_forward(std::deque<T>& v, const T& t, tw_lp* lp)
{
    v.push_back(t);
}
    
template <class T>
inline void bs_deque_push_back_reverse(std::deque<T>& v, tw_lp* lp)
{
    v.pop_back();
}



template <class T>
inline void bs_deque_pop_back_forward(std::deque<T>& v, tw_lp* lp)
{
    __store__(v.back(), lp);
    v.pop_back();
}
    
template <class T>
inline void bs_deque_pop_back_reverse(std::deque<T>& v, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    v.push_back(t);
}


template <class T>
inline void bs_deque_push_front_forward(std::deque<T>& v, const T& t, tw_lp* lp)
{
    v.push_front(t);
}
    
template <class T>
inline void bs_deque_push_front_reverse(std::deque<T>& v, tw_lp* lp)
{
    v.pop_front();
}



template <class T>
inline void bs_deque_pop_front_forward(std::deque<T>& v, tw_lp* lp)
{
    __store__(v.front(), lp);
    v.pop_front();
}
    
template <class T>
inline void bs_deque_pop_front_reverse(std::deque<T>& v, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    v.push_front(t);
}


template <class T>
inline typename std::deque<T>::iterator 
bs_deque_erase_forward(std::deque<T>& v, typename std::deque<T>::iterator iter, tw_lp* lp)
{
    size_t pos = iter - v.begin();
    __store__(pos, lp);
    __store__(*iter, lp);
    return v.erase(iter);
}
    
template <class T>
inline void bs_deque_erase_reverse(std::deque<T>& v, tw_lp* lp)
{
    size_t pos;
    T t;
    __restore__(t, lp);
    __restore__(pos, lp);
    v.insert(v.begin() + pos, t);
}


template <class T>
inline typename std::deque<T>::iterator 
bs_deque_insert_forward(std::deque<T>& v, typename std::deque<T>::iterator iter, const T& t, tw_lp* lp)
{
    size_t pos = iter - v.begin();
    __store__(pos, lp);
    return v.insert(iter, t);
}
    
template <class T>
inline void bs_deque_insert_reverse(std::deque<T>& v, tw_lp* lp)
{
    size_t pos;
    __restore__(pos, lp);
    v.erase(v.begin() + pos);
}


template <class T>
inline typename std::deque<T>::iterator 
bs_deque_begin_forward(std::deque<T>& v, tw_lp* lp)
{
    return v.begin();
}
    
template <class T>
inline void bs_deque_begin_reverse(std::deque<T>& v, tw_lp* lp)
{
}


template <class T>
inline typename std::deque<T>::iterator 
bs_deque_end_forward(std::deque<T>& v, tw_lp* lp)
{
    return v.end();
}
    
template <class T>
inline void bs_deque_end_reverse(std::deque<T>& v, tw_lp* lp)
{
}


// End of deque
/******************************************************************************************************/




/******************************************************************************************************/
// Begin of list


template <class T>
inline void bs_list_push_back_forward(std::list<T>& v, const T& t, tw_lp* lp)
{
    v.push_back(t);
}
    
template <class T>
inline void bs_list_push_back_reverse(std::list<T>& v, tw_lp* lp)
{
    v.pop_back();
}



template <class T>
inline void bs_list_pop_back_forward(std::list<T>& v, tw_lp* lp)
{
    __store__(v.back(), lp);
    v.pop_back();
}
    
template <class T>
inline void bs_list_pop_back_reverse(std::list<T>& v, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    v.push_back(t);
}


template <class T>
inline void bs_list_push_front_forward(std::list<T>& v, const T& t, tw_lp* lp)
{
    v.push_front(t);
}
    
template <class T>
inline void bs_list_push_front_reverse(std::list<T>& v, tw_lp* lp)
{
    v.pop_front();
}



template <class T>
inline void bs_list_pop_front_forward(std::list<T>& v, tw_lp* lp)
{
    __store__(v.front(), lp);
    v.pop_front();
}
    
template <class T>
inline void bs_list_pop_front_reverse(std::list<T>& v, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    v.push_front(t);
}


template <class T>
inline typename std::list<T>::iterator 
bs_list_erase_forward(std::list<T>& v, typename std::list<T>::iterator iter, tw_lp* lp)
{
    __store__(iter + 1, lp);
    __store__(*iter, lp);
    return v.erase(iter);
}
    
template <class T>
inline void bs_list_erase_reverse(std::list<T>& v, tw_lp* lp)
{
    typename std::list<T>::iterator iter;
    T t;
    __restore__(t, lp);
    __restore__(iter, lp);
    v.insert(iter, t);
}


template <class T>
inline typename std::list<T>::iterator 
bs_list_insert_forward(std::list<T>& v, typename std::list<T>::iterator iter, const T& t, tw_lp* lp)
{
    typename std::list<T>::iterator newIter = v.insert(iter, t);
    __store__(newIter, lp);
    return newIter;
}
    
template <class T>
inline void bs_list_insert_reverse(std::list<T>& v, tw_lp* lp)
{
    typename std::list<T>::iterator iter;
    __restore__(iter, lp);
    v.erase(iter);
}


template <class T>
inline typename std::list<T>::iterator 
bs_list_begin_forward(std::list<T>& v, tw_lp* lp)
{
    return v.begin();
}
    
template <class T>
inline void bs_list_begin_reverse(std::list<T>& v, tw_lp* lp)
{
}


template <class T>
inline typename std::list<T>::iterator 
bs_list_end_forward(std::list<T>& v, tw_lp* lp)
{
    return v.end();
}
    
template <class T>
inline void bs_list_end_reverse(std::list<T>& v, tw_lp* lp)
{
}


// End of list
/******************************************************************************************************/





/******************************************************************************************************/
// Begin of multiset


template <class T>
inline typename std::multiset<T>::iterator 
bs_multiset_erase_forward(std::multiset<T>& v, typename std::multiset<T>::iterator iter, tw_lp* lp)
{
    __store__(*iter, lp);
    return v.erase(iter);
}
    
template <class T>
inline void bs_multiset_erase_reverse(std::multiset<T>& v, tw_lp* lp)
{
    T t;
    __restore__(t, lp);
    v.insert(t);
}


template <class T>
inline typename std::list<T>::iterator 
bs_multiset_insert_forward(std::multiset<T>& v, const T& t, tw_lp* lp)
{
    typename std::multiset<T>::iterator newIter = v.insert(t);
    __store__(newIter, lp);
    return newIter;
}
    
template <class T>
inline void bs_multiset_insert_reverse(std::multiset<T>& v, tw_lp* lp)
{
    typename std::multiset<T>::iterator iter;
    __restore__(iter, lp);
    v.erase(iter);
}


template <class T>
inline typename std::multiset<T>::iterator 
bs_multiset_begin_forward(std::multiset<T>& v, tw_lp* lp)
{
    return v.begin();
}
    
template <class T>
inline void bs_multiset_begin_reverse(std::multiset<T>& v, tw_lp* lp)
{
}


template <class T>
inline typename std::multiset<T>::iterator 
bs_multiset_end_forward(std::multiset<T>& v, tw_lp* lp)
{
    return v.end();
}
    
template <class T>
inline void bs_multiset_end_reverse(std::multiset<T>& v, tw_lp* lp)
{
}


// End of multiset
/******************************************************************************************************/



