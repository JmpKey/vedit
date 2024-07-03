#include "paint.h"
#include "ui_paint.h"

static int randomBetween(int _low, int _high)
{
    return (qrand() % ((_high + 1) - _low) + _low);
}

Paint::Paint(QWidget* _parent) : QMainWindow(_parent), ui(new Ui::Paint)
{
    ui->setupUi(this);

    this->resize(640, 640);
    this->setFixedSize(640, 640);

    scene = new paintscene();

    QGraphicsScene* qscene = new QGraphicsScene;
    qscene->installEventFilter(scene);

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->resize(600,600);

    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    scene->setSceneRect(0,0,500,500);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Paint::slotTimer);
    timer->start(100);
}

Paint::~Paint()
{
    delete ui;
}

void Paint::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void Paint::resizeEvent(QResizeEvent* _event)
{
    timer->start(100);
    QWidget::resizeEvent(_event);
}

void Paint::placeholder(paintscene* _psc, MoveItem* _pmi, bool _arrow_t, char _arrow_s)
{
    _psc->setArrowTool(_arrow_t);
    _pmi->setArrowShape(_arrow_s);
    _pmi->setPos(randomBetween(30, 470),
    randomBetween(30,470));
    scene->addItem(_pmi);
}

void Paint::on_pb_square_clicked()
{
    MoveItem* item = new MoveItem();
    placeholder(scene, item, false, 'r');
}

void Paint::on_pb_circle_clicked()
{
    MoveItem* item = new MoveItem();
    placeholder(scene, item, false, 'e');
}

void Paint::on_pb_rhomb_clicked()
{
    MoveItem* item = new MoveItem();
    placeholder(scene, item, false, 'p');
}

void Paint::on_pb_oval_clicked()
{
    MoveItem* item = new MoveItem();
    placeholder(scene, item, false, 'E');
}

void Paint::on_pb_rectangle_clicked()
{
    MoveItem* item = new MoveItem();
    placeholder(scene, item, false, 'R');
}

void Paint::on_pb_triangle_clicked()
{
    MoveItem* item = new MoveItem();
    placeholder(scene, item, false, 't');
}

void Paint::on_pb_brush_clicked()
{
    (scene->getArrowTool()) ? scene->setArrowTool(false) : scene->setArrowTool(true);
}

void Paint::on_action_clear_scene_triggered() { scene->clear(); }

void Paint::on_pb_color_clicked() {}

