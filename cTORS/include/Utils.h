#pragma once
#include <iostream>
#include <vector>

#ifndef DEBUG
#define DEBUG 0
#endif // !DEBUG
#ifndef debug_out
#define debug_out(s) \
if(DEBUG) { cout << s << endl; }
#endif

#ifndef DELETE_LIST
#define DELETE_LIST(l) l.remove_if([](auto e) { delete e; return true; });
#endif

#ifndef DELETE_VECTOR
#define DELETE_VECTOR(l) l.erase(remove_if(l.begin(), l.end(), [](auto e) {delete e; return true;}), l.end());
#endif

template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

template<typename T>
std::vector<const T*> copy_of(const std::vector<const T*>& objs) {
    std::vector<const T*> res(objs.size());
    for(size_t i=0; i<objs.size(); i++)
        res[i] = new T(*objs[i]);
    return res;
}

template<class T>
inline void hash_combine(std::size_t& s, const T& v)
{
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

template <class T>
struct std::hash<std::pair<T, T>> {
    std::size_t operator() (const std::pair<T, T> &pair) const {
        auto x = std::hash<T>{}(pair.first);
        hash_combine(x, pair.second);
        return x;
    }
};
