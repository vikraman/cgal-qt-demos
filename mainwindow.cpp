#include "mainwindow.h"

MainWindow::MainWindow()
    : DemosMainWindow()
{
    setupUi(this);

    // Triangulation
    rgi = new CGAL::Qt::RegularTriangulationGraphicsItem<Regular>(&rt);

    QObject::connect(this, SIGNAL(changed()),
                     rgi, SLOT(modelChanged()));

    rgi->setVerticesPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    scene.addItem(rgi);

    // Powerdiagram
    vgi = new CGAL::Qt::PowerdiagramGraphicsItem<Regular>(&rt);

    QObject::connect(this, SIGNAL(changed()),
                     vgi, SLOT(modelChanged()));

    vgi->setEdgesPen(QPen(Qt::blue, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    scene.addItem(vgi);
    vgi->hide();

    // Mold
    //mgi = new CGAL::Qt::MoldGraphicsItem<Regular>(&rt);

    //QObject::connect(this, SIGNAL(changed()),
    //                 mgi, SLOT(modelChanged()));

    //mgi->setVerticesPen(QPen(Qt::red, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    //scene.addItem(mgi);

    // ConvexHull
    chgi = new ConvexHullGraphicsItem<CHull>(&ch);

    QObject::connect(this, SIGNAL(changed()),
                     chgi, SLOT(modelChanged()));

    chgi->setHullPen(QPen(Qt::magenta, 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    scene.addItem(chgi);
    chgi->hide();

    pi = new CGAL::Qt::GraphicsViewCircleInput<K>(this, &scene, 1);


    QObject::connect(pi, SIGNAL(generate(CGAL::Object)),
                     this, SLOT(processInput(CGAL::Object)));

    trv = new CGAL::Qt::RegularTriangulationRemoveVertex<Regular>(&rt, this);
    QObject::connect(trv, SIGNAL(modelChanged()),
                     this, SIGNAL(changed()));

    QObject::connect(this->actionQuit, SIGNAL(triggered()),
                     this, SLOT(close()));

    QActionGroup* ag = new QActionGroup(this);
    ag->addAction(this->actionInsertPoint);

    this->actionInsertPoint->setChecked(true);
    this->actionShowRegular->setChecked(true);

    scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    scene.setSceneRect(-100, -100, 100, 100);
    this->graphicsView->setScene(&scene);
    this->graphicsView->setMouseTracking(true);
    this->graphicsView->matrix().scale(1, -1);

    this->addNavigation(this->graphicsView);

    this->setupStatusBar();
    this->setupOptionsMenu();
    this->addAboutCGAL();
}


void
MainWindow::processInput(CGAL::Object o)
{
    std::pair<Point_2, K::FT > center_sqr;
    if(CGAL::assign(center_sqr, o))
        {
            Regular::Point wp(center_sqr.first, center_sqr.second);
            rt.insert(wp);
            ch.insert(wp);
        }

    emit(changed());
}


void
MainWindow::on_actionInsertPoint_toggled(bool checked)
{
    if(checked)
        {
            scene.installEventFilter(pi);
            scene.installEventFilter(trv);
        }
    else
        {
            scene.removeEventFilter(pi);
            scene.removeEventFilter(trv);
        }
}



void
MainWindow::on_actionShowRegular_toggled(bool checked)
{
    rgi->setVisibleEdges(checked);
}


void
MainWindow::on_actionShowPowerdiagram_toggled(bool checked)
{
    vgi->setVisible(checked);
}


void
MainWindow::on_actionShowConvexHull_toggled(bool checked)
{
    chgi->setVisible(checked);
}


void
MainWindow::on_actionShowMoldLpp_toggled(bool checked)
{
    //mgi->setVisible(checked);
}

void
MainWindow::on_actionClear_triggered()
{
    rt.clear();
    ch.clear();
    emit(changed());
}


void
MainWindow::on_actionInsertRandomPoints_triggered()
{
    QRectF rect = CGAL::Qt::viewportsBbox(&scene);
    CGAL::Qt::Converter<K> convert;
    Iso_rectangle_2 isor = convert(rect);
    CGAL::Random_points_in_iso_rectangle_2<Point_2> pg(isor.min(), isor.max());
    const int number_of_points =
        QInputDialog::getInteger(this,
                                 tr("Number of random points"),
                                 tr("Enter number of random points"), 100, 0);

    // wait cursor
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<Point_2> points;
    points.reserve(number_of_points);
    for(int i = 0; i < number_of_points; ++i)
        {
            points.push_back(*pg++);
        }
    rt.insert(points.begin(), points.end());
    ch.insert(points.begin(), points.end());
    // default cursor
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    emit(changed());
}


void
MainWindow::on_actionLoadPoints_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                       tr("Open Points file"),
                       ".");
    if(! fileName.isEmpty())
        {
            std::ifstream ifs(qPrintable(fileName));

            Gt::Weighted_point_2 p;
            std::vector<Gt::Weighted_point_2> points;
            while(ifs >> p)
                {
                    points.push_back(p);
                }
            rt.insert(points.begin(), points.end());
            ch.insert(points.begin(), points.end());

            actionRecenter->trigger();
            emit(changed());
        }
}


void
MainWindow::on_actionSavePoints_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                       tr("Save points"),
                       ".");
    if(! fileName.isEmpty())
        {
            std::ofstream ofs(qPrintable(fileName));
            for(Regular::Finite_vertices_iterator
                    vit = rt.finite_vertices_begin(),
                    end = rt.finite_vertices_end();
                    vit != end; ++vit)
                {
                    ofs << vit->point() << std::endl;
                }
        }
}


void
MainWindow::on_actionRecenter_triggered()
{
    this->graphicsView->setSceneRect(rgi->boundingRect());
    this->graphicsView->fitInView(rgi->boundingRect(), Qt::KeepAspectRatio);
}
