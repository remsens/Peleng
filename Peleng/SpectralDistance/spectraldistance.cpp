#include "spectraldistance.h"


SpectralDistance::SpectralDistance(QObject *parent)
{
    is_cubemap_emty = true;
}

SpectralDistance::~SpectralDistance()
{
   // delete window;
//    delete engine;
//    Destroy();
}

void SpectralDistance::callMethod(int methNumber)
{
    // учесть, что может быть внешний загруженный спектр. Тогда тип double
    // узнать, внешний загруженный спектр или нет m_attr->GetExternalSpectrFlag()
    qDebug() << methNumber;
    switch (methNumber) {
    case 0:{
        CalcEvklidDistance(m_attr->GetPointsList().at(0).x, m_attr->GetPointsList().at(0).y);
        return;
    }
    case 1:{
        CalcSpectralAngle(m_attr->GetPointsList().at(0).x, m_attr->GetPointsList().at(0).y);
        return;
    }
    case 2:{
        CalcSpectralCorellation(m_attr->GetPointsList().at(0).x, m_attr->GetPointsList().at(0).y);
        return;
    }
    default:
        return;
    }
}

void SpectralDistance::OnCloseEvent(QQuickCloseEvent*)
{
  //  delete window;
//    delete engine;
//    Destroy();
}

void SpectralDistance::Destroy()
{
    for (int i = 0; i < cube_map.size(); i++)
    {
        cube_map[i].clear();
    }
    cube_map.clear();
    for (int i = 0; i < masked_map.size(); i++)
    {
        masked_map[i].clear();
    }
    masked_map.clear();
    for (uint i = 0; i < new_map.size(); i++)
    {
        new_map[i].clear();
    }
    new_map.clear();
}

void SpectralDistance::Execute(HyperCube *cube, Attributes *attr)
{
    engine = new QQmlApplicationEngine(this);
    engine->load(QUrl("qrc:/sdistancewin.qml"));
    window = qobject_cast<QQuickWindow*>(engine->rootObjects().value(0));
    //window->setIcon();

    connect(window, SIGNAL(closing(QQuickCloseEvent*)), this, SLOT(OnCloseEvent(QQuickCloseEvent*)));
    connect(engine->rootObjects().value(0), SIGNAL(calcMeth(int)), this, SLOT(callMethod(int)));
    //connect(this, SIGNAL(progressPercentChanged()), engine->rootObjects().value(0), SLOT(processPercent));
    window->show();
    m_pHyperCube = cube;
    m_attr = attr;
    preview_2d = new Preview2D(0); // можно не удалять. удалится при закрытии
}

void SpectralDistance::CalcEvklidDistance(int k, int l)
{
    int execute_time = GetTickCount();
    int chan_count = m_pHyperCube->GetCountofChannels();
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();
    cube_map.resize(line_count);

    max_value = 0;
    min_value = 10000000;
    // почему до line_count - 1?
    for (int i=0; i < line_count - 1; i++)
    {
        cube_map[i].resize(row_count);
        for (int j=0; j < row_count - 1; j++)
        {
            double spectral_distance = 0.0;
            if ((k==i)||(l==j))
            {
                // если внешний спектр, то не нужна эта проверка
                continue;
            }
            for (int z=0; z < chan_count - 1; z++)
            {
                short int *data_ptr = static_cast<short int*>(m_pHyperCube->GetDataCube()[z]);
                spectral_distance +=pow(data_ptr[j*line_count + i] - data_ptr[l * line_count + k], 2);
            }
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
        }
    }
    qDebug() << "min:" << min_value;
    qDebug() << "max:" << max_value;
    qDebug() << "execute time" << (GetTickCount() - execute_time) / 1000.0;
    is_cubemap_emty = false;
    selectRange(5);
}

