#include <QtGui>
#include <QRectF>
#include <QPen>
#include <QColor>

#include <CGAL/point_generators_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_euclidean_traits_2.h>
#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/Qt/Converter.h>
#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <CGAL/Qt/RegularTriangulationGraphicsItem.h>
#include <CGAL/Qt/VoronoiGraphicsItem.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;

typedef double Weight;
typedef CGAL::Regular_triangulation_euclidean_traits_2<K,Weight> Gt; 
typedef CGAL::Regular_triangulation_2<Gt> Regular;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay;

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    QGraphicsScene scene;
    QRectF rect(0, 0, 1280, 720);

    scene.setSceneRect(rect);
    CGAL::Qt::Converter<K> convert;
    Iso_rectangle_2 isor = convert(rect);
    CGAL::Random_points_in_iso_rectangle_2<Point_2> pg(isor.min(), isor.max());
    std::vector<Point_2> points;
    for(int i = 0; i < 1000; i++) {
        points.push_back(*pg++);
    }

    Regular rt;
    rt.insert(points.begin(), points.end());

    Delaunay dt;
    dt.insert(points.begin(), points.end());

    CGAL::Qt::RegularTriangulationGraphicsItem<Regular> * rtgi;
    rtgi = new CGAL::Qt::RegularTriangulationGraphicsItem<Regular>(&rt);

    CGAL::Qt::VoronoiGraphicsItem<Delaunay> * vgi;
    vgi = new CGAL::Qt::VoronoiGraphicsItem<Delaunay>(&dt);
    vgi->setEdgesPen(QPen(QColor(0, 0, 255)));

    scene.addItem(rtgi);
    scene.addItem(vgi);

    QGraphicsView* view = new QGraphicsView(&scene);
    CGAL::Qt::GraphicsViewNavigation navigation;
    view->installEventFilter(&navigation);
    view->viewport()->installEventFilter(&navigation);
    view->setRenderHint(QPainter::Antialiasing);

    view->show();
    return app.exec();
}
