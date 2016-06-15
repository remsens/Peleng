#include "spectraldistance.h"

#include <QProgressBar>
#include "../Library/Spectr.h"
#include <math.h>
#include <QDebug>

SpectralDistance::SpectralDistance(QObject *parent)
{
    is_cubemap_emty = true;
    view_range = 50;
    m_specWindow = NULL;
    preview_2d = NULL;
}

SpectralDistance::~SpectralDistance()
{
}

void SpectralDistance::callMethod(int methNumber)
{
    switch (methNumber) {
    case 0:{
        CalcEvklidDistance();
        return;
    }
    case 1:{
        CalcSpectralAngle();
        return;
    }
    case 2:{
        CalcSpectralCorellation();
        return;
    }
    case 3:{
        CalcSpectralDistance();
        return;
    }
    case 4:{
        CalcSID();
        return;
    }
    case 5:{
        CalcEntropy();
        return;
    }
    default:
        return;
    }
}



void SpectralDistance::Destroy()
{
    for (int i = 0; i < cube_map.size(); i++)
    {
        cube_map[i].clear();
    }
    cube_map.clear();
}

void SpectralDistance::onClosePreview()
{
    preview_2d = NULL;
}

void SpectralDistance::Execute(HyperCube *cube, Attributes *attr)
{
    if(m_specWindow == NULL)
    {
        m_specWindow = new SpecDistWindow();
        m_specWindow->setWindowIcon(QIcon(":/IconsCube/iconsCube/distance.png"));
        connect(m_specWindow, SIGNAL(calcMeth(int)), this, SLOT(callMethod(int)));
        connect(m_specWindow, SIGNAL(rangeChanged(int)), this, SLOT(changeRange(int)));
    }
    if(preview_2d == NULL)
    {
        preview_2d = new Preview2D(false);
        connect(preview_2d, SIGNAL(destroyed()), this, SLOT(onClosePreview()));
    }
    m_specWindow->show();
    m_specWindow->raise();
    m_specWindow->showNormal();// если окно было свернуто
    m_pHyperCube = cube;
    m_attr = attr;
}

void SpectralDistance::normSpectr(QVector<double>& dataSpectr)
{
    QVector<double> sortArr;
    sortArr = dataSpectr;
    qSort(sortArr);
    double max = sortArr.last();
    for (int k = 0; k < dataSpectr.size(); k++)
    {
        dataSpectr[k] /= max;
    }
}

void SpectralDistance::CalcEvklidDistance()
{
    int execute_time = GetTickCount();
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    cube_map.clear();
    cube_map.resize(line_count);

    max_value = 0;
    min_value = 10000000;

    u::uint32 maxValueBar = line_count*row_count;
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится сравнение спектральных кривых...");
    QString descr = QString("Метод: Евклидово расстояние");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отмена");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();

    for (int i=0; i < line_count; i++)
    {
        cube_map[i].clear();
        cube_map[i].resize(row_count);
        for (int j=0; j < row_count; j++)
        {
            if (progressBar->wasCanceled())
            {
                delete progressBar;
                Destroy();
                return;
            }
            double spectral_distance = 0.0;
            QVector<double> dataSpectr;
            m_pHyperCube->GetSpectrumPoint(i, j, dataSpectr);
            normSpectr(dataSpectr);
            for (int z = m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave() - 1; z++)
            {
                //double value = m_pHyperCube->GetDataPoint(i, j, z);
                spectral_distance +=pow(dataSpectr.at(z)
                        - m_attr->GetCurrentSpectr()->GetCurrentDataY().at(z), 2);
            }
            dataSpectr.clear();
            spectral_distance = sqrt(spectral_distance);
            if (spectral_distance > max_value)
            {
                max_value = spectral_distance;
            }
            if (spectral_distance < min_value)
            {
                min_value = spectral_distance;
            }
            cube_map[i][j] = spectral_distance;
            double a = i*row_count + j;
            double b = a/maxValueBar*100;
            int percent = b*100/100;
            progressBar->setValue(percent);
            QApplication::processEvents();
        }
    }
    if (!progressBar->wasCanceled())
    {
        progressBar->setValue(100);
        QApplication::processEvents();
        progressBar->hide();
        delete progressBar;
    }
    qDebug() << "min:" << min_value;
    qDebug() << "max:" << max_value;
    qDebug() << "execute time" << (GetTickCount() - execute_time) / 1000.0;
    is_cubemap_emty = false;
    selectRange();
}

