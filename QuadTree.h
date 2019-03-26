// https://github.com/Megabyte918/QuadTree-Cpp

#ifndef QUADTREE_INCLUDED
#define QUADTREE_INCLUDED

#include <any>
#include <vector>
#include <algorithm>

struct Rect;
struct Collidable;
class  QuadTree;

struct Rect {
    double x, y, width, height;

    bool contains(const Rect &other) const noexcept;
    bool intersects(const Rect &other) const noexcept;
    double getLeft() const noexcept;
    double getTop() const noexcept;
    double getRight() const noexcept;
    double getBottom() const noexcept;

    Rect(const Rect&);
    Rect(double _x = 0, double _y = 0, double _width = 0, double _height = 0);
};

struct Collidable {
    friend class QuadTree;
public:
    Rect bound;
    std::any data;

    Collidable(const Rect &_bounds = {}, std::any _data = {});
private:
    QuadTree *qt = nullptr;
    Collidable(const Collidable&) = delete;
};

class QuadTree {
public:
    QuadTree(const Rect &_bound, unsigned _capacity, unsigned _maxLevel);
    QuadTree(const QuadTree&);
    QuadTree();

    bool insert(Collidable *obj);
    bool remove(Collidable *obj);
    bool update(Collidable *obj);
    std::vector<Collidable*> &getObjectsInBound(const Rect &bound);
    unsigned totalChildren() const noexcept;
    unsigned totalObjects() const noexcept;
    void clear() noexcept;

    ~QuadTree();
private:
    bool      isLeaf = true;
    unsigned  level  = 0;
    unsigned  capacity;
    unsigned  maxLevel;
    Rect      bounds;
    QuadTree* parent = nullptr;
    QuadTree* children[4] = { nullptr, nullptr, nullptr, nullptr };
    std::vector<Collidable*> objects, foundObjects;

    void subdivide();
    void discardEmptyBuckets();
    inline QuadTree *getChild(const Rect &bound) const noexcept;
};

#endif
