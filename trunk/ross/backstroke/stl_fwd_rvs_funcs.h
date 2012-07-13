#include <queue>

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
