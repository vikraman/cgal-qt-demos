#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <fstream>

// CGAL headers
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_euclidean_traits_2.h>
#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/point_generators_2.h>

// Qt headers
#include <QObject>
#include <QtGui>
#include <QString>
#include <QActionGroup>
#include <QFileDialog>
#include <QInputDialog>

// GraphicsView items and event filters (input classes)
#include "RegularTriangulationRemoveVertex.h"
#include <CGAL/Qt/GraphicsViewCircleInput.h>
#include <CGAL/Qt/RegularTriangulationGraphicsItem.h>
#include <CGAL/Qt/PowerdiagramGraphicsItem.h>
//#include "moldlppGraphicsItem.h"

#include "ConvexHull.h"
#include "ConvexHullGraphicsItem.h"

// for viewportsBox
#include <CGAL/Qt/utility.h>

#include "ui_mainwindow.h"
#include <CGAL/Qt/DemosMainWindow.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Point_2 Circle_2;
typedef K::Iso_rectangle_2 Iso_rectangle_2;

typedef double Weight;
typedef CGAL::Regular_triangulation_euclidean_traits_2<K, Weight>  Gt;
typedef CGAL::Regular_triangulation_2<Gt> Regular;
typedef ConvexHull<K, std::list<Point_2> > CHull;

class MainWindow :
    public CGAL::Qt::DemosMainWindow,
    public Ui::MainWindow
{
    Q_OBJECT

private:
    Regular rt;
    CHull ch;
    QGraphicsScene scene;

    CGAL::Qt::RegularTriangulationGraphicsItem<Regular> * rgi;
    CGAL::Qt::PowerdiagramGraphicsItem<Regular> * vgi;
    //CGAL::Qt::MoldGraphicsItem<Regular> * mgi;
    ConvexHullGraphicsItem<CHull> * chgi;

    CGAL::Qt::RegularTriangulationRemoveVertex<Regular> * trv;
    CGAL::Qt::GraphicsViewCircleInput<K> * pi;
public:
    MainWindow();

public slots:

    void processInput(CGAL::Object o);

    void on_actionShowRegular_toggled(bool checked);

    void on_actionShowPowerdiagram_toggled(bool checked);

    void on_actionShowConvexHull_toggled(bool checked);

    void on_actionShowMoldLpp_toggled(bool checked);

    void on_actionInsertPoint_toggled(bool checked);

    void on_actionInsertRandomPoints_triggered();

    void on_actionLoadPoints_triggered();

    void on_actionSavePoints_triggered();

    void on_actionClear_triggered();

    void on_actionRecenter_triggered();


signals:
    void changed();
};

#endif
