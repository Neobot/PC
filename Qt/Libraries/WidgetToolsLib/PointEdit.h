#include <QAbstractSpinBox>
#include <QVector3D>

namespace Tools
{
	class PointEdit : public QAbstractSpinBox
	{
		Q_OBJECT


	public:
		PointEdit(QWidget* parent = 0);
		PointEdit(const QVector3D& point, QWidget* parent = 0);

		QVector3D getPoint() const;
		void setPoint(const QVector3D &p);

		void setPrefixes(const QString& xPrefix, const QString& yPrefix, const QString& zPrefix);
		void setSuffixes(const QString& xSuffix, const QString& ySuffix, const QString& zSuffix);

		QString getSeparator() const;
		void setSeparator(const QString& value);

		bool event(QEvent *e);
	private:
		enum Section
		{
			X,
			Y,
			Z
		};

		QVector3D _p;

		QString _xPrefix;
		QString _xSuffix;

		QString _yPrefix;
		QString _ySuffix;

		QString _zPrefix;
		QString _zSuffix;

		QString _separator;

		QRegExp _validationExp;

		void init();
		void updateValue();
		void updateValue(Section sectionToSelect);
		void updateValidationExp();

		QString getXText() const;
		QString getYText() const;
		QString getZText() const;

		QValidator::State validate(QString& input, int& pos) const;
		void stepBy (int steps);
		StepEnabled stepEnabled() const;

		Section getCurrentSection() const;
		void focusInEvent(QFocusEvent *event);
		bool focusNextPrevChild(bool next);
		void keyReleaseEvent(QKeyEvent *event);
		void keyPressEvent(QKeyEvent *event);

	private slots:
		void inputChanged(const QString& input);
	};
}
