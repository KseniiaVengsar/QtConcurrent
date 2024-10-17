#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtConcurrent>
#include <QFutureWatcher>
#include "ExampleRace.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void StartRace();

private slots:
    void on_pb_start_clicked();

private:
    Ui::MainWindow *ui;
    uint32_t number = 0, countFinish = 0;

    QMutex m; // Мьютекс для синхронизации
    ExampleRace *concurRace1; // Первый экземпляр гонки
    ExampleRace *concurRace2; // Второй экземпляр гонки

    void handleRaceFinish(); // Метод для обработки завершения гонки
};

#endif // MAINWINDOW_H