void SpectralDistance::CalcSpectralAngle()
{
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    cube_map.clear();
    cube_map.resize(line_count);

    max_value = 0;
    min_value = 10000000;
    u::uint32 maxValueBar = line_count*row_count;
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится сравнение спектральных кривых...");
    QString descr = QString("Метод: Спектральный угол");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отмена");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();
    for (int i=0; i < line_count; i ++)
    {
        cube_map[i].clear();
        cube_map[i].resize(row_count);
        for (int j=0; j < row_count; j++)
        {
            if (progressBar->wasCanceled())
            {
                delete progressBar;
                Destroy();
                return;
            }
            double local_val1 = 0;
            double local_val2 = 0;
            double local_val3 = 0;
            QVector<double> dataSpectr;
            m_pHyperCube->GetSpectrumPoint(i, j, dataSpectr);
            normSpectr(dataSpectr);
            for (int z = m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave(); z++)
            {
                local_val1 += dataSpectr.at(z) * m_attr->GetCurrentSpectr()->GetCurrentDataY().at(z);
                local_val2 += pow(dataSpectr.at(z),2);
                local_val3 += pow(m_attr->GetCurrentSpectr()->GetCurrentDataY().at(z),2);
            }
            dataSpectr.clear();
            cube_map[i][j] = acos(local_val1 / (sqrt(local_val2) * sqrt(local_val3)));
            if (cube_map[i][j] > max_value)
            {
                max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
                min_value = cube_map[i][j];
            }
            double a = i*row_count + j;
            double b = a/maxValueBar*100;
            int percent = b*100/100;
            progressBar->setValue(percent);
            QApplication::processEvents();
        }
    }
    if (!progressBar->wasCanceled())
    {
        progressBar->setValue(100);
        QApplication::processEvents();
        progressBar->hide();
        delete progressBar;
    }
    qDebug() << "min:" << min_value;
    qDebug() << "max:" << max_value;
    is_cubemap_emty = false;
    selectRange();
}

void SpectralDistance::CalcSpectralCorellation()
{
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    max_value = 0;
    min_value = 10000000;

    cube_map.clear();
    cube_map.resize(line_count);

    u::uint32 maxValueBar = line_count*row_count;
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится сравнение спектральных кривых...");
    QString descr = QString("Метод: Спектральная корреляция");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отмена");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();

    QVector<double> dataSpectrOrigin = m_attr->GetCurrentSpectr()->GetCurrentDataY();

    double average_kl = averageSpectralValue(dataSpectrOrigin);

    for (int i=0; i < line_count; i++)
    {
        cube_map[i].clear();
        cube_map[i].resize(row_count);
        for (int j = 0; j < row_count; j++)
        {
            if (progressBar->wasCanceled())
            {
                delete progressBar;
                Destroy();
                return;
            }
            double local_val1 = 0;
            double local_val2 = 0;
            double local_val3 = 0;
            QVector<double> dataSpectr;
            m_pHyperCube->GetSpectrumPoint(i, j, dataSpectr);
            normSpectr(dataSpectr);
            double average_ij = averageSpectralValue(dataSpectr);

            for (int z = m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave(); z++)
            {

                local_val1 += (dataSpectr.at(z) - average_ij) *
                        (dataSpectrOrigin.at(z) - average_kl);
                local_val2 += pow(dataSpectr.at(z) - average_ij, 2);
                local_val3 += pow(dataSpectrOrigin.at(z) - average_kl, 2);
            }
            dataSpectr.clear();
            cube_map[i][j] =  sqrt(1.0 - local_val1 / (sqrt(local_val2) * sqrt(local_val3)))/sqrt(2);
            if (cube_map[i][j] > max_value)
            {
                max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
                min_value = cube_map[i][j];
            }
            double a = i*row_count + j;
            double b = a/maxValueBar*100;
            int percent = b*100/100;
            progressBar->setValue(percent);
            QApplication::processEvents();
        }
    }
    if (!progressBar->wasCanceled())
    {
        progressBar->setValue(100);
        QApplication::processEvents();
        progressBar->hide();
        delete progressBar;
    }
    dataSpectrOrigin.clear();
    is_cubemap_emty = false;
    selectRange();

}

