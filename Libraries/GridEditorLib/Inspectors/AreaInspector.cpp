#include "AreaInspector.h"
#include "ui_AreaInspector.h"

#include "AreaItem.h"

AreaInspector::AreaInspector(QWidget *parent) :
    QWidget(parent), ui(new Ui::AreaInspectorWidget), _displayedArea(0)
{
    ui->setupUi(this);

    connect(ui->leAreaName, SIGNAL(textEdited(QString)), this, SLOT(nameChanged()));
    connect(ui->spX, SIGNAL(valueChanged(double)), this, SLOT(positionChanged()));
    connect(ui->spY, SIGNAL(valueChanged(double)), this, SLOT(positionChanged()));
    connect(ui->spWidth, SIGNAL(valueChanged(double)), this, SLOT(positionChanged()));
    connect(ui->spHeight, SIGNAL(valueChanged(double)), this, SLOT(positionChanged()));
}

AreaInspector::~AreaInspector()
{
    delete ui;
}

void AreaInspector::clearInfo()
{
    _displayedArea = 0;
    setReadOnly(true);

    blockSignals(true);
    ui->leAreaName->clear();
    ui->spX->clear();
    ui->spY->clear();
    ui->spWidth->clear();
    ui->spHeight->clear();
    blockSignals(false);
}

void AreaInspector::showInfo(AreaItem *area)
{
    if (!area)
        return;

    _displayedArea = area;
    setReadOnly(false);

    QPointF pos = SceneTools::mapWithStandardReference(_displayedArea->pos());
    QSizeF s = SceneTools::mapWithStandardReference(_displayedArea->getRect().size());

    blockSignals(true);
    ui->leAreaName->setText(_displayedArea->getName());
    ui->spX->setValue(pos.x());
    ui->spY->setValue(pos.y());
    ui->spWidth->setValue(s.width());
    ui->spHeight->setValue(s.height());
    blockSignals(false);
}

void AreaInspector::update()
{
    showInfo(_displayedArea);
}

void AreaInspector::setReadOnly(bool value)
{
    ui->leAreaName->setReadOnly(value);
    ui->spX->setReadOnly(value);
    ui->spY->setReadOnly(value);
    ui->spWidth->setReadOnly(value);
    ui->spHeight->setReadOnly(value);
}

void AreaInspector::nameChanged()
{
    emit nameChanged(_displayedArea, ui->leAreaName->text());
}

void AreaInspector::positionChanged()
{
    const QRectF& oldRect = _displayedArea->getRect();
    QRectF newRect = SceneTools::mapWithStandardReference(QRectF(ui->spX->value(), ui->spY->value(),
                                                                 ui->spWidth->value(), ui->spHeight->value()));

    emit areaChanged(_displayedArea, _displayedArea->pos(), oldRect.size(), newRect.topLeft(), newRect.size());
}
