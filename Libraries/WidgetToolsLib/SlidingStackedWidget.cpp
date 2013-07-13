#include "SlidingStackedWidget.h"
#include <QtDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

using namespace Tools;

SlidingStackedWidget::SlidingStackedWidget(QWidget *parent)
    : QStackedWidget(parent), m_speed(500), m_animationtype(QEasingCurve::OutBack), m_vertical(false), m_wrap(false), m_active(false),
	m_now(0), m_next(0)
{
    if (parent == 0) 
	{
		qDebug()<<"WARNING: untested main window case!";
	}
}


SlidingStackedWidget::~SlidingStackedWidget()
{
}

void SlidingStackedWidget::setVerticalMode(bool vertical) 
{
	m_vertical = vertical;
}

bool SlidingStackedWidget::getVerticalMode() const
{
	return m_vertical;
}

void SlidingStackedWidget::setSpeed(int speed) 
{
	m_speed = speed;
}

int SlidingStackedWidget::getSpeed() const
{
	return m_speed;
}

void SlidingStackedWidget::setAnimation(enum QEasingCurve::Type animationtype) 
{
	m_animationtype = animationtype;
}

QEasingCurve::Type SlidingStackedWidget::getAnimation() const
{
	return m_animationtype;
}

void SlidingStackedWidget::setWrap(bool wrap) 
{
	m_wrap = wrap;
}

bool SlidingStackedWidget::isWrap() const
{
	return m_wrap;
}

void SlidingStackedWidget::slideInNext() 
{
	if (count() == 0)
		return;

	Direction dir = AUTOMATIC;

	int now = currentIndex();
	int next = now + 1;

	if (next >= count())
	{
		if (m_wrap)
		{
			next = 0;
			dir = m_vertical ? TOP2BOTTOM : RIGHT2LEFT;
		}
		else
			return;
	}

	slideInIndex(next, dir);
}

void SlidingStackedWidget::slideInPrev() 
{
	if (count() == 0)
		return;

	Direction dir = AUTOMATIC;

	int now = currentIndex();
	int prev = now - 1;

	if (prev < 0)
	{
		if (m_wrap)
		{
			prev = count() - 1;
			dir = m_vertical ? BOTTOM2TOP : LEFT2RIGHT;
		}
		else
			return;
	}

	slideInIndex(prev, dir);
}

void SlidingStackedWidget::slideInIndex(int index, Direction direction) 
{
	if (index >= count() || index < 0) 
		return;

	slideInWidget(widget(index), direction);
}

bool SlidingStackedWidget::isActive() const
{
	return m_active;
}

void SlidingStackedWidget::slideInWidget(QWidget* widget, Direction direction) 
{
	if (m_active) 
	{
		// at the moment, do not allow re-entrance before an animation is completed.
		//other possibility may be to finish the previous animation abrupt, or
		//to revert the previous animation with a counter animation, before going ahead
		//or to revert the previous animation abrupt
		//and all those only, if the newwidget is not the same as that of the previous running animation.
		return; 
	}

	if (widget == currentWidget())
		return;

	//Computes automatic direction
	int now = currentIndex();
	int next = indexOf(widget);

	if (direction == AUTOMATIC)
	{
		if (m_vertical)
			direction = now < next ? TOP2BOTTOM : BOTTOM2TOP;
		else
			direction = now < next ? RIGHT2LEFT : LEFT2RIGHT;
	}

	m_active = true;

	//calculate the shifts
	int offsetx=frameRect().width(); //inherited from mother
	int offsety=frameRect().height();//inherited from mother

	//the following is important, to ensure that the new widget
	//has correct geometry information when sliding in first time
	widget->setGeometry (0,  0, offsetx, offsety);

	if (direction==BOTTOM2TOP)  
	{
		offsetx=0;
		offsety=-offsety;
	}
	else if (direction==TOP2BOTTOM) 
	{
		offsetx=0;
		//offsety=offsety;
	}
	else if (direction==RIGHT2LEFT) 
	{
		offsetx=-offsetx;
		offsety=0;
	}
	else if (direction==LEFT2RIGHT) 
	{
		//offsetx=offsetx;
		offsety=0;
	}

	//re-position the next widget outside/aside of the display area
	QPoint pnext=widget->pos();
	QPoint pnow=currentWidget()->pos();
	m_pnow = pnow;

	widget->move(pnext.x()-offsetx,pnext.y()-offsety);
	//make it visible/show
	widget->show();
	widget->raise();

	//animate both, the now and next widget to the side, using animation framework
	QPropertyAnimation *animnow = new QPropertyAnimation(currentWidget(), "pos");

	animnow->setDuration(m_speed);
	animnow->setEasingCurve(m_animationtype);
	animnow->setStartValue(QPoint(pnow.x(), pnow.y()));
	animnow->setEndValue(QPoint(offsetx+pnow.x(), offsety+pnow.y()));
	QPropertyAnimation *animnext = new QPropertyAnimation(widget, "pos");
	animnext->setDuration(m_speed);
	animnext->setEasingCurve(m_animationtype);
	animnext->setStartValue(QPoint(-offsetx+pnext.x(), offsety+pnext.y()));
	animnext->setEndValue(QPoint(pnext.x(), pnext.y()));

	QParallelAnimationGroup *animgroup = new QParallelAnimationGroup;

	animgroup->addAnimation(animnow);
	animgroup->addAnimation(animnext);

	QObject::connect(animgroup, SIGNAL(finished()),this,SLOT(animationDoneSlot()));
	m_next=next;
	m_now=now;
	m_active=true;
	animgroup->start();

	//note: the rest is done via a connect from the animation ready;
	//animation->finished() provides a signal when animation is done;
	//so we connect this to some post processing slot,
	//that we implement here below in animationDoneSlot.
}

void SlidingStackedWidget::animationDoneSlot() 
{
    //when ready, call the QStackedWidget slot setCurrentIndex(int)
    setCurrentIndex(m_next);  //this function is inherit from QStackedWidget
    
	//then hide the outshifted widget now, and  (may be done already implicitely by QStackedWidget)
    widget(m_now)->hide();
    
	//then set the position of the outshifted widget now back to its original
	//so that the application could also still call the QStackedWidget original functions/slots for changings
    widget(m_now)->move(m_pnow);
    
    //widget(m_now)->update();
    //setCurrentIndex(m_next);  //this function is inherit from QStackedWidget
    m_active=false;
    emit animationFinished();
}
