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

using ColT = Collidable;

class QuadTree
{
private:
  bool      isLeaf = true;
  unsigned  level  = 0;
  const unsigned  capacity;
  const unsigned  maxLevel;
  const V4        bounds;
  QuadTree* parent = nullptr;
  QuadTree* children[4] = { nullptr, nullptr, nullptr, nullptr };
  std::vector<Collidable*> objects, foundObjects;

private:
  void subdivide();
  void discardEmptyBuckets();
  inline QuadTree *getChild(const V4 &bound) const noexcept;

public:
  V4 getBounds() const { return bounds; }

  QuadTree(const V4 &_bound, unsigned _capacity, unsigned _maxLevel);
  QuadTree(const QuadTree&);
  QuadTree();

  bool insert(ColT *obj);
  bool remove(ColT *obj);
  bool update(ColT *obj);
  std::vector<ColT*> &getObjectsInBound(const V4 &bound);
  unsigned totalChildren() const noexcept;
  unsigned totalObjects() const noexcept;
  void clear() noexcept;

  ~QuadTree();
};

#endif // QUADTREE_H
