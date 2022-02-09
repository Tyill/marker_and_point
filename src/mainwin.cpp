#include "../forms/mainwin.h"
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QtConcurrent>

MainWin::MainWin(QWidget *parent)
    : QMainWindow(parent){
    ui.setupUi(this);

    this->setWindowTitle("MarkersAndPoints");

    connects();   

    qRegisterMetaType<QVector<QPoint>>("QVector<QPoint>");
}

MainWin::~MainWin(){

    _fClose = true;

    _futPoints.waitForFinished();
}

void MainWin::connects(){

    connect(ui.actionExit, &QAction::triggered, [this]() {
        close();
    });
    connect(ui.actionLoadMarkers, &QAction::triggered, [this]() {

        QString fname = QFileDialog::getOpenFileName(this,
            tr("Загрузка файла с маркерами"), "", "xml files (*.xml)");

        if (fname.isEmpty()) return;

        QFile file(fname);
        if (!file.open(QIODevice::ReadOnly)){
            ui.statusBar->showMessage(tr("Ошибка открытия файла XML: ") + fname, 3000);
            return;
        }
        QVector<QPoint> markers;

        QXmlStreamReader reader(&file);
        while (!reader.atEnd()) {
            reader.readNext();
            if (reader.isStartDocument()){
                continue;
            }
            if (reader.isEndDocument()){
                break;
            }
            QStringRef elementName = reader.name();
            if (reader.isStartElement() && (elementName == "Point")){
                auto attr = reader.attributes();
                if (attr.size() != 2){
                    ui.statusBar->showMessage(tr("XML attr не соотв значению Point"), 3000);
                    continue;
                }
                markers.push_back(QPoint(int(attr[0].value().toDouble()),
                                         int(attr[1].value().toDouble())));
            }
        }
        if (!reader.hasError()){
            ui.plot->setMarkers(qMove(markers));
        }
        else{
            ui.statusBar->showMessage(tr("XML ошибка парсинга: ") + reader.errorString(), 3000);
        }
        file.close();
        update();
    });
    connect(ui.actionLoadPoints, &QAction::triggered, [this]() {
        
        if (_isLoadPoints){
            ui.statusBar->showMessage(tr("Файл с точками уже загружен"), 3000);
            return;
        }
        QString fname = QFileDialog::getOpenFileName(this,
            tr("Загрузка файла с точками"), "", "bin files (*.bin)");

        if (fname.isEmpty()) return;
       
        _futPoints = QtConcurrent::run([fname, this](){

            QFile file(fname);
            if (!file.open(QIODevice::ReadOnly)){
                QMetaObject::invokeMethod(ui.statusBar, "showMessage", Qt::AutoConnection,
                    Q_ARG(QString, tr("Ошибка открытия bin файла: ") + fname),
                    Q_ARG(int, 3000));
                return;
            }
            _isLoadPoints = true;

            QDataStream ds(&file);

            QVector<qint16> buff(_cng.readPntCnt * 2); // 2 координаты точки
            QVector<QPoint> pnts(_cng.maxShowPntCnt);

            int pntCntForShow = 0;

            while (!_fClose){

                int readSz = 0;
                while (readSz < _cng.readPntCnt * _cng.pntSzByte){

                    readSz += ds.readRawData((char*)buff.data() + readSz, _cng.readPntCnt * _cng.pntSzByte - readSz);

                    if (ds.atEnd()){
                        ds.device()->seek(0);
                    }
                }
                for (int i = 0; i < _cng.readPntCnt; ++i){
                    pnts[pntCntForShow] = QPoint(buff[i * 2], buff[i * 2 + 1]);

                    ++pntCntForShow;
                    if (pntCntForShow == _cng.maxShowPntCnt){
                        pntCntForShow = 0;

                        QMetaObject::invokeMethod(ui.plot, "setPoints", Qt::AutoConnection, Q_ARG(QVector<QPoint>, pnts));
                    }
                }
                QThread::currentThread()->msleep(10);
            }
            file.close();    
        });        
    });
}