// A scene that supports collision

#ifndef COLSCENE_H
#define COLSCENE_H

#include <vector>
#include <iostream>

#include "GameObject.h"
#include "CachedVector.h"
#include "QuadTree.h"
#include "Either.h"


template <class S, class R>
struct ColObj : public GameObject<S,R>, public Collidable
{
public:
  virtual void onHit (S *) {}
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
  using Obj  = GameObject <St, bool>;
  using CObj = ColObj     <St, ColResult>;
  using Box  = Either     <Obj, CObj>;

  DualCachedVector<Box> objs;
  std::vector<CObj*> qtbuf;

  ResourceManager & rm;
  SDL_Renderer    * rend;

public:
  QuadTree qt;

  ColScene
    ( const int w
    , const int h
    , ResourceManager & rm
    , SDL_Renderer    * rend
    )
    : qt{QuadTree{V4 {0, 0, (double) w, (double) h}, 100, 4}}
    , rm{rm}
    , rend{rend}
  {}

  V4   getBounds () const { return qt.getBounds(); }
  int  getWidth  () const { return getBounds().w; }
  int  getHeight () const { return getBounds().h; }

  SceneR tick
    ( double               dt
    , SDL_Renderer       * rend
    , const InputManager & im
    , SceneS             *
    )
  {
    objs.flush ();
    objs.filter ( [dt, &im, &rend, this] (Box b)
      {
        if (b.isR)
        {
          ColResult r = b.r->logic (dt, im, (St*) this);

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
          if (b.l->logic (dt, im, (St*) this))
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

    b.l->init (rm, rend);
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

  void addObject (GameObject<ColScene<St>, bool> & obj, bool front = true)
  { addObject((Obj&) obj, front); }

  void addObject (ColObj<ColScene<St>, ColResult> & obj, bool front = true)
  { addObject((CObj&) obj, front); }

  template <template <class ...> class O>
  void addObject (bool front = true) { addObject<O<St>>(front); }


  void removeObject (Box needle)
  {
    objs.filter([&needle] (Box x) { return x.l == needle.l; });
  }
  void removeObject (Obj * needle)
  {
    removeObject(Box(needle));
  }
  void removeObject (CObj * needle)
  {
    removeObject(Box(needle));
    qt.remove(needle);
  }


  void clear ()
  {
    objs.clear ();
    qt.clear  ();
  }

  std::vector<CObj*> & getObjectsInBound (const V4 & bound)
  {
    std::vector<Collidable*> & cs = qt.getObjectsInBound(bound);
    // return (std::vector<CObj*> &) cs;
    return qtbuf = std::vector<CObj*>{cs.begin(), cs.end()};
  }
};

#endif // ifndef COLSCENE_H
