
#include <QPainter>
#include <QWheelEvent>

#include "wdgPlot.h"

wdgPlot::wdgPlot(QWidget *parent){

	setParent(parent);
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(p);
}

void wdgPlot::setMarkers(QVector<QPoint>&& markers){

    _markers = qMove(markers);

    update();
}

void wdgPlot::setPoints(QVector<QPoint> pnts){

    _points = qMove(pnts);

    update();
}

void wdgPlot::wheelEvent(QWheelEvent* event){
  
    _scale += (event->delta() > 0) ? 0.2f : -0.2f;

    if (_scale < 0.2f){
        _scale = 0.2f;
    }

    update();

    QWidget::wheelEvent(event);
}

void wdgPlot::paintEvent(QPaintEvent *event){
    
    QPainter painter(this);

    int w = width(),
        h = height();

    painter.translate(w / 2, h / 2);

    painter.scale(_scale, _scale);
   
    painter.setPen(Qt::blue);     
   
    int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
    for (auto& p : _points){
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    if (maxX == minX){
        maxX = minX + 1;
    }
    if (maxY == minY){
        maxY = minY + 1;
    }
    for (auto& p : _points){
        p = QPoint((p.x() - minX) * w / float(maxX - minX) - w /2, (p.y() - minY) * h / float(maxY - minY) - h /2);
    }

    painter.drawPoints(_points.data(), _points.size());

    //////////////////////////////////////////////////////    
      
    painter.setPen(Qt::green);

    minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
    for (auto& p : _markers){
        if (p.x() < minX) minX = p.x();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.y() > maxY) maxY = p.y();
    }
    if (maxX == minX){
        maxX = minX + 1;
    }
    if (maxY == minY){
        maxY = minY + 1;
    }
    auto markers = _markers;
    for (auto& p : markers){
        p = QPoint((p.x() - minX) * w / float(maxX - minX) - w / 2, (p.y() - minY) * h / float(maxY - minY) - h / 2);
    }

    for (auto& m : markers){
        painter.drawLine(QPoint(m.x() - 5, m.y()), QPoint(m.x() + 5, m.y()));
        painter.drawLine(QPoint(m.x(), m.y() - 5), QPoint(m.x(), m.y() + 5));
    }

    QWidget::paintEvent(event);
}



