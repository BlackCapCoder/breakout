// A scene that supports collision

#ifndef COLSCENE_H
#define COLSCENE_H

#include <iostream>
#include <list>
#include <vector>
#include <type_traits>
#include <functional>
#include <utility>

#include "GameObject.h"
#include "Cached.h"
#include "QuadTree.h"
#include "Virt.h"
#include "temp.h"


enum ColResult
{ None          = 0,
  BoundsChanged = 1,
  Remove        = 2,
};

constexpr unsigned ONHIT = 4;

template <class...S>
using HasOnHit = VIRT<ONHIT, void(S...)>;

// A game object that supports collision
template <class...S>
struct ColObj : public GameObject<ColResult,S...>, public Collidable<ColObj<S...>>, public HasOnHit<S...>
{
  void operator()(const ProxyIX<ONHIT>, S...) {}

  inline void onHit (S...s) {
    (*this)(ProxyIX<ONHIT>{}, s...);
  }
};





// A scene that supports collision
template <unsigned layers, class St, bool multipass = true>
class ColScene : public Scene
{
private:

  // A game object may or may not support collision
  using Obj  = GameObject <bool, St>;
  using CObj = ColObj     <St>;

  // Wrap them in a common interface
  struct GADT
    : std::function<bool(QuadTree<CObj> &, const TickArgs<St>, const bool fst)>
  {
    template <class T>
    constexpr GADT (T * o)
      : std::function<bool(QuadTree<CObj> &, const TickArgs<St>, const bool fst)>
      { [&obj=*o] (QuadTree<CObj> & qt, const TickArgs<St> args, const bool fst) constexpr -> bool
          {
            if (!multipass || fst) {
              if constexpr (std::is_base_of<HasLogic<bool, St>, T>::value) {
                if (logic(obj, args.l())) {
                  args.dirty = true;
                  return true;
                }
              } else if constexpr (std::is_base_of<CObj, T>::value) {
                const ColResult r = logic(obj, args.l());
                if (r & Remove) {
                  args.dirty = true;
                  qt.remove(&obj);
                  return true;
                }
                if (r & BoundsChanged) {
                  args.dirty = true;
                  V4 & bo = obj.getBounds();
                  qt.update(&obj);
                }
              }
            }

            if (!multipass || !fst) {
              if constexpr (std::is_base_of<HasRender, T>::value) {
                render(obj, args.r());
              }
            }

            return false;
          }
      }
    {}
  };


  // We support storing objects on different layers. The lower
  // an object's layer is, the sooner it will receive the logic/render tick.

  // Also, we might add objects in the middle of a tick and we don't
  // want those to be rendered, so we keep a queue.
  CachedN
    < std::vector
    , layers
    , GADT
    > objs;

  bool inTick = false;


  QuadTree<CObj>        qt;
  std::vector<CObj*> qtbuf;

  static constexpr int      qtCap     = 200;
  static constexpr int      qtLvl     = 4;
  static constexpr unsigned LayerSize = 1;


protected:
  ColScene (InitArgs args)
    : qt { V4 {0, 0, (double) args.w, (double) args.h}, qtCap, qtLvl }
  {
    qtbuf.reserve (qtCap);
  }

  template <class T>
  inline void updateQuadtree (T obj) { qt.update (obj); }

  inline void clear ()
  {
    objs.clear ();
    qt.clear   ();
  }

  inline void tickChildren (const TickArgs<St> args, const bool fst = true)
  {
    if (!multipass || fst)
    {
      inTick = true;
      objs.flush  ();
      objs.filter ([&args, &qt=qt] (GADT g) constexpr { return g (qt, args, true); });
      inTick = false;
    }
    if constexpr (multipass) if (!fst) {
      objs.filter ([&args, &qt=qt] (GADT g) constexpr { return g (qt, args, false); });
    }
  }


public:

  template <unsigned L, class T> void insert (T * obj) {
    if (inTick)
      std::get <0 + L*LayerSize> (objs.vs) . insert (GADT{obj});
    else
      std::get <0 + L*LayerSize> (objs.vs) . skip (GADT{obj});

    if constexpr (std::is_base_of<CObj, T>::value)
      qt.insert(obj);
  }

  inline V4  getBounds () const { return qt.getBounds(); }
  inline int getWidth  () const { return getBounds().w; }
  inline int getHeight () const { return getBounds().h; }

  inline
  std::vector<CObj*> & getObjectsInBound (const V4 & bound)
  {
    qtbuf.clear ();
    qt.getObjectsInBound (bound, qtbuf);
    return qtbuf;
  }
};



#endif // ifndef COLSCENE_H
