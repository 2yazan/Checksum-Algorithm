#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Radio buttons for CRC types
    crc8RadioButton = new QRadioButton("CRC8", this);
    crc16RadioButton = new QRadioButton("CRC16", this);
    crc32RadioButton = new QRadioButton("CRC32", this);
    crc64RadioButton = new QRadioButton("CRC64", this);

    // Radio buttons for input types
    stringInputRadioButton = new QRadioButton("Use String", this);
    fileInputRadioButton = new QRadioButton("Use File", this);

    // Create button groups to separate the radio buttons
    QButtonGroup *crcTypeGroup = new QButtonGroup(this);
    crcTypeGroup->addButton(crc8RadioButton);
    crcTypeGroup->addButton(crc16RadioButton);
    crcTypeGroup->addButton(crc32RadioButton);
    crcTypeGroup->addButton(crc64RadioButton);

    QButtonGroup *inputTypeGroup = new QButtonGroup(this);
    inputTypeGroup->addButton(stringInputRadioButton);
    inputTypeGroup->addButton(fileInputRadioButton);

    // Checkboxes for result format
    binaryCheckBox = new QCheckBox("Binary System", this);
    decimalCheckBox = new QCheckBox("Decimal System", this);
    hexadecimalCheckBox = new QCheckBox("Hexadecimal System", this);

    // Input fields
    inputLineEdit = new QLineEdit(this);
    fileLineEdit = new QLineEdit(this);

    // Buttons
    calculateButton = new QPushButton("Calculate", this);
    browseButton = new QPushButton("Browse", this);
    copyButton = new QPushButton("Copy Results", this);
    downloadButton = new QPushButton("Download Results", this);
    resetButton = new QPushButton("Reset", this);

    // Replace QLabel with QTextEdit for results
    resultTextEdit = new QTextEdit(this);
    resultTextEdit->setReadOnly(true);
    resultTextEdit->setPlaceholderText("Results will appear here");

    // Layout setup
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // CRC Type selection (horizontal)
    QHBoxLayout *crcTypeLayout = new QHBoxLayout;
    crcTypeLayout->addWidget(new QLabel("Select CRC Type:", this));
    crcTypeLayout->addWidget(crc8RadioButton);
    crcTypeLayout->addWidget(crc16RadioButton);
    crcTypeLayout->addWidget(crc32RadioButton);
    crcTypeLayout->addWidget(crc64RadioButton);
    crcTypeLayout->addStretch();
    mainLayout->addLayout(crcTypeLayout);

    // Input Type selection (horizontal)
    QHBoxLayout *inputTypeLayout = new QHBoxLayout;
    inputTypeLayout->addWidget(new QLabel("Select Input Type:", this));
    inputTypeLayout->addWidget(stringInputRadioButton);
    inputTypeLayout->addWidget(fileInputRadioButton);
    inputTypeLayout->addStretch();
    mainLayout->addLayout(inputTypeLayout);

    mainLayout->addWidget(new QLabel("Enter String:", this));
    mainLayout->addWidget(inputLineEdit);
    mainLayout->addWidget(new QLabel("Enter File:", this));

    QHBoxLayout *fileInputLayout = new QHBoxLayout;
    fileInputLayout->addWidget(fileLineEdit);
    fileInputLayout->addWidget(browseButton);
    mainLayout->addLayout(fileInputLayout);

    mainLayout->addWidget(new QLabel("Result Format:", this));
    mainLayout->addWidget(binaryCheckBox);
    mainLayout->addWidget(decimalCheckBox);
    mainLayout->addWidget(hexadecimalCheckBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addWidget(downloadButton);
    buttonLayout->addWidget(resetButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addWidget(new QLabel("Results:", this));
    mainLayout->addWidget(resultTextEdit);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setFixedSize(640, 500);

    crcCalculator = new CrcCalculator();

    // Connect signals to slots for real-time updates
    connect(crc8RadioButton, SIGNAL(toggled(bool)), this, SLOT(updateResult()));
    connect(crc16RadioButton, SIGNAL(toggled(bool)), this, SLOT(updateResult()));
    connect(crc32RadioButton, SIGNAL(toggled(bool)), this, SLOT(updateResult()));
    connect(crc64RadioButton, SIGNAL(toggled(bool)), this, SLOT(updateResult()));

    connect(stringInputRadioButton, SIGNAL(toggled(bool)), this, SLOT(updateInputType()));
    connect(fileInputRadioButton, SIGNAL(toggled(bool)), this, SLOT(updateInputType()));

    connect(binaryCheckBox, SIGNAL(clicked()), this, SLOT(updateResult()));
    connect(decimalCheckBox, SIGNAL(clicked()), this, SLOT(updateResult()));
    connect(hexadecimalCheckBox, SIGNAL(clicked()), this, SLOT(updateResult()));

    connect(calculateButton, SIGNAL(clicked()), this, SLOT(calculateCrc()));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copyToClipboard()));
    connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadResults()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetForm()));

    // Add connections for input changes
    connect(inputLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateResult()));
    connect(fileLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateResult()));
//    connect(inputLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateResult()));
//    connect(fileLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(updateResult()));

    // Default input type settings
    stringInputRadioButton->setChecked(true); // Default to string input

    binaryCheckBox->setChecked(true);
    decimalCheckBox->setChecked(true);
    hexadecimalCheckBox->setChecked(true);

    updateInputType(); // Initialize input fields
}

void MainWindow::updateInputType()
{
    if (stringInputRadioButton->isChecked()) {
        inputLineEdit->setEnabled(true);
        fileLineEdit->setEnabled(false);
        browseButton->setEnabled(false);
    } else if (fileInputRadioButton->isChecked()) {
        inputLineEdit->setEnabled(false);
        fileLineEdit->setEnabled(true);
        browseButton->setEnabled(true);
    }
    updateResult();
}