void SpectralDistance::CalcSpectralDistance()
{
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    max_value = 0;
    min_value = 10000000;

    cube_map.clear();
    cube_map.resize(line_count);

    u::uint32 maxValueBar = line_count*row_count;
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится сравнение спектральных кривых...");
    QString descr = QString("Метод: Спектральные расстояния");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отмена");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();

    QVector<double> dataSpectrOrigin = m_attr->GetCurrentSpectr()->GetCurrentDataY();

    for (int i=0; i < line_count; i++)
    {
        cube_map[i].clear();
        cube_map[i].resize(row_count);
        for (int j = 0; j < row_count; j++)
        {
            if (progressBar->wasCanceled())
            {
                delete progressBar;
                Destroy();
                return;
            }
            QVector<double> dataSpectr;
            m_pHyperCube->GetSpectrumPoint(i, j, dataSpectr);
            normSpectr(dataSpectr);
            double sum = 0;
            for (int z = m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave(); z++)
            {
                sum += abs(dataSpectr.at(z) - dataSpectrOrigin.at(z));
            }
            dataSpectr.clear();
            cube_map[i][j] =  sum;
            if (cube_map[i][j] > max_value)
            {
                max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
                min_value = cube_map[i][j];
            }
            double a = i*row_count + j;
            double b = a/maxValueBar*100;
            int percent = b*100/100;
            progressBar->setValue(percent);
            QApplication::processEvents();
        }
    }
    if (!progressBar->wasCanceled())
    {
        progressBar->setValue(100);
        QApplication::processEvents();
        progressBar->hide();
        delete progressBar;
    }
    dataSpectrOrigin.clear();
    is_cubemap_emty = false;
    selectRange();
}

void SpectralDistance::CalcSID()
{
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    max_value = 0;
    min_value = 10000000;

    cube_map.clear();
    cube_map.resize(line_count);

    u::uint32 maxValueBar = line_count*row_count;
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится сравнение спектральных кривых...");
    QString descr = QString("Метод: Спектральная информационная дивергенция");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отмена");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();

    QVector<double> dataSpectrOrigin = m_attr->GetCurrentSpectr()->GetCurrentDataY();
    double sum = 0;
    for (int i = 0; i < dataSpectrOrigin.size(); i++)
    {
        sum += dataSpectrOrigin.at(i);
    }
    for (int i = 0; i < dataSpectrOrigin.size(); i++)
    {
        dataSpectrOrigin[i] /= sum;
    }
    for (int i=0; i < line_count; i++)
    {
        cube_map[i].clear();
        cube_map[i].resize(row_count);
        for (int j = 0; j < row_count; j++)
        {
            if (progressBar->wasCanceled())
            {
                delete progressBar;
                Destroy();
                return;
            }
            QVector<double> dataSpectr;
            m_pHyperCube->GetSpectrumPoint(i, j, dataSpectr);
            normSpectr(dataSpectr);
            sum = 0;
            for (int k = 0; k < dataSpectr.size(); k++)
            {
                sum += dataSpectr.at(k);
            }
            for (int k = 0; k < dataSpectr.size(); k++)
            {
                dataSpectr[k] /= sum;
            }

            double d1 = 0;
            double d2 = 0;
            for (int z = m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave(); z++)
            {
                //double l = log2(dataSpectr.at(z)/dataSpectrOrigin.at(z));
                //qDebug() << l;
                d1 += dataSpectr.at(z)*log2(dataSpectr.at(z)/dataSpectrOrigin.at(z));
                //qDebug() << d1;
                d2 += dataSpectrOrigin.at(z)*log2(dataSpectrOrigin.at(z)/dataSpectr.at(z));
            }
            dataSpectr.clear();
            //qDebug() << d1+d2;
            cube_map[i][j] =  d1+d2;
            if (cube_map[i][j] > max_value)
            {
                max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
                min_value = cube_map[i][j];
            }
            double a = i*row_count + j;
            double b = a/maxValueBar*100;
            int percent = b*100/100;
            progressBar->setValue(percent);
            QApplication::processEvents();
        }
    }
    if (!progressBar->wasCanceled())
    {
        progressBar->setValue(100);
        QApplication::processEvents();
        progressBar->hide();
        delete progressBar;
    }
    dataSpectrOrigin.clear();
    is_cubemap_emty = false;
    selectRange();
}

