#include "classifyscrollarea.h"

ClassifyScrollArea::ClassifyScrollArea()
{
    init_wid();

}

void ClassifyScrollArea::init_wid()
{
    this->verticalScrollBar()->setHidden(true);
    this->horizontalScrollBar()->setHidden(true);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}
