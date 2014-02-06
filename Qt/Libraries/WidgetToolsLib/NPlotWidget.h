#ifndef NPLOTWIDGET_H
#define NPLOTWIDGET_H

#include <qcustomplot.h>

namespace Tools
{
	class NPlotWidget : public QCustomPlot
	{
		Q_OBJECT
	public:
		explicit NPlotWidget(QWidget *parent = 0);
		virtual ~NPlotWidget();

		void setLegendVisible(bool value);
		void setAutoResize(bool value);

		void setAxisScale(double xMin, double xMax, double yMin, double yMax);
		void setAxisValuesVisible(bool x, bool y);

		void setDefaultInteractionEnabled(bool value);

        void setTitle(const QString& title);

	protected:
		bool _autoResize;
	};

	class MultiCurveWidget : public NPlotWidget
	{
		Q_OBJECT
	public:
		explicit MultiCurveWidget(QWidget *parent = 0);
		virtual ~MultiCurveWidget();

		QCPCurve *addCurve(const QString& name, const QPen& pen);
		void addValue(int curveIndex, double x, double y);
		void addValues(double x, const QList<double> &y);
		void addValues(const QList<double>& x, double y);
		void addValues(const QList<double>& x, const QList<double>& y);

		void setValues(int curveIndex, const QVector<double>& x, const QVector<double>& y);
		void clear();

		QCPCurve* getCurve(int i) const;

		void setDataNumberLimit(int limit);

	private:
		QList<QCPCurve*> _curves;
		int _dataNumberLimit;

		void customRescale();
	};

	class MultiBarWidget : public NPlotWidget
	{
		Q_OBJECT
	public:
		explicit MultiBarWidget(QWidget *parent = 0);
		virtual ~MultiBarWidget();

		QCPBars *addBar(const QString& name, const QPen& pen, const QBrush& brush);
		void setValues(const QList<double> &values);
		void clear();

		QCPBars* getBar(int i) const;

	protected:
		QList<QCPBars*> _bars;
	};
}


#endif // NPLOTWIDGET_H
