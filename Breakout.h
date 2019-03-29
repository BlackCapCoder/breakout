#ifndef BREAKOUT_H
#define BREAKOUT_H


#include <vector>
#include <iostream>

#include "GameObject.h"
#include "QuadTree.h"
#include "CachedVector.h"
#include "Either.h"


template <class S, class R>
struct ColObj : public GameObject<S,R>, public Collidable
{
  public:
    virtual void onHit (S *) {}
};

enum ColResult {
  None          = 0,
  BoundsChanged = 1,
  Remove        = 2,
};


class Breakout : public ColObj<void, void>
{
  private:
    using St   = Breakout;
    using Obj  = GameObject <St, bool>;
    using CObj = ColObj     <St, ColResult>;
    using Box  = Either     <Obj, CObj>;

    QuadTree * qt = nullptr;
    DualCachedVector<Box> objs;
    std::vector<CObj*> qtbuf;

    V4 bounds;

  public:
    Breakout (int w, int h) : bounds{V4{0, 0, (double) w, (double) h}}
    {
      this->qt = new QuadTree (bounds, 100, 4);
    }

    V4 * getBounds () { return &bounds; }
    int  getWidth  () { return bounds.w; }
    int  getHeight () { return bounds.h; }

    void tick
      ( double dt
      , SDL_Renderer * rend
      , InputManager * im
      , void *
      )
    {
      objs.flush ();
      objs.filter ( [dt, &im, &rend, this] (Box b)
        {
          if (b.isR)
          {
            CObj * c = b.r;
            ColResult r = c->logic (dt, im, this);

            if (r & Remove) {
              qt->remove(c);
              return true;
            }

            if (r & BoundsChanged)
              qt->update (c);

            c->render (rend);
            return false;
          }
          else
          {
            Obj * obj = b.l;
            if (obj->logic (dt, im, this))
              return true;
            else {
              obj->render (rend);
              return false;
            }
          }
        });
    }

    void addObject (Box b, bool front = true)
    {
      if (front)
        objs.push_front(b);
      else
        objs.push_back(b);

      if (b.isR)
        qt->insert(b.r);
    }
    void addObject (Obj  * obj, bool front = true) { addObject (Box{obj}, front); }
    void addObject (CObj * obj, bool front = true) { addObject (Box{obj}, front); }
    template <class O> void addObject (bool front = true) { addObject(new O(), front); }

    void clear ()
    {
      objs.clear ();
      qt->clear  ();
    }

    std::vector<CObj*> &getObjectsInBound(const V4 &bound)
    {
      std::vector<Collidable*> cs = qt->getObjectsInBound(bound);
      return qtbuf = std::vector<CObj*>{cs.begin(), cs.end()};
    }

};

#endif // ifndef BREAKOUT_H
