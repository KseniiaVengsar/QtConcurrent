#ifndef EXAMPLERACE_H
#define EXAMPLERACE_H

#include <QThread>
#include <QMutex>
#include <QObject>

class ExampleRace : public QObject {
    Q_OBJECT
    QMutex* mut;

public:

    ExampleRace(QMutex* m) : mut(m) {}

    void DoWork(uint32_t* inc, bool mutexOn, int numIterat);

signals:
    void sig_Finish();
};

#endif // EXAMPLERACE_H
