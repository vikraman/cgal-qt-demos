#ifndef MOLD_LPP_H
#define MOLD_LPP_H

#include <utility>
#include <vector>
#include <cstddef>

template < class Traits_ML,
         class Container_ML = std::vector<typename Traits_ML::Segment_2>,
         class LPoint = typename Traits_ML::Point_2 >
class MoldLpp
{
public:
    typedef Traits_ML Traits;
    typedef Container_ML COntainer;

    typedef typename Traits_ML::Segment_2 Segment_2;

    typedef typename Container_ML::iterator Segment_iterator;
    typedef typename Container_ML::const_iterator Segment_const_iterator;

    MoldLpp(const Traits & ml_traits = Traits()) : traits(ml_traits)
    {
        update();
    }
    MoldLpp(const MoldLpp<Traits_ML, Container_ML>& mold)
        : d_container(mold.d_container), traits(mold.traits)
    {
        update();
    }
    template <class InputIterator>
    MoldLpp(InputIterator first, InputIterator last,
            Traits ml_traits = Traits())
        : d_container(), traits(ml_traits)
    {
        std::copy(first, last, std::back_inserter(d_container));
        update();
    }

    template <class InputIterator>
    void insert(InputIterator first,
                InputIterator last,
                Traits ml_traits = Traits())
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
        return d_container.size();
    }

    LPoint lpoint() const
    {
        return lp;
    }

private:
    Container_ML d_container;
    Traits_ML traits;

    LPoint lp;

    void update()
    {
        if (d_container.size() < 3) return;
        std::vector<std::pair<double, double> > points;
        for (Segment_iterator it = d_container.begin();
                it != d_container.end(); it++)
            {
                points.push_back(std::make_pair(it->vertex(0).x(), it->vertex(0).y()));
                points.push_back(std::make_pair(it->vertex(1).x(), it->vertex(1).y()));
            }
        std::vector<double> lpoint = mold(points);
        lp = LPoint(lpoint[0], lpoint[1]);
    }

    /* Begin sbcd90's code */
    std::vector<double> mold(std::vector<std::pair<double, double> > a)
    {
        std::vector <std::vector<double> > x;
        int i, j, w;
        double u, v;
        for(i = 0; i < (a.size() - 1); i = i + 2)
            {
                double j;
                x.resize(x.size() + 1);
                x[x.size()-1].resize(3);
                j = ((a[i].second - a[i+1].second) / (a[i].first - a[i+1].first));
                x[x.size()-1][0] = j;
                x[x.size()-1][1] = -1;
                x[x.size()-1][2] = (j * a[i+1].first - a[i+1].second);
            }
        std::vector <double> result(2);
        result[0] = 0.0;
        result[1] = (x[0][2] / x[0][1]);
        for(i = 1; i < a.size()/2; i++)
            {
                if((result[0]*x[i][0] + result[1]*x[i][1]) <= x[i][2])
                    continue;
                else
                    {
                        int f = 0;
                        for(j = 0; j < i; j++)
                            {
                                u = (x[i][2] * x[j][1] - x[i][1] * x[j][2]) / (x[i][0] * x[j][1] - x[j][0] * x[i][1]);
                                v = (x[i][0] * x[j][2] - x[i][2] * x[j][0]) / (x[i][0] * x[j][1] - x[j][0] * x[i][1]);
                                for(w = 0; w < i; w++)
                                    {
                                        if((u * x[w][0] + v * x[w][1]) <= x[w][2])
                                            continue;
                                        else
                                            break;
                                    }
                                if(w == i)
                                    {
                                        f = 1;
                                        break;
                                    }
                            }
                        if(f == 1)
                            {
                                result[0] = u;
                                result[1] = v;
                            }
                    }
            }
        return result;
    }
    /* End sbcd90's code */
};

#endif
