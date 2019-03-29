#ifndef COLSCENE_H
#define COLSCENE_H


#include <vector>
#include <iostream>

#include "GameObject.h"
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


template <class St>
class ColScene : public ColObj<void, void>
{
  private:
    using Obj  = GameObject <St, bool>;
    using CObj = ColObj     <St, ColResult>;
    using Box  = Either     <Obj, CObj>;

    QuadTree * qt = nullptr;
    DualCachedVector<Box> objs;
    std::vector<CObj*> qtbuf;

    V4 bounds;

  public:
    ColScene (int w, int h) : bounds{V4{0, 0, (double) w, (double) h}}
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
      , void         *
      )
    {
      objs.flush ();
      objs.filter ( [dt, &im, &rend, this] (Box b)
        {
          if (b.isR)
          {
            ColResult r = b.r->logic (dt, im, (St*) this);

            if (r & Remove) {
              qt->remove(b.r);
              return true;
            }

            if (r & BoundsChanged)
              qt->update (b.r);

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

    void addObject (GameObject<ColScene<St>, bool> * obj, bool front = true)
    { addObject((Obj*) obj, front); }

    void addObject (ColObj<ColScene<St>, ColResult> * obj, bool front = true)
    { addObject((CObj*) obj, front); }

    template <template <class ...> class O>
    void addObject (bool front = true) { addObject<O<St>>(front); }


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

#endif // ifndef COLSCENE_H
