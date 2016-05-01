#include "FileProjectWindow.h"
#include "ui_FileProjectWindow.h"

#include <QFileDialog>
#include <QTextStream>
#include "QXmlStreamWriter"
#include <QMessageBox>

FileProjectWindow::FileProjectWindow(Attributes* attr, QDialog *parent)
    : QDialog(parent)
    , m_ui(new Ui::FileProjectWindow)
    , m_attr(attr)
{
    m_ui->setupUi(this);
    m_spectralLibDir = "D:/";
    m_ui->lineEdit_spectralLibs->setText(m_spectralLibDir);
    setAttribute(Qt::WA_DeleteOnClose, true);
    QObject::connect(m_ui->pushButton_openFileHeader, SIGNAL(clicked()), this, SLOT(OpenHeaderFile()));
    QObject::connect(m_ui->pushButton_openTempDir, SIGNAL(clicked()), this, SLOT(ChooseTempDir()));
    QObject::connect(m_ui->pushButton_spectralLibs, SIGNAL(clicked()), this, SLOT(ChooseSpectralLibDir()));
    QObject::connect(m_ui->pushButton_CreateFileProject, SIGNAL(clicked()), this, SLOT(CreateFileProject()));
}

FileProjectWindow::~FileProjectWindow()
{
    delete m_ui;
}

void FileProjectWindow::OpenHeaderFile()
{
    m_headerFile = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "");
    m_ui->lineEdit_fileHEader->setText(m_headerFile);
    QFileInfo file(m_headerFile);
    m_tempDir = file.dir().absolutePath();
    m_ui->lineEdit_tempDir->setText(m_tempDir);
}
void FileProjectWindow::ChooseTempDir()
{
    m_tempDir = QFileDialog::getExistingDirectory(this, tr("Директория для хранения временных файлов"), m_tempDir, QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);

}
void FileProjectWindow::ChooseSpectralLibDir()
{
    m_spectralLibDir = QFileDialog::getExistingDirectory(this, tr("Выбрать директорию спектральных библиотек"), m_tempDir, QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
}
void FileProjectWindow::CreateFileProject()
{
    m_projectName = m_ui->lineEdit_fileName->text();
    QString fileName = m_tempDir + "/" + m_projectName + ".xml";
    QFile file(fileName);
    QDomDocument doc("File_Project");
    QDomElement rootElement = doc.createElement("project");

    QDomElement headerPathElement = doc.createElement("headerPath");
    QDomText headerPathText = doc.createTextNode(m_headerFile);
    headerPathElement.appendChild(headerPathText);

    QDomElement tempPathElement = doc.createElement("tempPath");
    QDomText tempPathText = doc.createTextNode(m_tempDir);
    tempPathElement.appendChild(tempPathText);

    QDomElement spectralLibPathElement = doc.createElement("spectralLib");
    QDomText spectralLibPathText = doc.createTextNode(m_spectralLibDir);
    spectralLibPathElement.appendChild(spectralLibPathText);

    doc.appendChild(rootElement);
    rootElement.appendChild(headerPathElement);
    rootElement.appendChild(tempPathElement);
    rootElement.appendChild(spectralLibPathElement);
    //rootElement.appendChild(stepsElement);
    if (file.exists())
    {
        int n = QMessageBox::warning(this, "Предупреждение", "Файл будет перезаписан", "Yes", "No", 0, 1);
        if (n)
        {
            this->close();
            return;
        }
    }
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream(&file) <<  doc.toString();
        file.close();
    }
    QFileInfo file1(fileName);
    emit SendProjectFilePath(file1.absoluteFilePath());
    this->close();
}

