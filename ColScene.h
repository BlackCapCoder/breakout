// A scene that supports collision

#ifndef COLSCENE_H
#define COLSCENE_H

#include <vector>
#include <iostream>

#include "GameObject.h"
#include "Cached.h"
#include "QuadTree.h"
#include "Either.h"


enum ColResult
{ None          = 0,
  BoundsChanged = 1,
  Remove        = 2,
};

template <class...S>
struct ColObj : public GameObject<ColResult,S...>, public Collidable<ColObj<S...>>
{
  virtual void onHit (S...) {}
};


template <class St>
class ColScene : public Scene
{
private:
  using Obj  = GameObject <bool, St>;
  using CObj = ColObj     <St>;
  using Box  = Either     <Obj*, CObj*>;

  Cached<std::vector, Box, Box> objs;
  std::vector<CObj*> qtbuf;

  static constexpr int qtCap = 200;
  static constexpr int qtLvl = 4;

public:
  QuadTree<CObj> qt;

  template <class ... Params>
  ColScene (InitArgs args, Params ... ps)
    : qt   { V4 {0, 0, (double) args.w, (double) args.h}, qtCap, qtLvl }
    , objs { ps... }
  {
    qtbuf.reserve (qtCap);
  }

  inline V4  getBounds () const { return qt.getBounds(); }
  inline int getWidth  () const { return getBounds().w; }
  inline int getHeight () const { return getBounds().h; }

  void tickChildren (const TickArgs<St> args)
  {
    objs.flush  ();
    objs.filter ([&args, &qt=qt] (Box b)
      {
        if (b.isR)
        {
          ColResult r = b.r->logic (args.l);

          if (r & Remove) {
            qt.remove(b.r);
            return true;
          }

          if (r & BoundsChanged)
            qt.update (b.r);

          b.r->render (args.r);
          return false;
        }
        else
        {
          if (b.l->logic (args.l))
            return true;
          else {
            b.l->render (args.r);
            return false;
          }
        }
      });
  }

  void addObject (Box b, bool front = true)
  {
    if (front)
      std::get<1>(objs.vs).insert(b);
    else
      std::get<0>(objs.vs).insert(b);

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

  void addObject (ColObj<ColScene<St>*> & obj, bool front = true)
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