void MainWindow::updateResult()
{
    if (stringInputRadioButton->isChecked() && inputLineEdit->text().isEmpty()) {
        resultTextEdit->setPlaceholderText("Enter a string and select CRC type to see results");
        return;
    }

    if (fileInputRadioButton->isChecked() && fileLineEdit->text().isEmpty()) {
        resultTextEdit->setPlaceholderText("Select a file and CRC type to see results");
        return;
    }

    calculateCrc();
}

void MainWindow::calculateCrc()
{
    QByteArray inputData;

    // Check whether to use string or file input
    if (stringInputRadioButton->isChecked()) {
        if (inputLineEdit->text().isEmpty()) {
            return;  // Don't show an error, just return
        }
        inputData = inputLineEdit->text().toUtf8();
    }
    else if (fileInputRadioButton->isChecked()) {
        if (fileLineEdit->text().isEmpty()) {
            return;  // Don't show an error, just return
        }
        QFile file(fileLineEdit->text());
        if (!file.open(QIODevice::ReadOnly)) {
            resultTextEdit->setPlaceholderText("Error opening file");
            return;
        }
        inputData = file.readAll();
    }
    else {
        return;  // Don't show an error, just return
    }

    QString result;

    // Check which CRC type is selected
    if (crc8RadioButton->isChecked()) {
        unsigned char crc8Result = crcCalculator->calculateCrc8(inputData);

        if (binaryCheckBox->isChecked())
            result += QString("CRC-8 (Binary): %1\n").arg(QString::number(crc8Result, 2).rightJustified(8, '0'));
        if (decimalCheckBox->isChecked())
            result += QString("CRC-8 (Decimal): %1\n").arg(QString::number(crc8Result));
        if (hexadecimalCheckBox->isChecked())
            result += QString("CRC-8 (Hexadecimal): 0x%1\n").arg(QString::number(crc8Result, 16).toUpper().rightJustified(2, '0'));
    }
    else if (crc16RadioButton->isChecked()) {
        uint16_t crc16Result = crcCalculator->calculateCrc16(inputData);

        if (binaryCheckBox->isChecked())
            result += QString("CRC-16 (Binary): %1\n").arg(QString::number(crc16Result, 2).rightJustified(16, '0'));
        if (decimalCheckBox->isChecked())
            result += QString("CRC-16 (Decimal): %1\n").arg(QString::number(crc16Result));
        if (hexadecimalCheckBox->isChecked())
            result += QString("CRC-16 (Hexadecimal): 0x%1\n").arg(QString::number(crc16Result, 16).toUpper().rightJustified(4, '0'));
    }
    else if (crc32RadioButton->isChecked()) {
        uint32_t crc32Result = crcCalculator->calculateCrc32(inputData);

        if (binaryCheckBox->isChecked())
            result += QString("CRC-32 (Binary): %1\n").arg(QString::number(crc32Result, 2).rightJustified(32, '0'));
        if (decimalCheckBox->isChecked())
            result += QString("CRC-32 (Decimal): %1\n").arg(QString::number(crc32Result));
        if (hexadecimalCheckBox->isChecked())
            result += QString("CRC-32 (Hexadecimal): 0x%1\n").arg(QString::number(crc32Result, 16).toUpper().rightJustified(8, '0'));
    }
    else if (crc64RadioButton->isChecked()) {
        uint64_t crc64Result = crcCalculator->calculateCrc64(inputData);

        if (binaryCheckBox->isChecked())
            result += QString("CRC-64 (Binary): %1\n").arg(QString::number(crc64Result, 2).rightJustified(64, '0'));
        if (decimalCheckBox->isChecked())
            result += QString("CRC-64 (Decimal): %1\n").arg(QString::number(crc64Result));
        if (hexadecimalCheckBox->isChecked())
            result += QString("CRC-64 (Hexadecimal): 0x%1\n").arg(QString::number(crc64Result, 16).toUpper().rightJustified(16, '0'));
    }
    else {
        resultTextEdit->setPlaceholderText("Select a CRC type to see results");
        return;
    }

    if (result.isEmpty()) {
        resultTextEdit->setPlaceholderText("Select at least one result format");
    } else {
        resultTextEdit->setText(result);
    }
    currentResult = result;
}

void MainWindow::browseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    fileLineEdit->setText(fileName);
}

void MainWindow::copyToClipboard()
{
    QApplication::clipboard()->setText(currentResult);
}

void MainWindow::downloadResults()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Text Files (*.txt)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&file);
            stream << currentResult;
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Error saving file.");
        }
    }
}

void MainWindow::resetForm()
{
    // Reset all fields to their default states
    crc8RadioButton->setAutoExclusive(false);
    crc16RadioButton->setAutoExclusive(false);
    crc32RadioButton->setAutoExclusive(false);
    crc64RadioButton->setAutoExclusive(false);

    crc8RadioButton->setChecked(false);
    crc16RadioButton->setChecked(false);
    crc32RadioButton->setChecked(false);
    crc64RadioButton->setChecked(false);

    crc8RadioButton->setAutoExclusive(true);
    crc16RadioButton->setAutoExclusive(true);
    crc32RadioButton->setAutoExclusive(true);
    crc64RadioButton->setAutoExclusive(true);

    stringInputRadioButton->setChecked(true);
    inputLineEdit->clear();
    fileLineEdit->clear();

    binaryCheckBox->setChecked(true);
    decimalCheckBox->setChecked(true);
    hexadecimalCheckBox->setChecked(true);

    resultTextEdit->clear();
    resultTextEdit->setPlaceholderText("Results will appear here");

    currentResult.clear();

    updateInputType();
}
