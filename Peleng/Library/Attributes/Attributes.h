#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H


#include <QObject>
#include <QMap>
#include <QString>
#include <QList>
#include <../Library/PointStruct.h>
#include <../Library/Spectr.h>

class Attributes;
#include "../Library/Interfaces/ProcessingPluginInterface.h"

enum NoiseAlgorithm {
    Median2D, Median1D, Savitski_Golay1D
};

class Attributes
{
public:
    static Attributes *I();
    static void Destroy();
    virtual ~Attributes();

    // пути к каталогам
    void SetTempPath(QString& path);
    QString GetTempPath() const;
    // путь к спектральным библиотекам
    void SetSpectralLibPath(QString& path);
    QString GetSpectralLibPath() const;

    // путь к файлу-заголовка
    void SetHeaderPath(QString& path);
    QString GetHeaderPath() const;

    // путь к файлу-проекта (для записи шагов)
    void SetFilePathProject(QString& path);
    QString GetFileProjectPath() const;

    // список шагов, который хранит описания шагов до сохранения.
    void SetStep(const QString& stepDescription);
    QStringList& GetStepsList();
    void CleasrStepsList();

    // шаг итерации.
    void IncrementStep();
    void DecrementStep();
    u::uint32 GetStep() const;

    // общее
    void SetPointsList(const Point &point);
    void SetPoint(u::uint32 x, u::uint32 y, u::uint32 z);
    void ClearList();
    void SetAvailablePlugins(const QMap<QString, ProcessingPluginsInterface*> availablePlugins);

    // спектральные библиотеки
    void SetModeLib(bool value);
    bool GetModeLib() const;

    void SetCurrentSpectr(Spectr *spectr);
    Spectr* GetCurrentSpectr() const;

    // для простоты вызова в Plotter
    void SetCurrentXUnits(const QVector<double>& xUnits);
    QVector<double> GetCurrentXUnits();

    void SetCurrentYUnits(const QVector<double>& yUnits);
    QVector<double> GetCurrentYUnits();

    void SetCurrentTitle(const QString& title);
    QString GetCurrentTitle() const;

    //SpectrPlotter
    void SetExternalSpectrFlag(bool externalSpectr);
    bool GetExternalSpectrFlag() const;
    void SetSPlotterYtitle(QString Yunits);
    QString GetSPlotterYtitle() const;

    // общее
    const QList<Point>& GetPointsList() const;
    const QMap<QString, ProcessingPluginsInterface*>& GetAvailablePlugins() const;

    // шумы
    void SetNoiseAlg(NoiseAlgorithm alg);
    NoiseAlgorithm GetNoiseAlg() const;
    void SetApplyToAllCube(bool apply);
    bool GetApplyToAllCube() const;

    void SetMaskPixelsCount(const u::uint32 pixelsNumber);
    u::uint32 GetMaskPixelsCount() const;
    // только для алгоритма Савитского-Голау
    void SetDegreePolinom(u::uint8 degree);
    u::uint8 GetDegreePolinom() const;

    //2D куб - Гистограмма
    void SetTempChanel(double *pChan);
    double *GetTempChanel();
private:
    Attributes();
    Attributes(const Attributes&);
    Attributes& operator=(Attributes&);

private:
    static Attributes* m_instance;

private:
    QString m_tempDir;
    QString m_spectralLibDir;
    QString m_headerPath;
    QString m_fileProjectPath;

    QStringList m_stepsList;
    u::uint32 m_step;
    QList<Point> m_pointsList;
    QMap<QString, ProcessingPluginsInterface*> m_availablePlugins;

    bool m_addSpectr; //! 0 - создать новый, 1 - выгрузить из библиотеки
    QVector<double> m_currentXUnits;
    QVector<double> m_currentYUnits;
    QString m_currentTitle;

    bool m_externalSpectr; //! флаг, внешний спектр или спектр с куба

    NoiseAlgorithm m_noiseAlg; //! алгоритм удаления шумов
    int m_maskPixelsCount;
    bool m_applyToAllCube;
    u::uint8 m_degreePolinom;
    double* m_tempChan; //! указатель на временный канал из 2D модуля
    QString m_SPlotter_Ytitle; //! подпись по оси Y в SpectrPlotter
    Spectr* m_spectr;
};

#endif // ATTRIBUTES_H
