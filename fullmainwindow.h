#ifndef FULLMAINWINDOW_H
#define FULLMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class FullMainWindow;
}

class FullMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FullMainWindow(QWidget *parent = nullptr);
    ~FullMainWindow();
public:
Q_SIGNALS:
    void showNormalWindow();
private Q_SLOTS:
    void on_minPushButton_clicked();

    void on_fullSelectToolButton_clicked();

    void on_fullSelectMenuButton_triggered(QAction *arg1);

protected:
    void paintEvent(QPaintEvent *event);
    bool event ( QEvent * event);

private:
    QAction *m_allAction = nullptr;
    QAction *m_letterAction = nullptr;
    QAction *m_funcAction = nullptr;
    QMenu *m_menu = nullptr;
    Ui::FullMainWindow *ui;
};

#endif // FULLMAINWINDOW_H
