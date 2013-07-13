#ifndef FULLSCREENWIDGET_H
#define FULLSCREENWIDGET_H

#include <QWidget>
#include <QHBoxLayout>

namespace Tools
{
	class FullScreenWidget : public QWidget
	{
		Q_OBJECT

	signals:
		void fullScreenStoped(QWidget* widget);

	public:
		static FullScreenWidget* instance();

		void setFullScreen(QWidget* widget);
		void stopFullScreen();

	protected:
		explicit FullScreenWidget();

		void keyPressEvent(QKeyEvent *event);

	private:
		bool _isFullScreen;
		QWidget* _w;
		QHBoxLayout* _layout;

	};
}

#endif // FULLSCREENWIDGET_H
