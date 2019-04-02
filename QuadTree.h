// https://github.com/Megabyte918/QuadTree-Cpp

#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>
#include <algorithm>

#include "Math.h"

class  QuadTree;
struct Collidable;

struct Collidable
{
  friend class QuadTree;

private:
  QuadTree *qt = nullptr;

public:
  virtual V4 & getBounds () = 0;
};

class QuadTree
{
private:
  bool      isLeaf = true;
  unsigned  level  = 0;
  const unsigned  capacity;
  const unsigned  maxLevel;
  V4        bounds;
  QuadTree* parent = nullptr;
  QuadTree* children[4] = { nullptr, nullptr, nullptr, nullptr };
  std::vector<Collidable*> objects, foundObjects;

private:
  void subdivide();
  void discardEmptyBuckets();
  inline QuadTree *getChild(const V4 &bound) const noexcept;

public:
  QuadTree(const V4 &_bound, unsigned _capacity, unsigned _maxLevel);
  QuadTree(const QuadTree&);
  QuadTree();

  bool insert(Collidable *obj);
  bool remove(Collidable *obj);
  bool update(Collidable *obj);
  std::vector<Collidable*> &getObjectsInBound(const V4 &bound);
  unsigned totalChildren() const noexcept;
  unsigned totalObjects() const noexcept;
  void clear() noexcept;

  ~QuadTree();
};

#endif // QUADTREE_H