void SpectralDistance::CalcSpectralAngle(int k, int l)
{
    int chan_count = m_pHyperCube->GetCountofChannels();
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();
    cube_map.resize(line_count);

    max_value = 0;
    min_value = 10000000;

    for (int i=0; i < line_count; i ++)
    {
        cube_map[i].resize(row_count);
        for (int j=0; j < row_count; j++)
        {
            if ((k==i)||(l==j))
            {
                continue;
            }
            double local_val1 = 0;
            double local_val2 = 0;
            double local_val3 = 0;
            for (int z = 0; z < chan_count; z++)
            {
                short int *data_ptr = static_cast<short int*>(m_pHyperCube->GetDataCube()[z]);
                local_val1 += data_ptr[j * line_count + i] * data_ptr[l*line_count + k];
                local_val2 += pow(data_ptr[j * line_count + i],2);
                local_val3 += pow(data_ptr[l*line_count + k],2);
            }
            cube_map[i][j] = acos(local_val1 / (sqrt(local_val2) * sqrt(local_val3)));
            if (cube_map[i][j] > max_value)
            {
               max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
               min_value = cube_map[i][j];
            }
        }
    }
    qDebug() << "min:" << min_value;
    qDebug() << "max:" << max_value;
    is_cubemap_emty = false;
    selectRange(5);
}

void SpectralDistance::CalcSpectralCorellation(int k, int l)
{
    int chan_count = m_pHyperCube->GetCountofChannels();
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();
    cube_map.resize(line_count);
    max_value = 0;
    min_value = 10000000;

    double average_kl = averageSpectralValue(k,l);

    for (int i=0; i < line_count; i++)
    {
        cube_map[i].resize(row_count);
        for (int j = 0; j < row_count; j++)
        {
            if ((k==i)||(l==j))
            {
                continue;
            }
            double local_val1 = 0;
            double local_val2 = 0;
            double local_val3 = 0;
            double average_ij = averageSpectralValue(i,j);
            for (int z = 0; z < chan_count; z++)
            {
                short int *data_ptr = static_cast<short int*>(m_pHyperCube->GetDataCube()[z]);
                local_val1 += (data_ptr[j*line_count + i] - average_ij) *
                              (data_ptr[l*line_count + k] - average_kl);
                local_val2 += pow(data_ptr[j*line_count + i] - average_ij, 2);
                local_val3 += pow(data_ptr[l*line_count + k] - average_kl, 2);
            }
            cube_map[i][j] = local_val1 / (sqrt(local_val2) * sqrt(local_val3));
            if (cube_map[i][j] > max_value)
            {
               max_value = cube_map[i][j];
            }
            if (cube_map[i][j] < min_value)
            {
               min_value = cube_map[i][j];
            }
        }
    }

    is_cubemap_emty = false;
    selectRange(5);

}


void SpectralDistance::selectRange(const double percent)
{
    if (!is_cubemap_emty)
    {
    //int chan_count = m_pHyperCube->GetCountofChannels();
    int line_count = m_pHyperCube->GetLines();
    int row_count  = m_pHyperCube->GetColumns();

    double dist_range = min_value + (max_value - min_value)*(percent / 100.0);
    double* view_mem = (double*)malloc(sizeof(double) * line_count*row_count);

    masked_map.resize(line_count);
    new_map.resize(line_count);
    for (int i = 0; i < cube_map.length(); i++)
    {
        masked_map[i].resize(row_count);
        new_map[i].resize(row_count);
        for (int j=0; j < cube_map[i].length(); j++)
        {
            if (cube_map[i][j] <= dist_range)
            {
                masked_map[i][j] = round(cube_map[i][j]);
                new_map[i][j] = round(cube_map[i][j]);
                view_mem[i + cube_map.length() * j] = 255; //round(cube_map[i][j]);
            } else
            {
                masked_map[i][j] = 0;
                new_map[i][j] = 0;
                view_mem[i + cube_map.length() * j] = 0;
            }
        }
    }
    preview_2d->Plot(view_mem, line_count, row_count);
    free(view_mem);
    }
}

double SpectralDistance::averageSpectralValue(const int _i, const int _j)
{
    int chan_count = m_pHyperCube->GetCountofChannels();
    int line_count = m_pHyperCube->GetLines();


    double chanel_sum = 0;
    for (int z=0; z < chan_count; z++)
    {
        short int *data_ptr = static_cast<short int*>(m_pHyperCube->GetDataCube()[z]);
        chanel_sum += data_ptr[_j*line_count + _i];
    }
    return chanel_sum / (double)chan_count;
}

QObject *SpectralDistance::GetObjectPointer()
{
    return this;
}
