#include <queue>
#include <vector>
#include <set>

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



template <class T, class Allocator>
inline typename std::vector<T, Allocator>::iterator bs_vector_erase_forward(std::vector<T, Allocator>& v, typename std::vector<T, Allocator>::iterator iter, tw_lp* lp)
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
inline typename std::vector<T>::iterator bs_vector_begin_forward(std::vector<T>& v, tw_lp* lp)
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
