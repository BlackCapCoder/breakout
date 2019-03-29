#ifndef CACHED_VECTOR_H
#define CACHED_VECTOR_H

#include <vector>
#include <functional>
#include <cstdlib>

template <class T>
struct DualCachedVector
{
  private:
    std::vector<T> front       {};
    std::vector<T> back        {};
    std::vector<T> queue_front {};
    std::vector<T> queue_back  {};

  public:
    void push_front (T x) { queue_front.push_back(x); }
    void push_back  (T x) { queue_back .push_back(x); }

    void flush ()
    {
      std::move (queue_back .begin(), queue_back .end(), std::back_inserter(back ));
      std::move (queue_front.begin(), queue_front.end(), std::back_inserter(front));
      queue_back .clear();
      queue_front.clear();
    }

    void filter (std::function<bool(T)> f)
    {
      back .erase(std::remove_if(back .begin(), back .end(), f), back .end());
      front.erase(std::remove_if(front.begin(), front.end(), f), front.end());
    }

    void clear ()
    {
      front.clear();
      back.clear();
      queue_front.clear();
      queue_back.clear();
    }
};


#endif // ifndef CACHED_VECTOR_H