void Paint::on_action_openf_triggered()
{
    QString tmp_f = "";
    QString f_name = QFileDialog::getOpenFileName(this, trUtf8("Open SVG File"), tmp_f, "SVG Files (*.svg)");

    if (f_name.isEmpty()) {
        qWarning("No file selected");
        return;
    }

    scene->clear();
    scene->setArrowTool(false);
    QBrush sneak;

    foreach (QGraphicsEllipseItem* oval_i, SvgRead::getOvalElements(f_name)) {
        MoveItem* m_oval = new MoveItem();
        m_oval->setArrowShape('E');
        m_oval->setPos(m_oval->pos());
        sneak = oval_i->brush();
        m_oval->setColor(sneak.color());
        scene->addItem(m_oval);
    }

    foreach (QGraphicsPathItem* romb_i, SvgRead::getDiamondElements(f_name)) {
        MoveItem* m_romb = new MoveItem();
        m_romb->setArrowShape('p');
        m_romb->setPos(m_romb->pos());
        sneak = romb_i->brush();
        m_romb->setColor(sneak.color());
        scene->addItem(m_romb);
    }

    foreach (QGraphicsPathItem* triangle_i, SvgRead::getTriangleElements(f_name)) {
        MoveItem* m_triangle = new MoveItem();
        m_triangle->setArrowShape('t');
        m_triangle->setPos(m_triangle->pos());
        sneak = triangle_i->brush();
        m_triangle->setColor(sneak.color());
        scene->addItem(m_triangle);
    }

    // init obj pars
    QList<QGraphicsEllipseItem*> circles = SvgRead::getCircleElements(f_name);
    QList<QGraphicsRectItem*> rects = SvgRead::getRectElements(f_name);
    QList<QGraphicsRectItem*> squares = SvgRead::getRectElements(f_name);
    QList<MoveItem*> addedItems;

    // assemble circle <--
    int objectsAdded = 0;
    int fu = 0;
    foreach (QGraphicsEllipseItem* circl_i, circles) {
        MoveItem* m_circl = new MoveItem();
        m_circl->setArrowShape('e');
        sneak = circl_i->brush();
        addedItems.append(m_circl);
        objectsAdded++;
    }

    foreach(MoveItem* circl, addedItems)
    {
        if(fu >= objectsAdded) { break; }
        else {
            sneak = circles[fu]->brush();
            circl->setColor(sneak.color());
            scene->addItem(circl);
            fu++;
        }
    }

    while (fu < addedItems.size()) {
        MoveItem* itemToRemove = addedItems.takeLast();
        delete itemToRemove;
    } addedItems.clear();
    // -->

    QVector<QBrush> vsneak_r;
    QVector<QBrush> vsneak_s;

    // assemble rect <--
    objectsAdded = 0; fu = 0;
    foreach (QGraphicsRectItem* rect_i, rects) {
        if (qFuzzyCompare(rect_i->rect().width(), rect_i->rect().height())) {
            continue;
        }

        MoveItem* m_rect = new MoveItem();
        m_rect->setArrowShape('R');
        vsneak_r.append(rect_i->brush());
        addedItems.append(m_rect);
        objectsAdded++;
    }

    foreach(MoveItem* rect, addedItems)
    {
        if(fu >= objectsAdded) { break; }
        else {
            sneak = vsneak_r.takeFirst();
            rect->setColor(sneak.color());
            scene->addItem(rect);
            fu++;
        }
    }

    while (fu < addedItems.size()) {
        MoveItem* itemToRemove = addedItems.takeLast();
        delete itemToRemove;
    } addedItems.clear(); objectsAdded = 0; fu = 0; vsneak_r.clear();
    // -->

    // assemble square <--
    foreach (QGraphicsRectItem* square_i, squares) {
        if (!qFuzzyCompare(square_i->rect().width(), square_i->rect().height())) {
             continue;
            }
        MoveItem* m_square = new MoveItem();
        m_square->setArrowShape('r');
        vsneak_s.append(square_i->brush());
        addedItems.append(m_square);
        objectsAdded++;
    }

    foreach(MoveItem* square, addedItems)
    {
        if(fu >= objectsAdded) { break; }
        else {
            sneak = vsneak_s.takeFirst();
            square->setColor(sneak.color());
            scene->addItem(square);
            fu++;
        }
    }

    while (fu < addedItems.size()) {
        MoveItem* itemToRemove = addedItems.takeLast();
        delete itemToRemove;
    } addedItems.clear(); objectsAdded = 0; fu = 0; vsneak_s.clear();
    // -->
}

void Paint::on_action_savef_triggered()
{
    /*
     * Dependency packages:
         qt5-qtsvg
         qt5-qtsvg-devel
         qt5-qtsvg-examples

         add: sudo dnf install qt5-qtsvg qt5-qtsvg-devel qt5-qtsvg-examples
    */

    QString new_path = QFileDialog::getSaveFileName(this, trUtf8("Save SVG"), f_path + ".svg", tr("SVG files (*.svg)"));

    if(new_path.isEmpty())
        return;

    f_path = new_path;

    QSvgGenerator generator;
    generator.setFileName(f_path);
    generator.setSize(QSize(scene->width(), scene->height()));
    generator.setViewBox(QRect(0, 0, scene->width(), scene->height()));
    generator.setTitle(trUtf8("SVG Example"));
    generator.setDescription(trUtf8("File created by SVG Example"));

    QPainter painter;
    painter.begin(&generator);

    for (QGraphicsItem* item : scene->items()) {
        if (!dynamic_cast<QPointF*>(item) && !checkUniqueCriteria(item)) {
            QStyleOptionGraphicsItem option;
            QWidget widget;
            item->paint(&painter, &option, &widget);
        }
    }

    painter.end();
}

bool Paint::checkUniqueCriteria(QGraphicsItem* item) {
    if (item->type() == QGraphicsEllipseItem::Type) {
        QGraphicsEllipseItem* ellipseItem = dynamic_cast<QGraphicsEllipseItem*>(item);
        if (ellipseItem) {
            QBrush brush = ellipseItem->brush();
            if (brush.color() == Qt::red) {
                return true;
            }
        }
    }
    return false;
}
