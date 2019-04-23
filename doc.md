# Overview


### Game

`Game` is the main class. It initializes all the SDL bullshit and has a single method `loop`. `loop`
takes a single `Scene` and ticks it repeatedly.


### Scene

Something that has a method `TickArgs<> -> bool`. The return value indicates whether the scene is over.


### Zipper

There are 3 scenes: `MainMenu`, `Breakout` and `HighScore`, but by design, `Game` can only play a single one.


`Zipper<T<B..>, F, Xs..>` is isomorphic to `tuple<F, Xs..>`. The zipper inherits from all classes `B..`,
overriding virtual methods, which when called will be forwarded to the element `F`, which is said to be the focus.
If the call returns a boolean `true` the Zipper will focus the next element,
resulting in a Zipper isomorphic to `Zipper<T<B..>, Xs.., F>`.

```c++
ZipperS <Xs..> == Zipper <T<Scene>, Xs..>
```


### GameObject

A `GameObject` is something that has a `logic` and a `render` method. `logic + render` is identical to
`Scene.tick`. The separation of `GameObject` and `Scene` was done so that `logic` and `render` can be done
in two separate passes, potentially skipping `render`, which is by far the most expensive method.


Any GameObject can be made a scene:
```c++
struct WrappedGameObject : public Scene
{
  GameObject <SceneR> obj;

  SceneR operator::() (ProxyIX<TICK>, TickArgsS args) override
  {
    SceneR out = obj.logic  ( args.l() );
    obj.render ( args.r() );
    return out;
  }
}
```


### ColObj

A `GameObject` that supports collision.


### ColScene

`ColScene` is a `Scene` and also a homogeneous container of GameObjects. It will keep track of any
`ColObj`ects inserted into it in a `QuadTree`, and exposes a method for querying this.


It supports storing gameobjects on different layers. An object is guaranteed to receive the tick later
than all other objects on lower layers. This is useful for rendering.


`ColScene` also guarantees that objects inserted during a tick will not be included in the same tick.


### ResouceManager

Some scenes/objects needs to load resources (font/image/audio) from disk. We don't want different objects
to load their own copies, so everything is piped through the `ResourceManager`.



# Weird Design Decisions


### Proxies


Several places I take proxy types as arguments:

```c++
template <class T> struct Proxy {};
```

C++ only does type-inference for function arguments, which means that if I were to write the function:
```c++
template <class R> R foo () {}
```

Then invocation of `foo` would be ambiguous- I would have to call it as `foo<blah>()`, which doesn't sound too
bad until you start start doing type level programming:

```c++
// I'm not sure if this even works..
(static_cast<VIRT<TICK, LogicR>&>(x) .* VIRT<TICK, LogicR>::template operator()<TICK>) (args);

// vs
x (Proxy<TICK> {}, args);
```


The reason it is `struct Proxy {}` rather than `struct Proxy;` is that `foo (Proxy<T> {})` is optimized away by the compiler,
while `foo ((Proxy<T>*) nullptr)` is not. The former is a zero-cost abstraction!


I have no idea if this is common practice in c++, but we used to do this in Haskell before type application.



### Type level strings


Used for audio:
```c++
const ResourceManager::Sound
  < str ("resources/Explosion1.wav")
  , str ("resources/Explosion2.wav")
  , str ("resources/Explosion3.wav")
  , str ("resources/Explosion4.wav")
  > audioExplosion;
```

The alternative would be:
```c++
const ResourceManager::Sound <4> audioExplosion;
// ...
audioExplosion = Sound<4>(rm, "path", "path", "path", "path");
```

IE: I would have to edit the code at multiple locations if I wanted to add a new sound. Or
pay for a vector or a pointer or something. That's at least 8 bytes I have to pay for no other
reason than C++ lacking type inference.



### Virt


`VIRT` lifts virtual methods to the type level:


```c++
template <unsigned N, class R, class...S>
struct VIRT<N, R(S...)>
{
  virtual R operator() (const ProxyIX<N>, S...) = 0;
};

// ..

static constexpr FOO = 42;
using HasFoo = VIRT <FOO, void (int, char)>;

// ..

struct Thing : HasFoo, HasBar, HasBaz {}
```


I have two reasons for using `VIRT`:

1) Virtual methods are not a zero-cost abstraction. If `VIRT` is responsible for all
   virtual methods I have a single location in code to update if I figure out an
   alternate way of doing it.

2) `VIRT` is composable. It is possible to write higher order `VIRT` combinatorics:

```c++
template <class N, class R, class...A>
struct Override<VIRT<N,R(A...)>> : VIRT<N,R(A...)> {
  R operator () (ProxyIX<N>, A...) override { .. }
}

template <class...Virts>
struct Foo : Override<Virts>... { .. }
```

What makes this possible is that `VIRT` methods don't have names. The proxy argument acts as its name.
If I instead had uniquely named methods I wouldn't be able to write `Override`- anything that has to
override a virtual method I would have to write manually!

