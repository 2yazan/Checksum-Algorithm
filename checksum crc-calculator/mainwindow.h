#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QFile>
#include <QFileDialog>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QTextStream>
#include <QStandardPaths>
#include <QButtonGroup>
#include <QTextEdit>
#include "crc.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void calculateCrc();
    void browseFile();
    void copyToClipboard();
    void updateResult();
    void downloadResults();
    void resetForm();
    void updateInputType();
//    void clearResult();

private:
    QRadioButton *crc8RadioButton;
    QRadioButton *crc16RadioButton;
    QRadioButton *crc32RadioButton;
    QRadioButton *crc64RadioButton;
    QRadioButton *stringInputRadioButton;
    QRadioButton *fileInputRadioButton;


    QCheckBox *binaryCheckBox;
    QCheckBox *decimalCheckBox;
    QCheckBox *hexadecimalCheckBox;

    QLineEdit *inputLineEdit;
    QLineEdit *fileLineEdit;

    QPushButton *calculateButton;
    QPushButton *browseButton;
    QPushButton *copyButton;
    QPushButton *downloadButton;
    QPushButton *resetButton;

    QLabel *resultLabel;

    CrcCalculator *crcCalculator;

    QString currentResult;

    QTextEdit *resultTextEdit;

    bool showBinaryResult;
    bool showDecimalResult;
    bool showHexadecimalResult;
};

#endif // MAINWINDOW_H
