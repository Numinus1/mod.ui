#ifndef BACKEND_H
#define BACKEND_H

#include "modbusmodule.h"
#include "writebox.h"

class Backend : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject *parent = nullptr);

    QString userName();
    void setUserName(const QString &userName);

private:
    int fsm();

    int ComPing();
    int ComSweep();
    void genStableCommands();
    void genMutableCommands();
    //facilitate information flow between comm and modb

signals:

    void deviceConnected();
    void createPage(QString qmlString);
    void cleanUp();

    void createTab(QString qmlString, QString tabTitle);
    void spawnStableTabs(QList<QString> pageList, QList<QString> titleList, int pageCount);
    void spawnMutableTabs(QList<QString> pageList, QList<QString> titleList, int pageCount);
    void spawnButtonPanel(QString qmlString);

    //update values to/from UI
    void upChildVal(int box, int card, QString value);
    void getChildVal(int box, int card);
    void upWriteStatus(int status);

    //manual serial port
    void portList(QList<QString> list, int size);
    void manReport(QString pname, bool result);
    void manClosed(bool result);

public slots:
    //reset
    void ark_reset();
    void promptClean();

    //fsm
    void EXEfsm();
    void pauseComm();

    //update card transients
    void upTransient(int boxToken, int cardCount, QString ftransient);
    void upButtonTransient(int boxToken, int cardCount);
    void flushButtonTransients();
    void promptWrite();
    void fauxWrite();
    void fauxUpValues();
    //modb slots
    void startSweep();
    void refresh_statics();

    //component-loading check
    void incrementComponents();

    //file io
    void loadSetupFile(QString path);
    void altloadSetupFile(QString path);
    void loadConfigFile(QString path);
    void saveConfigFile(QString path);

    //Manual Serial Port

    void refreshPorts();
    void openCOMPort(QString portN, QString baudR,
                     QString parity, QString stopB);
    void closeCOMPort();



private:
    ParamWarehouse *storage;
    ModbusModule *modb;

    //State Holders
    int bkState;

    //true if page config loaded
    bool paged;
    bool writeTime;
    bool writeButtonTime;
    bool loadConfigTime;

    //0 -> nothing
    //1 -> try to connect
    //2 -> connected
    int connstat;
    bool pauseCommunication;

    //the next ParameterBox to query
    int liftIndex;

    //lift static params (as well as dynamic) if true
    bool refresh;
    int refreshCount = 0;

    //timer to iterate backend fsm
    QTimer *timer;

    //test
    int oneTime;
    bool lockFSM;

    //Component Completion checkers
    int componentCount;
    int componentComplete;
};

#endif // BACKEND_H
