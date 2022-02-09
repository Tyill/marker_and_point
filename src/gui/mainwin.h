#ifndef MAINWIN_H
#define MAINWIN_H
#include <QtWidgets/QMainWindow>
#include <QFuture>
#include "ui_mainwin.h"

struct config{
    int readPntCnt = 1000;
    int maxShowPntCnt = 16000;
    int pntSzByte = 4; // byte x+y
};

class MainWin : public QMainWindow{
    Q_OBJECT

public:
    MainWin(QWidget *parent = 0);
    ~MainWin();
        
private:
    Ui::MainWinClass ui;
          
    bool _isLoadPoints = false,
         _fClose = false;
    
    config _cng;

    QFuture<void> _futPoints;

    void connects(); 
};

#endif // MAINWIN_H
