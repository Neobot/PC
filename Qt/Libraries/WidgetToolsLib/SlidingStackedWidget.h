#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QEasingCurve>


namespace Tools
{
	/**
	 * \details SlidingStackedWidget is a class that is derived from QtStackedWidget
	 * and allows smooth side shifting of widgets, in addition
	 * to the original hard switching from one to another as offered by
	 * QStackedWidget itself.
	 * \note Original version: http://www.developer.nokia.com/Community/Wiki/Code_Example_for_SlidingStackedWidget_class_in_Qt
	*/
	class SlidingStackedWidget : public QStackedWidget
	{
		Q_OBJECT

		Q_PROPERTY( int speed READ getSpeed WRITE setSpeed )
		Q_PROPERTY( QEasingCurve::Type animation READ getAnimation WRITE setAnimation )
		Q_PROPERTY( bool verticalMode READ getVerticalMode WRITE setVerticalMode )
		Q_PROPERTY( bool wrap READ isWrap WRITE setWrap )

	public:
		/**
		 * \brief This enumeration is used to define the animation direction.
		 */
		enum Direction
		{
			LEFT2RIGHT,
			RIGHT2LEFT,
			TOP2BOTTOM,
			BOTTOM2TOP,
			AUTOMATIC
		};

		SlidingStackedWidget(QWidget *parent);
		~SlidingStackedWidget();

		/**
		* \brief Sets the duration of the naimation in milliseconds.
		* \see getSpeed
		*/
		void setSpeed(int speed);   //animation duration in milliseconds

		/**
		* \brief Returns the duration of the naimation in milliseconds.
		* \see setSpeed
		*/
		int getSpeed() const;

		/**
		* \brief Sets animation type.
		* \see getAnimation
		*/
		void setAnimation(QEasingCurve::Type animationtype); //check out the QEasingCurve documentation for different styles

		/**
		* \brief Returns animation type.
		* \see setAnimation
		*/
		QEasingCurve::Type getAnimation() const;

		/**
		* \brief Sets the vertical mode.
		* \see getVerticalMode
		*/
		void setVerticalMode(bool vertical);

		/**
		* \brief Returns true, if the animation will be performed vertically by default.
		* \see setVerticalMode
		*/
		bool getVerticalMode() const;

		/**
		* \brief Sets the wrap mode.
		* \see isWrap
		*/
		void setWrap(bool wrap);

		/**
		* \brief Reurns the wrap mode.
		* \details It defines the behaviour when reaching last/first page of #slideInNext and #slideInPrev functions.
		* \see setWrap
		*/
		bool isWrap() const;

	public slots:
		/**
		* \brief Slides to the next page.
		* \see slideInPrev
		* \see slideInIdx
		* \see isWrap
		*/
		void slideInNext();

		/**
		* \brief Slides to the previous page.
		* \see slideInNext
		* \see slideInIdx
		* \see isWrap
		*/
		void slideInPrev();

		/**
		* \brief Slides to the page at \p index.
		* \see slideInNext
		* \see slideInIdx
		*/
		void slideInIndex(int index, Direction direction=AUTOMATIC);

		/**
		* \brief Returns true if the animation is in progress.
		*/
		bool isActive() const;

	signals:
		/**
		* \brief This signal is emmitted when the animation has finished.
		*/
		void animationFinished();

	private slots:
		void animationDoneSlot();

	private:
		/**
		* \brief Slides to \p widget.
		*/
		void slideInWidget(QWidget* widget, Direction direction);

		int m_speed;
		enum QEasingCurve::Type m_animationtype;
		bool m_vertical;
		bool m_wrap;
		bool m_active;

		int m_now;
		int m_next;
		QPoint m_pnow;

		QList<QWidget*> blockedPageList;
	};
}

#endif // SLIDINGSTACKEDWIDGET_H
