#ifndef MOLD_LPP_GRAPHICS_ITEM_H
#define MOLD_LPP_GRAPHICS_ITEM_H

#include <CGAL/Qt/GraphicsItem.h>
#include <CGAL/Qt/PainterOstream.h>

template <typename ML>
class MoldLppGraphicsItem : public CGAL::Qt::GraphicsItem
{
    typedef typename CGAL::Kernel_traits<typename ML::Segment_2>::Kernel K;
    typedef typename ML::Traits Traits;

public:
    inline MoldLppGraphicsItem(ML * ml);
    inline void modelChanged();

public:
    inline QRectF boundingRect() const;
    inline void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);

    const QPen& lpPen() const
        {
            return lp_pen;
        }

    void setLpPen(const QPen& pen)
        {
            lp_pen = pen;
        }

    void setVisible(const bool b)
        {
            visible = b;
            update();
        }

protected:
    ML * lp;
    QPainter * m_painter;
    CGAL::Qt::PainterOstream<K> painterostream;

    QPen lp_pen;
    bool visible;
};

template <typename ML>
MoldLppGraphicsItem<ML>::MoldLppGraphicsItem(ML * ml) : lp(ml), painterostream(0), visible(false)
{
    setLpPen(QPen(::Qt::magenta, 3.));
    this->hide();
}

template <typename ML>
void
MoldLppGraphicsItem<ML>::modelChanged()
{
    if (lp->size() > 2)
        {
            this->show();
        }
    else
        {
            this->hide();
        }
    update();
}

template <typename ML>
QRectF
MoldLppGraphicsItem<ML>::boundingRect() const
{
    if(scene()){
        return CGAL::Qt::viewportsBbox(scene());
    }
    return QRectF();
}

template <typename ML>
void
MoldLppGraphicsItem<ML>::paint(QPainter * painter,
                              const QStyleOptionGraphicsItem * option,
                              QWidget * widget)
{
    painter->setPen(this->lpPen());
    painterostream = CGAL::Qt::PainterOstream<K>(painter);

    if (visible) {
        typename K::Circle_2 c(lp->lpoint(), 5);
        painterostream << c;
    }
}

#endif
