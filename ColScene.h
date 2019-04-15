// A scene that supports collision

#ifndef COLSCENE_H
#define COLSCENE_H

#include <list>
#include <vector>
#include <type_traits>

#include "GameObject.h"
#include "Cached.h"
#include "QuadTree.h"
#include "Virt.h"


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
template <unsigned layers, class St>
class ColScene : public Scene
{
private:

  // This class can hold two different kinds of objects
  using Obj  = GameObject <bool, St>;
  using CObj = ColObj     <St>;

  // And supports storing them on different layers. The lower
  // an object's layer is, the sooner it will receive the logic/render tick.

  // And by the way, I might add objects in the middle of ticks and I don't
  // want those to be rendered!
  CachedN
    < std::vector
    , layers
    // , Obj,   CObj
    , Obj*,  CObj*
    > objs;

  QuadTree<CObj>        qt;
  std::vector<CObj*> qtbuf;

  static constexpr int      qtCap     = 200;
  static constexpr int      qtLvl     = 4;
  static constexpr unsigned LayerSize = 2;

  bool inTick = false;


protected:
  ColScene (InitArgs args)
    : qt { V4 {0, 0, (double) args.w, (double) args.h}, qtCap, qtLvl }
  {
    qtbuf.reserve (qtCap);
  }

  template <class T>
  inline void updateQuadtree (T obj) { qt.update (obj); }

  inline
  void clear ()
  {
    objs.clear ();
    qt.clear   ();
  }

  void tickChildren (const TickArgs<St> args)
  {
    inTick = true;
    objs.flush  ();
    objs.filter ([&args, &qt=qt](auto c) constexpr {
      using C = decltype(c);
      if constexpr (std::is_same<C, Obj>::value || std::is_same<C, Obj&>::value) {
        if (logic(c, args.l())) return true;
        else render(c, args.r());
      } else if constexpr (std::is_same<C, Obj*>::value) {
        if (logic(*c, args.l())) return true;
        else render(*c, args.r());
      } else if constexpr (std::is_same<C, CObj>::value || std::is_same<C, CObj&>::value) {
        ColResult r = logic(c, args.l());
        if (r & Remove) {
          qt.remove(&c);
          return true;
        }
        if (r & BoundsChanged) qt.update(&c);
        render(c, args.r());
      } else if constexpr (std::is_same<C, CObj*>::value) {
        ColResult r = logic(*c, args.l());
        if (r & Remove) {
          qt.remove(c);
          return true;
        }
        if (r & BoundsChanged) qt.update(c);
        render(*c, args.r());
      }
      return false;
    });
    inTick = false;
  }


public:

  template <unsigned L> void insert (Obj * obj) {
    if (inTick)
      std::get <0 + L*LayerSize> (objs.vs) . insert (obj);
    else
      std::get <0 + L*LayerSize> (objs.vs) . skip (obj);
  }

  template <unsigned L> void insert (CObj * obj) {
    if (inTick)
      std::get <1 + L*LayerSize> (objs.vs) . insert (obj);
    else
      std::get <1 + L*LayerSize> (objs.vs) . skip (obj);

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
