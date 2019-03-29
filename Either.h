#ifndef EITHER_H
#define EITHER_H


template <class L, class R>
struct Either
{
  bool isR;
  union
  {
    L * l;
    R * r;
  };

  Either<L,R> (L * l) : isR{false}, l{l} {}
  Either<L,R> (R * r) : isR{true }, r{r} {}
};


#endif // ifndef EITHER_H
