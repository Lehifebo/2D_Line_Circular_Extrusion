// canvas.cpp
#include "canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QOpenGLWidget(parent) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);
    initializeGL();
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    for (int i = 0; i < points.size() - 1; ++i) {
        painter.drawLine(points[i], points[i + 1]);
    }
}


/**
 * @brief Canvas::getPoints used to get the point drawn on the canvas
 * before being returned, the points buffer is cleared
 * @return a QVector<QPointF> of the current active points on the canvas
 */
QVector<QPointF> Canvas::getPoints(){
    QVector<QPointF> temp = points;
    return temp;
}
