#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <CGAL/Sweep_line_2_algorithms.h>

template <class Traits_IS,
         class Container_IS = std::vector<typename Traits_IS::Segment_2>,
         class Container_IP = std::vector<typename Traits_IS::Point_2> >
class Intersection
{
public:
    typedef Traits_IS Traits;

    typedef typename Traits_IS::Segment_2 Segment_2;

    typedef typename Container_IS::iterator Segment_iterator;
    typedef typename Container_IS::const_iterator Segment_const_iterator;

    typedef typename Traits_IS::Point_2 Point_2;

    typedef typename Container_IP::iterator Point_iterator;
    typedef typename Container_IP::const_iterator Point_const_iterator;

    Intersection(const Traits & is_traits = Traits()) : traits(is_traits)
    {
        update();
    }
    Intersection(const Intersection<Traits_IS, Container_IS>& is)
        : d_container(is.d_container), traits(is.traits)
        {
            update();
        }
    template <class InputIterator>
        Intersection(InputIterator first, InputIterator last,
                   Traits is_traits = Traits())
        : d_container(), traits(is_traits)
            {
                std::copy(first, last, std::back_inserter(d_container));
                update();
            }

    template <class InputIterator>
        void insert(InputIterator first,
                    InputIterator last,
                    Traits is_traits = Traits())
            {
                std::copy(first, last, std::back_inserter(d_container));
                update();
            }

    void insert(Segment_2 s)
        {
            d_container.push_back(s);
            update();
        }

    void erase(Segment_iterator first, Segment_iterator last)
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
            return is.size();
        }
    
    Point_const_iterator is_begin()
        {
            return is.begin();
        }

    Point_const_iterator is_end()
        {
            return is.end();
        }

private:
    Container_IS d_container;
    Traits_IS traits;

    Container_IP is;

    void update()
        {
            is.clear();
            CGAL::compute_intersection_points (d_container.begin(), d_container.end(),
                                               std::back_inserter(is));
        }
};

#endif
