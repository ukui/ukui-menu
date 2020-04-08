#include "letterclassifybutton.h"

LetterClassifyButton::LetterClassifyButton(QWidget *parent,
                                           QString hoverbg,
                                           QString pressedbg,
                                           QString letter):
    QToolButton(parent)
{
    this->hoverbg=hoverbg;
    this->pressedbg=pressedbg;
    this->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize);
    this->setFont(font);
    this->setText(letter);
    this->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
    this->setCheckable(true);
    connect(this,SIGNAL(toggled(bool)),this,SLOT(reactToToggle(bool)));
}

void LetterClassifyButton::enterEvent(QEvent *e)
{
    Q_UNUSED(e);
    this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
}

void LetterClassifyButton::leaveEvent(QEvent *e)
{
    Q_UNUSED(e);
    if(is_pressed)
        this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    else
        this->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight,Style::LeftLetterBtnHeight);
}

void LetterClassifyButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
    is_pressed=true;
}

void LetterClassifyButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
    QFont font;
    font.setPixelSize(Style::LeftLetterFontSize*3);
    this->setFont(font);
    this->setFixedSize(Style::LeftLetterBtnHeight*2,Style::LeftLetterBtnHeight*2);
    Q_EMIT buttonClicked(this);
}

void LetterClassifyButton::reactToToggle(bool checked)
{
    if(checked)
    {
        this->setStyleSheet(QString("background:transparent;color:#ffffff;padding-left:0px;"));
        this->is_pressed=true;
    }
    else
    {
        this->setStyleSheet(QString("background:transparent;color:#8b8b8b;padding-left:0px;"));
        this->is_pressed=false;
    }
}
