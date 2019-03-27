// https://github.com/Megabyte918/QuadTree-Cpp

#ifndef QUADTREE_H
#define QUADTREE_H

#include <any>
#include <vector>
#include <algorithm>

#include "Math.h"

class  QuadTree;
struct Collidable;

struct Collidable {
  friend class QuadTree;

public:
  virtual V4 * getBounds () { return nullptr; }

private:
  QuadTree *qt = nullptr;
};

class QuadTree {
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
private:
  bool      isLeaf = true;
  unsigned  level  = 0;
  unsigned  capacity;
  unsigned  maxLevel;
  V4        bounds;
  QuadTree* parent = nullptr;
  QuadTree* children[4] = { nullptr, nullptr, nullptr, nullptr };
  std::vector<Collidable*> objects, foundObjects;

  void subdivide();
  void discardEmptyBuckets();
  inline QuadTree *getChild(const V4 &bound) const noexcept;
};

#endif // QUADTREE_H
