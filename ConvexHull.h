#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <CGAL/convex_hull_2.h>

template < class Traits_CH,
         class Container_CH = std::vector<typename Traits_CH::Point_2> >
class ConvexHull
{
public:
    typedef Traits_CH Traits;
    typedef Container_CH Container;

    typedef typename Traits_CH::Point_2 Point_2;

    typedef typename Container::iterator Point_iterator;
    typedef typename Container::const_iterator Point_const_iterator;

    ConvexHull(const Traits & ch_traits = Traits()) : traits(ch_traits)
    {
        update();
    }
    ConvexHull(const ConvexHull<Traits_CH, Container_CH>& chull)
        : d_container(chull.d_container), traits(chull.traits)
    {
        update();
    }
    template <class InputIterator>
    ConvexHull(InputIterator first, InputIterator last,
               Traits ch_traits = Traits())
        : d_container(), traits(ch_traits)
    {
        std::copy(first, last, std::back_inserter(d_container));
        update();
    }

    template <class InputIterator>
    void insert(InputIterator first,
                InputIterator last,
                Traits ch_traits = Traits())
    {
        std::copy(first, last, std::back_inserter(d_container));
        update();
    }

    void insert(Point_2 p)
    {
        d_container.push_back(p);
        update();
    }

    void erase(Point_iterator first, Point_iterator last)
    {
        d_container.erase(first, last);
        update();
    }

    void clear()
    {
        d_container.clear();
        update();
    }

    std::size_t size() const
    {
        return ch.size();
    }

    Point_const_iterator chull_begin()
    {
        return ch.begin();
    }

    Point_const_iterator chull_end()
    {
        return ch.end();
    }

private:
    Container_CH d_container;
    Traits_CH traits;

    Container_CH ch;

    void update()
    {
        /*
         * Ideally, this should not recompute the convex hull entirely.
         * The convex hull should be updated with the new point.
         * ConvexHull guy: please fix this!
         *
         */
        ch.clear();
        CGAL::convex_hull_2(d_container.begin(), d_container.end(), std::back_inserter(ch));
    }
};

#endif
