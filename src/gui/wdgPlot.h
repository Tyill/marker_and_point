
#pragma once

#include <QtWidgets/qwidget.h>

class wdgPlot : public QWidget
{
	Q_OBJECT
      
    float _scale = 1.f;
    QVector<QPoint> _markers, _points;

public:
	wdgPlot(QWidget *parent = 0);   
    void setMarkers(QVector<QPoint>&& markers);

protected:		
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent * event);

public slots:
    void setPoints(QVector<QPoint> pnts);
};