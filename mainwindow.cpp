#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->sb_initNum->setValue(200000);
    ui->rb_synchOff->setChecked(true);

    // Инициализация гонок
    concurRace1 = new ExampleRace(&m);
    concurRace2 = new ExampleRace(&m);

    // Сигналы по завершению работы
    connect(concurRace1, &ExampleRace::sig_Finish, this, &MainWindow::handleRaceFinish);
    connect(concurRace2, &ExampleRace::sig_Finish, this, &MainWindow::handleRaceFinish);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::StartRace() {
    if (ui->rb_qtConcur->isChecked()) {
        ui->te_debug->append("Запуск гонки с использованием QtConcurrent...");
    } else {
        ui->te_debug->append("Запуск гонки с синхронизацией через мьютексы...");
    }

    // Сброс счетчика завершений и значения числа
    number = 0;
    countFinish = 0;

    uint32_t initNum = ui->sb_initNum->value();
    bool useMutex = ui->rb_mutexOn->isChecked();

    // Запускаем параллельное выполнение DoWork для concurRace1 и concurRace2
    QFuture<void> future1 = QtConcurrent::run([=]() {
        concurRace1->DoWork(&number, useMutex, initNum);
    });

    QFuture<void> future2 = QtConcurrent::run([=]() {
        concurRace2->DoWork(&number, useMutex, initNum);
    });

    // Ждем завершения обоих потоков
    QFutureWatcher<void>* watcher1 = new QFutureWatcher<void>(this);
    QFutureWatcher<void>* watcher2 = new QFutureWatcher<void>(this);

    connect(watcher1, &QFutureWatcher<void>::finished, this, [=]() {
        handleRaceFinish();
        watcher1->deleteLater();
    });

    connect(watcher2, &QFutureWatcher<void>::finished, this, [=]() {
        handleRaceFinish();
        watcher2->deleteLater();
    });

    watcher1->setFuture(future1);
    watcher2->setFuture(future2);
}

// Обработка кнопки "Старт"
void MainWindow::on_pb_start_clicked() {
    ui->pb_start->setEnabled(false);
    StartRace();
}

// Дополнительный метод для обработки завершения гонки
void MainWindow::handleRaceFinish() {
    countFinish++;
    if (countFinish == 2) { // Проверяем, завершены ли оба потока
        ui->te_debug->append("Искомое число равно: " + QString::number(number) +
                             ", а должно быть " + QString::number(ui->sb_initNum->value() * 2));
        ui->pb_start->setEnabled(true);
    }
}


