#include "PointEdit.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QFocusEvent>
#include <QKeyEvent>

using namespace Tools;

PointEdit::PointEdit( QWidget * parent ) :
	QAbstractSpinBox(parent)
{
	init();
}

PointEdit::PointEdit(const QVector3D &point, QWidget * parent ) :
	QAbstractSpinBox(parent), _p(point)
{
	init();
}

void PointEdit::init()
{
	_separator = ";";
	connect(lineEdit(), SIGNAL(textChanged(QString)), this, SLOT(inputChanged(const QString&)));
	updateValue();
	updateValidationExp();
	setWrapping(false);
	//lineEdit()->setAlignment(Qt::AlignRight);
	setAccelerated(true);
}

QVector3D PointEdit::getPoint() const
{
	return _p;
}

void PointEdit::setPoint(const QVector3D &p)
{
	_p = p;
	updateValue();
}

void PointEdit::setPrefixes(const QString &xPrefix, const QString &yPrefix, const QString &zPrefix)
{
	_xPrefix = xPrefix;
	_yPrefix = yPrefix;
	_zPrefix = zPrefix;
	updateValidationExp();
	updateValue();
}

void PointEdit::setSuffixes(const QString &xSuffix, const QString &ySuffix, const QString &zSuffix)
{
	_xSuffix = xSuffix;
	_ySuffix = ySuffix;
	_zSuffix = zSuffix;
	updateValidationExp();
	updateValue();
}

QString PointEdit::getSeparator() const
{
	return _separator;
}

void PointEdit::setSeparator(const QString &value)
{
	_separator = value;
	if (_separator.isEmpty())
		_separator = " ";
	updateValidationExp();
	updateValue();
}

QValidator::State PointEdit::validate(QString& input, int& pos) const
{
	Q_UNUSED(pos);
	bool ok = _validationExp.exactMatch(input);

	return ok ? QValidator::Acceptable : QValidator::Invalid;

	return QAbstractSpinBox::validate(input, pos);
}

QAbstractSpinBox::StepEnabled PointEdit::stepEnabled() const
{
	return StepUpEnabled | StepDownEnabled;
}

void PointEdit::stepBy( int steps )
{
	PointEdit::Section currentSection = getCurrentSection();
	switch (currentSection)
	{
	case X:
		_p.setX(_p.x() + steps);
		break;
	case Y:
		_p.setY(_p.y() + steps);
		break;
	case Z:
		_p.setZ(_p.z() + steps);
		break;
	}

	updateValue();
}
void PointEdit::updateValue()
{
	Section currentSection = getCurrentSection();
	updateValue(currentSection);
}

void PointEdit::updateValue(Section sectionToSelect)
{
	int px1, px2;
	int py1, py2;
	int pz1, pz2;

	QString text;
	text += _xPrefix;
	px1 = text.count();
	text += QString::number(_p.x());
	px2 = text.count();
	text += _xSuffix;

	text += _separator;

	text += _yPrefix;
	py1 = text.count();
	text += QString::number(_p.y());
	py2 = text.count();
	text += _ySuffix;

	text += _separator;

	text += _zPrefix;
	pz1 = text.count();
	text += QString::number(_p.z());
	pz2 = text.count();
	text += _zSuffix;

	lineEdit()->setText(text);

	if (hasFocus())
	{
		switch(sectionToSelect)
		{
		case X:
			lineEdit()->setSelection(px1, px2-px1);
			break;
		case Y:
			lineEdit()->setSelection(py1, py2-py1);
			break;
		case Z:
			lineEdit()->setSelection(pz1, pz2-pz1);
			break;
		}
	}
}

void PointEdit::updateValidationExp()
{
	QString escapedSeparator = QRegExp::escape(_separator);

	QString pattern;
	pattern += "^";

	pattern += QRegExp::escape(_xPrefix);
	pattern += "\\d{1,}";
	pattern += QRegExp::escape(_xSuffix);

	pattern += escapedSeparator;

	pattern += QRegExp::escape(_yPrefix);
	pattern += "\\d{1,}";
	pattern += QRegExp::escape(_ySuffix);

	pattern += escapedSeparator;

	pattern += QRegExp::escape(_zPrefix);
	pattern += "\\d{1,}";
	pattern += QRegExp::escape(_zSuffix);

	pattern += "$";

	_validationExp = QRegExp(pattern);
}


