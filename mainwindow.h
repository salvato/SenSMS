#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

public slots:
    void on_pushButton_clicked();

protected:
    bool check_permission();
private:
    QPushButton* pSendButton;
    QLineEdit*   pLineEditDestinataire;
    QLineEdit*   pLineEditTexte;
};

#endif // MAINWINDOW_H
