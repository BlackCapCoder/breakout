# How to build

Summon your favorite terminal and punch in

```bash
λ make run   # 🚌
```

Followed by a litteral newline, or alternatively `<C-j>`. If you find this process difficult you can contact our support-line at `#monadsafterdark` at freenode! If we're unavailable, leave a message with `knut_____` (5 underscores).

# Default keys

After hours and hours in search for the optimal keys for playing the game,
we stuck with the vim keys. They are convenient, reduces strain `=` lets you play more.

**"But I'm not a vim user"** - I can hear you say. <br>
Thats hardly a problem as `vimtutor` is an excellent kickstarter.

Here is a list of the keys:

             ^
             k              Hint:  The h key is at the left and moves left.
       < h       l >               The l key is at the right and moves right.
             j                     The j key looks like a down arrow.
             v

    h      => MoveLeft
    l      => MoveRight
    j      => MoveDown
    k      => MoveUp
    SPACE  => ReleaseBall
    f      => FireRocket
    d      => PowerMagnet
    ESCAPE => Quit

Moreover, the keys can be edited in `main.cpp` (arrow keys are not supported- they were too expensive)


# Workflow

We have worked on separate things over git. We kept a list of things that needed to
be done on our github page, and would pick something off of it to work on.
We have also been discussing implementation details over irc, and irl.


We have had two major code conventions: write performant and imperative code. Our
reasoning is that in the wild, there is only one reason for choosing C++ and that
is performance.


We have intentionally overdone this. If we could avoid a pointer by writing 200 lines
of template programming we did the template programming.


For mostly the same reason we choose to do imperative programming, rather than the psuedo
functional style you can do in c++. The implication is that c++ is a low level language:
regardless of your code style your code is going to have a lot of low level details, and
the compiler cannot optimize away that.


For instance, consider the functor laws:
```haskell
map id = id
map f . map g = map (f . g)
```

The first law says that looping through all the elements in a list and doing nothing is the same as doing nothing.


The second law says that looping through all the elements in a list and doing `f`, then looping through them and doing `g`
is the same as doing both `f` and `g` in a single pass.


gcc does not enforce either of these laws for its `for_each` function, even for pure code, but it is able to do both
optimizations for the equivalent for loop.


One solution to this is defunctionalization- to promote functions to the type level. Then we could hand optimize
them with rewrite rules and get guaranteed inlining. For instance:

```c++
template <int N>
struct Add {
  int operator () (int x) { return x + N; }
};

// function composition
template <template <class...> class f, template <class...> class g> struct Dot;

template <int X, int Y>
struct Simplify<Dot<Add<X>, Add<Y>>> {
  using R = Add<X+Y>;
};
```

This is pretty extreme though- we are essentially writing our own compiler on the
type level. Rewrite rules are turing-complete, so there is no limit to how far
we could stretch this.


### Why OOP

What are the alternatives?


1) Procedural code. This doesn't really scale well for games.

2) Functional reactive programming. This would be both slow and awkward in c++. We could make it less
   awkward with concepts, but they were disallowed. We could make it fast with defunctionalization,
   but it would neither be maintainable nor idiomatic.


# Gotchas

We have used `new` for some objects to be added to the `ColScene`, without ever freeing them. We could solve this with
smart pointers, but we ought to store the objects in the `ColScene` rather than just pointers to them. We never got
around to this.


We didn't implement the QuadTree ourselves (we did adapt it somewhat, though!).
There is a link to where we got it from at the very top of the file.
Our reasoning is that quadtrees really ought to be library data structure highly tuned by someone else.


# Implementation Overview


### Game

`Game` is the main class. It initializes all the SDL boilerplate code and has a single method `loop`. `loop`
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

`ColScene` is a `Scene` and also a heterogeneous container of GameObjects. It will keep track of any
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

