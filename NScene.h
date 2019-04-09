#ifndef NSCENE_H
#define NSCENE_H

#include "GameObject.h"
#include "Product.h"


template <typename ...Scenes>
struct NScene : public GameObject<SceneR>
{
private:
  Product<Scenes...> scenes;
  unsigned           cursor;

private:
  inline Scene & getActive () noexcept
  {
    return scenes.get (cursor % sizeof...(Scenes), (Proxy<Scene>*) NULL);
  }

public:
  template <class ... Params>
  NScene<Scenes...> (const Params... ps) noexcept
    : scenes { Product<Scenes...>{ps...} }
    , cursor { 0 }
  {
  }

  inline
  void render (const RenderArgs args) noexcept override
  {
    getActive () . render (args);
  }

  inline
  SceneR logic (const LogicArgsS args) noexcept override
  {
    if (getActive () . logic (args)) toggle ();
    return false;
  }

  inline
  SceneR tick (const TickArgsS args) noexcept override
  {
    if (getActive () . tick (args)) toggle ();
    return false;
  }

  inline
  void toggle () noexcept
  {
    cursor++;
  }
};

#endif
