// A scene that supports collision

#ifndef COLSCENE_H
#define COLSCENE_H

#include <vector>
#include <iostream>

#include "GameObject.h"
#include "CachedVector.h"
#include "QuadTree.h"
#include "Either.h"


template <class R, class ... S>
struct ColObj : public GameObject<R,S...>, public Collidable<ColObj<R,S...>>
{
public:
  virtual void onHit (S...) {}
};

enum ColResult
{ None          = 0,
  BoundsChanged = 1,
  Remove        = 2,
};


template <class St>
class ColScene : public Scene
{
private:
  using Obj  = GameObject <bool, St*>;
  using CObj = ColObj     <ColResult, St*>;
  using Box  = Either     <Obj, CObj>;

  DualCachedVector<Box> objs;
  std::vector<CObj*> qtbuf;

  static constexpr int qtCap = 200;
  static constexpr int qtLvl = 4;

public:
  QuadTree<CObj> qt;

  ColScene (InitArgs args)
    : qt { V4 {0, 0, (double) args.w, (double) args.h}, qtCap, qtLvl}
  {
    qtbuf.reserve (qtCap);
  }

  V4   getBounds () const { return qt.getBounds(); }
  int  getWidth  () const { return getBounds().w; }
  int  getHeight () const { return getBounds().h; }

  SceneR tick (const TickArgsS args)
  {
    LogicArgs<St*> largs {args.dt(), args.im(), (St*) this};

    objs.flush ();
    objs.filter ([&largs, &rend=args.r, &qt=qt] (Box b)
      {
        if (b.isR)
        {
          ColResult r = b.r->logic (largs);

          if (r & Remove) {
            qt.remove(b.r);
            return true;
          }

          if (r & BoundsChanged)
            qt.update (b.r);

          b.r->render (rend);
          return false;
        }
        else
        {
          if (b.l->logic (largs))
            return true;
          else {
            b.l->render (rend);
            return false;
          }
        }
      });

    return false;
  }

  void addObject (Box b, bool front = true)
  {
    if (front)
      objs.push_front(b);
    else
      objs.push_back(b);

    if (b.isR)
      qt.insert(b.r);
  }

  void addObject (Obj  & obj, bool front = true) { addObject (Box{&obj}, front); }
  void addObject (CObj & obj, bool front = true) { addObject (Box{&obj}, front); }

  template <class O, typename... Args>
  O & addObject (bool front = true, Args... args)
  {
    O & o = *new O (args...);
    addObject (o, front);
    return o;
  }

  void addObject (GameObject<bool, ColScene<St>*> & obj, bool front = true)
  { addObject((Obj&) obj, front); }

  void addObject (ColObj<ColResult, ColScene<St>*> & obj, bool front = true)
  { addObject((CObj&) obj, front); }

  template <template <class ...> class O>
  void addObject (bool front = true) { addObject<O<St>>(front); }


  void clear ()
  {
    objs.clear ();
    qt.clear  ();
  }

  inline
  std::vector<CObj*> & getObjectsInBound (const V4 & bound)
  {
    qtbuf.clear();
    qt.getObjectsInBound (bound, qtbuf);
    return qtbuf;
  }

};

#endif // ifndef COLSCENE_H
