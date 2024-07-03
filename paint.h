#ifndef PAINT_H
#define PAINT_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>

#include <QSvgGenerator>
#include <svgread.h>

#include <moveitem.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Paint; }
QT_END_NAMESPACE

class Paint : public QMainWindow
{
    Q_OBJECT

public:
    explicit Paint(QWidget* _parent = 0);
    ~Paint();

private:
    Ui::Paint* ui;
    QTimer* timer;
    paintscene* scene;
    QString f_path;

private:
    void resizeEvent(QResizeEvent* _event);
    void placeholder(paintscene* _psc, MoveItem* _pmi, bool _arrow_t, char _arrow_s);
    bool checkUniqueCriteria(QGraphicsItem* _item);

private slots:
    void slotTimer();
    void on_pb_square_clicked();
    void on_pb_circle_clicked();
    void on_pb_rhomb_clicked();
    void on_pb_oval_clicked();
    void on_pb_rectangle_clicked();
    void on_pb_brush_clicked();
    void on_pb_triangle_clicked();
    void on_action_clear_scene_triggered();
    void on_pb_color_clicked();
    void on_action_openf_triggered();
    void on_action_savef_triggered();
};
#endif // PAINT_H
