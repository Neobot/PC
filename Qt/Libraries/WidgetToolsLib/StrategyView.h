#ifndef STRATEGYVIEW_H
#define STRATEGYVIEW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QWheelEvent>
#include <QTimer>

namespace Tools
{
	class StrategyScene;
	class DataMap;

	class StrategyView : public QGraphicsView
	{
		Q_OBJECT
		#define ZOOM_STEP 1.2

	public:
		explicit StrategyView(QWidget *parent = 0);

		void zoomIn();
		void zoomOut();
		void scaleViewBy(qreal scaleFactor);
		void scaleViewTo(qreal absoluteScaleFactor);

		StrategyScene* getScene() const;
		void reset();

		void setRefreshInterval(int value);
		void setRefreshEnabled(bool value);

	private:
		qreal _zoomScale;
		QTimer _timer;
		StrategyScene* _scene;

		void wheelEvent(QWheelEvent *event);
		qreal calculateScaleFactor(const QMatrix& matrix);

		void mousePressEvent(QMouseEvent* event);
		void resizeEvent(QResizeEvent* event);

	public slots:
		void refresh();
	};
}

#endif // STRATEGYVIEW_H
