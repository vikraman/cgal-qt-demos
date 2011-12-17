#ifndef CONVEX_HULL_GRAPHICS_ITEM_H
#define CONVEX_HULL_GRAPHICS_ITEM_H

#include <CGAL/Qt/GraphicsItem.h>
#include <CGAL/Qt/PainterOstream.h>

template <typename CH>
class ConvexHullGraphicsItem : public CGAL::Qt::GraphicsItem
{
    typedef typename CGAL::Kernel_traits<typename CH::Point_2>::Kernel K;
    typedef typename K::Segment_2 Segment_2;

    typedef typename CH::Traits Traits;

public:
    inline ConvexHullGraphicsItem(CH * ch);
    inline void modelChanged();

public:
    inline QRectF boundingRect() const;
    inline void paint(QPainter * painter,
                      const QStyleOptionGraphicsItem * option,
                      QWidget * widget);

    const QPen& hullPen() const
    {
        return hull_pen;
    }

    void setHullPen(const QPen& pen)
    {
        hull_pen = pen;
    }

    void setVisible(const bool b)
    {
        visible = b;
        update();
    }

protected:
    CH * chull;
    QPainter * m_painter;
    CGAL::Qt::PainterOstream<K> painterostream;

    QPen hull_pen;
    bool visible;
};

template <typename CH>
ConvexHullGraphicsItem<CH>::ConvexHullGraphicsItem(CH * ch) : chull(ch), painterostream(0), visible(false)
{
    setHullPen(QPen(::Qt::magenta, 3.));
    this->hide();
}

template <typename CH>
void
ConvexHullGraphicsItem<CH>::modelChanged()
{
    if (chull->size() > 1)
        this->show();
    else
        this->hide();
    update();
}

template <typename CH>
QRectF
ConvexHullGraphicsItem<CH>::boundingRect() const
{
    if(scene())
        {
            return CGAL::Qt::viewportsBbox(scene());
        }
    return QRectF();
}

template <typename CH>
void
ConvexHullGraphicsItem<CH>::paint(QPainter * painter,
                                  const QStyleOptionGraphicsItem * option,
                                  QWidget * widget)
{
    painter->setPen(this->hullPen());
    painterostream = CGAL::Qt::PainterOstream<K>(painter);

    if (visible)
        {
            typename CH::Point_const_iterator it0, it1;

            if (chull->size() < 2) return;

            for (it0 = it1 = chull->chull_begin(), it1++;
                    it1 != chull->chull_end();
                    it0++, it1++)
                {
                    Segment_2 s(*it0, *it1);
                    painterostream << s;
                }

            if (chull->size() > 2)
                {
                    it1 = chull->chull_begin();
                    Segment_2 s(*it0, *it1);
                    painterostream << s;
                }
        }
}

#endif
