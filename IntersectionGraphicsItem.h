#ifndef INTERSECTION_GRAPHICS_ITEM_H
#define INTERSECTION_GRAPHICS_ITEM_H

#include <CGAL/Qt/GraphicsItem.h>
#include <CGAL/Qt/PainterOstream.h>

template <typename IS>
class IntersectionGraphicsItem : public CGAL::Qt::GraphicsItem
{
    typedef typename CGAL::Kernel_traits<typename IS::Segment_2>::Kernel K;

public:
    inline IntersectionGraphicsItem(IS * is);
    inline void modelChanged();

public:
    inline QRectF boundingRect() const;
    inline void paint(QPainter * painter,
                      const QStyleOptionGraphicsItem * option,
                      QWidget * widget);

    const QPen& isPen() const
    {
        return is_pen;
    }

    void setIsPen(const QPen& pen)
    {
        is_pen = pen;
    }

    void setVisible(const bool b)
    {
        visible = b;
        update();
    }

protected:
    IS * is;
    QPainter * m_painter;
    CGAL::Qt::PainterOstream<K> painterostream;

    QPen is_pen;
    bool visible;
};

template <typename IS>
IntersectionGraphicsItem<IS>::IntersectionGraphicsItem(IS * is) : is(is), painterostream(0), visible(false)
{
    setIsPen(QPen(::Qt::cyan, 3.));
    this->hide();
}

template <typename IS>
void
IntersectionGraphicsItem<IS>::modelChanged()
{
    if (is->size() > 0)
        this->show();
    else
        this->hide();
    update();
}

template <typename IS>
QRectF
IntersectionGraphicsItem<IS>::boundingRect() const
{
    if(scene())
        {
            return CGAL::Qt::viewportsBbox(scene());
        }
    return QRectF();
}

template <typename IS>
void
IntersectionGraphicsItem<IS>::paint(QPainter * painter,
                                    const QStyleOptionGraphicsItem * option,
                                    QWidget * widget)
{
    painter->setPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painterostream = CGAL::Qt::PainterOstream<K>(painter);
    typename IS::Segment_const_iterator it;

    for (it = is->s_begin(); it != is->s_end(); it++)
        {
            painterostream << *it;
        }

    if (visible)
        {
            painter->setPen(this->isPen());
            painterostream = CGAL::Qt::PainterOstream<K>(painter);
            typename IS::Point_const_iterator it;

            for (it = is->is_begin(); it != is->is_end(); it++)
                {
                    typename K::Circle_2 c (*it, 5);
                    painterostream << c;
                }
        }
}

#endif
