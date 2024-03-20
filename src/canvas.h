#ifndef CANVAS_H
#define CANVAS_H

#include <QOpenGLWidget>
#include <QMouseEvent>

/**
 * @brief The Canvas class responsbile for taking the mouse drawing input.
 */
class Canvas : public QOpenGLWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    QVector<QPointF> getPoints();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QVector<QPointF> points;
};

#endif // CANVAS_H