void SpectralDistance::CalcEntropy()
{
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    max_value = 0;
    min_value = 10000000;

    cube_map.clear();
    cube_map.resize(line_count);

    u::uint32 maxValueBar = line_count*row_count;
    QProgressDialog* progressBar  = new QProgressDialog();
    progressBar->setLabelText("Производится сравнение спектральных кривых...");
    QString descr = QString("Метод: Спектральная информационная дивергенция");
    //progressBar->setWindowIcon(icon);
    progressBar->setWindowTitle(descr);
    progressBar->setRange(0, 100);
    progressBar->setWindowModality(Qt::WindowModal);
    progressBar->setCancelButtonText("Отмена");
    progressBar->show();
    progressBar->setValue(0);
    QApplication::processEvents();

    QVector<double> dataSpectrOrigin = m_attr->GetCurrentSpectr()->GetCurrentDataY();
    double sum = 0;
    for (int i = 0; i < dataSpectrOrigin.size(); i++)
    {
        sum += dataSpectrOrigin.at(i);
    }
    for (int i = 0; i < dataSpectrOrigin.size(); i++)
    {
        dataSpectrOrigin[i] /= sum;
    }
    for (int i=0; i < line_count; i++)
    {
        cube_map[i].clear();
        cube_map[i].resize(row_count);
        for (int j = 0; j < row_count; j++)
        {
            if (progressBar->wasCanceled())
            {
                delete progressBar;
                Destroy();
                return;
            }
            QVector<double> dataSpectr;
            m_pHyperCube->GetSpectrumPoint(i, j, dataSpectr);
            normSpectr(dataSpectr);
            sum = 0;
            for (int k = 0; k < dataSpectr.size(); k++)
            {
                sum += dataSpectr.at(k);
            }
            for (int k = 0; k < dataSpectr.size(); k++)
            {
                dataSpectr[k] /= sum;
            }

            double d1 = 0;
            double d2 = 0;
            for (int z = m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave(); z++)
            {
                //if (dataSpectrOrigin.at(z)!= 0)
                    d1 += dataSpectr.at(z)/dataSpectrOrigin.at(z);
                //else d1 += max_value;
                //if (dataSpectr.at(z) != 0)
                    d2 += dataSpectrOrigin.at(z)/dataSpectr.at(z);
                //else d2 += max_value;
            }
            dataSpectr.clear();
            cube_map[i][j] =  log2(d1*d2);
            if (cube_map[i][j] > max_value)
            {
                max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
                min_value = cube_map[i][j];
            }
            double a = i*row_count + j;
            double b = a/maxValueBar*100;
            int percent = b*100/100;
            progressBar->setValue(percent);
            QApplication::processEvents();
        }
    }
    if (!progressBar->wasCanceled())
    {
        progressBar->setValue(100);
        QApplication::processEvents();
        progressBar->hide();
        delete progressBar;
    }
    dataSpectrOrigin.clear();
    is_cubemap_emty = false;
    selectRange();
}

void SpectralDistance::selectRange()
{
    if (!is_cubemap_emty)
    {
        //int chan_count = m_pHyperCube->GetCountofChannels();
        int line_count = m_pHyperCube->GetLines();
        int row_count  = m_pHyperCube->GetColumns();

        double dist_range = min_value + (max_value - min_value)*((100.0 - view_range) / 100.0);
        qDebug() << "dist: " << dist_range;
        double* view_mem = (double*)malloc(sizeof(double) * line_count*row_count);
        for (int i = 0; i < cube_map.length(); i++)
        {
            for (int j=0; j < cube_map[i].length(); j++)
            {
                qDebug() << cube_map[i][j];
                    if (cube_map[i][j] <= dist_range)
                    {
                        view_mem[j + cube_map[i].length() * i] = 1;
                    } else
                    {
                        view_mem[j + cube_map[i].length() * i] = 0;
                    }
            }
        }

        if(preview_2d == NULL)
        {
            preview_2d = new Preview2D(false);
            connect(preview_2d, SIGNAL(destroyed()), this, SLOT(onClosePreview()));
        }
        QString title = "Сравнение спектральных кривых. Исходный спектр: " + m_attr->GetCurrentSpectr()->GetTitle();
        preview_2d->Plot(view_mem, line_count, row_count, title, m_pHyperCube, m_attr);
        if (!m_attr->GetExternalSpectrFlag())
        {
            QVector<double> x;
            QVector<double> y;
            x.append(m_attr->GetCurrentSpectr()->GetXCoord());
            y.append(m_attr->GetCurrentSpectr()->GetYCoord());
            preview_2d->plotPointsOn2D(x,y);
        }
        free(view_mem);

    }
}

void SpectralDistance::changeRange(const int range)
{
    view_range = range;
    selectRange();
}

double SpectralDistance::averageSpectralValue(const QVector<double> &dataSpectr)
{
    double chanel_sum = 0;
    for (int z=m_attr->GetStartRangeWave(); z < m_attr->GetEndRangeWave(); z++)
    {
            chanel_sum += dataSpectr.at(z);

    }
    return chanel_sum / (double)dataSpectr.size();
}

QObject *SpectralDistance::GetObjectPointer()
{
    return this;
}