PointEdit::Section PointEdit::getCurrentSection() const
{
	int pos = lineEdit()->cursorPosition();

	QString text = lineEdit()->text();

	int p1 = 0;
	if (_xSuffix.isEmpty())
		p1 = text.indexOf(_separator, _xPrefix.count());
	else
	{
		p1 = text.indexOf(_xSuffix, _xPrefix.count());
		p1 += _xSuffix.count();
	}
	if (pos <= p1)
		return X;

	int p2 = 0;
	if (_xSuffix.isEmpty())
		p2 = text.indexOf(_separator, p1 + _separator.count());
	else
	{
		p2 = text.indexOf(_ySuffix, p1 + _separator.count());
		p2 += _ySuffix.count();
	}
	if (pos <= p2)
		return Y;

	return Z;
}

void PointEdit::focusInEvent(QFocusEvent *event)
{
	Q_UNUSED(event);
	//QAbstractSpinBox::focusInEvent(event);
	updateValue(X);
}

bool PointEdit::focusNextPrevChild(bool next)
{
	Section currentSection = getCurrentSection();
	if (next && currentSection != Z)
	{
        currentSection = (Section)(currentSection + 1);
        updateValue(currentSection);
		return true;
	}
	else if (!next && currentSection != X)
	{
        currentSection = (Section)(currentSection - 1);
        updateValue(currentSection);
		return true;
	}

	return QAbstractSpinBox::focusNextPrevChild(next);
}

void PointEdit::keyReleaseEvent(QKeyEvent *event)
{
	QAbstractSpinBox::keyReleaseEvent(event);
}

void PointEdit::keyPressEvent(QKeyEvent *event)
{
	Section currentSection = getCurrentSection();

	if (event->key() == Qt::Key_Right)
	{
		if (currentSection == Z)
			updateValue(X);
		else
        {
            currentSection = (Section)(currentSection + 1);
            updateValue(currentSection);
        }
	}
	else if (event->key() == Qt::Key_Left)
	{
		if (currentSection == X)
			updateValue(Z);
		else
        {
            currentSection = (Section)(currentSection - 1);
            updateValue(currentSection);
        }
	}
	else if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_PageUp) && lineEdit()->hasSelectedText())
	{
		int value = 1;
		if ((event->modifiers() & Qt::ShiftModifier) ==  Qt::ShiftModifier)
			value *= 10;
		if (event->key() == Qt::Key_PageUp)
			value *= 10;
		stepBy(value);
	}
	else if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_PageDown) && lineEdit()->hasSelectedText())
	{
		int value = -1;
		if ((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier)
			value *= 10;
		if (event->key() == Qt::Key_PageDown)
			value *= 10;
		stepBy(value);
	}
	else
		QAbstractSpinBox::keyReleaseEvent(event);
}

void PointEdit::inputChanged(const QString& input)
{
	QString text(input);
	text.remove(_xPrefix);
	text.remove(_xSuffix);
	text.remove(_yPrefix);
	text.remove(_ySuffix);
	text.remove(_zPrefix);
	text.remove(_zSuffix);

	QStringList tokens = text.split(_separator);
	_p.setX(tokens.value(0).toInt());
	_p.setY(tokens.value(1).toInt());
	_p.setZ(tokens.value(2).toInt());
}

QString PointEdit::getXText() const
{
	QString text;
	text += _xPrefix;
	text += QString::number(_p.x());
	text += _xSuffix;

	return text;
}

QString PointEdit::getYText() const
{
	QString text;
	text += _yPrefix;
	text += QString::number(_p.y());
	text += _ySuffix;

	return text;
}

QString PointEdit::getZText() const
{
	QString text;
	text += _zPrefix;
	text += QString::number(_p.z());
	text += _zSuffix;

	return text;
}

bool PointEdit::event(QEvent* e)
{
//	int a = e->type();

//	if (e->type() == QEvent::KeyPress)
//	{

//		QKeyEvent* ke = (QKeyEvent*)(e);
//		if (ke->key() == Qt::Key_Tab && _lastSelectedSection != Z)
//		{
//			updateValue(++_lastSelectedSection);
//			return false;
//		}
//	}
	return QAbstractSpinBox::event(e);
}
