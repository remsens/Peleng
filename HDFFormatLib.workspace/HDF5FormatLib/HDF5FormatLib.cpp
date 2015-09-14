#include "HDF5FormatLib.h"

#include "../../Library/GenericExc.h"
#include "../HDF5FormatLib/include/hdf5.h"

HDF5FormatLib::HDF5FormatLib()
{

}

HDF5FormatLib::~HDF5FormatLib()
{

}

u::logic HDF5FormatLib::WriteToFile(QString filePath, QString dataSetPath, u::cptr data, u::int64 lines, u::int64 columns, int byteType)
{
    try
    {
        QStringList listPathes = ParseFile(dataSetPath);
        if (listPathes.isEmpty())
        {
            throw GenericExc(QObject::tr("Не задан путь к данным"));
        }
        if (filePath.isEmpty())
        {
            throw GenericExc(QObject::tr("Не задан путь к файлу"));
        }
        // пробуем открыть файл. Если status = -1, то создаем файл
        hid_t fileHandle, dataspace_id, group_id, dataset_id;
        hsize_t     dims[2];
        QList<hid_t> groups_idList;
        herr_t status;
        fileHandle = H5Fopen(filePath.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        if (fileHandle < 0)
        {
            fileHandle = H5Fcreate(filePath.toStdString().c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
            if (fileHandle < 0)
            {
                throw GenericExc(QObject::tr("Ошибка создания файла"));
            }
        }
        if (listPathes.size() != 1)
        {
            QString str = "/" + listPathes.at(0);
            group_id = H5Gopen2(fileHandle, str.toStdString().c_str(), H5P_DEFAULT);
            if (group_id < 0)
            {
                group_id = H5Gcreate2(fileHandle, str.toStdString().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                if (group_id < 0)
                {
                    throw GenericExc(QObject::tr("Ошибка создания группы файла"));
                }
            }
            groups_idList.append(group_id);
            // Последнее - это название dataSet
            for (int i = 1; i < listPathes.size()-1; i++)
            {
                // сначала открываем, если не вышло, то создаем новую группу
                group_id = H5Gopen2(groups_idList.at(i-1), listPathes.at(i).toStdString().c_str(), H5P_DEFAULT);
                if (group_id < 0)
                {
                    group_id = H5Gcreate2(groups_idList.at(i-1), listPathes.at(i).toStdString().c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                    if (group_id < 0)
                    {
                        throw GenericExc(QObject::tr("Ошибка создания группы файла"));
                    }
                }
                groups_idList.append(group_id);
            }
        }

        // создаем размерность dataset
        dims[0] = lines;
        dims[1] = columns;
        dataspace_id = H5Screate_simple(2, dims, NULL);
        if (dataspace_id < 0)
        {
            throw GenericExc(QObject::tr("Ошибка выделения памяти для данных"));
        }
        dataset_id = H5Dcreate2(groups_idList.at(groups_idList.size()-1), listPathes.at(listPathes.size()-1).toStdString().c_str(), GetByteType(byteType), dataspace_id,
                               H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        if (dataset_id < 0)
        {
            throw GenericExc(QObject::tr("Ошибка создания set данных. Возможно данный сет данных уже создан"));
        }
        status = H5Dwrite(dataset_id, GetByteType(byteType), H5S_ALL, H5S_ALL, H5P_DEFAULT,
                          data);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка записи данных"));
        }

        status = H5Sclose(dataspace_id);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка установки запрета на ввод данных"));
        }

        status = H5Dclose(dataset_id);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка закрытия set данных"));
        }

        for (int i = groups_idList.size()-1; i >= 0; i-- )
        {
            status = H5Gclose(groups_idList.at(i));
            if (status < 0)
            {
                throw GenericExc(QObject::tr("Ошибка закрытия группы"));
            }
        }
        status = H5Fclose (fileHandle);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка закрытия файла"));
        }
        return true;
    } catch (const GenericExc& exc)
    {
        m_errDescription = exc.GetWhat();
        return false;
    } catch (...)
    {
        m_errDescription = QObject::tr("Неизвестная ошибка при записи в файл");
        return false;
    }
}

u::uint32 HDF5FormatLib::GetDataSetSize(QString filePath, QString dataSetPath, int byteType)
{
    try
    {
        QStringList listPathes = ParseFile(dataSetPath);
        if (listPathes.isEmpty())
        {
            throw GenericExc(QObject::tr("Не задан путь к данным"));
        }
        hid_t fileHandle, dataspace_id, group_id, dataset_id;
        QList<hid_t> groups_idList;
        herr_t status;
        fileHandle = H5Fopen(filePath.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        if (fileHandle < 0)
        {
            throw GenericExc(QObject::tr("Ошибка открытия файла"));
        }
        if (listPathes.size() != 1)
        {
            group_id = H5Gopen2(fileHandle, listPathes.at(0).toStdString().c_str(), H5P_DEFAULT);
            if (group_id < 0)
            {
                throw GenericExc(QObject::tr("Ошибка открытия группы файла"));
            } else {
                groups_idList.append(group_id);
            }
        }
            // Последнее - это название dataSet
        for (int i = 1; i < listPathes.size()-1; i++)
        {
            group_id = H5Gopen2(groups_idList.at(i-1), listPathes.at(i).toStdString().c_str(), H5P_DEFAULT);
            if (group_id < 0)
            {
                throw GenericExc(QObject::tr("Ошибка открытия группы файла"));
            }
             else
            {
                groups_idList.append(group_id);
            }
        }
        dataset_id = H5Dopen2 (groups_idList.at(groups_idList.size()-1), listPathes.at(listPathes.size()-1).toStdString().c_str(), H5P_DEFAULT);
        if (dataset_id < 0)
        {
            throw GenericExc(QObject::tr("Ошибка открытия set данных"));
        }
        dataspace_id = H5Dget_space (dataset_id);
        if (dataspace_id < 0)
        {
            throw GenericExc(QObject::tr("Ошибка получения размерности сета"));
        }
        const int ndims = H5Sget_simple_extent_ndims(dataspace_id);
        hsize_t dims[ndims];
        status = H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
        u::uint32 size = dims[0];
        for (int i = 1; i < ndims; i++)
        {
            size *= dims[i];
        }
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка получения размера данных"));
        }

        status = H5Sclose(dataspace_id);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка установки запрета на чтение данных"));
        }

        status = H5Dclose(dataset_id);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка закрытия set данных"));
        }

        for (int i = groups_idList.size()-1; i >= 0; i-- )
        {
            status = H5Gclose(groups_idList.at(i));
            if (status < 0)
            {
                throw GenericExc(QObject::tr("Ошибка закрытия группы"));
            }
        }
        status = H5Fclose (fileHandle);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка закрытия файла"));
        }
        u::uint32 sizeEl;
        switch (byteType) {
            case 1: sizeEl = 1; break;
            case 2: sizeEl = 2; break;
            case 4: sizeEl = 4; break;
            case 8: sizeEl = 8; break;
            case 5: sizeEl = 4; break;
            case 12: sizeEl = 8; break;
            default: throw GenericExc(QObject::tr("Неизвестный формат данных"));
        }
        return size*sizeEl;
    } catch (const GenericExc& exc)
    {
        m_errDescription = exc.GetWhat();
        return -1;
    } catch (...)
    {
        m_errDescription = QObject::tr("Неизвестная ошибка при получении размера файла");
        return -1;
    }
}

u::logic HDF5FormatLib::ReadFromFile(QString filePath, QString dataSetPath, u::ptr data, int byteType)
{
    try
    {
        QStringList listPathes = ParseFile(dataSetPath);
        if (listPathes.isEmpty())
        {
            throw GenericExc(QObject::tr("Не задан путь к данным"));
        }
        // пробуем открыть файл. Если status = -1, то создаем файл
        hid_t fileHandle, group_id, dataset_id;
        QList<hid_t> groups_idList;
        herr_t status;
        fileHandle = H5Fopen(filePath.toStdString().c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        if (fileHandle < 0)
        {
            throw GenericExc(QObject::tr("Ошибка открытия файла"));
        }
        if (listPathes.size() != 1)
        {
            group_id = H5Gopen2(fileHandle, listPathes.at(0).toStdString().c_str(), H5P_DEFAULT);
            if (group_id < 0)
            {
                throw GenericExc(QObject::tr("Ошибка открытия группы файла"));
            } else {
                groups_idList.append(group_id);
            }
        }
            // Последнее - это название dataSet
        for (int i = 1; i < listPathes.size()-1; i++)
        {
            group_id = H5Gopen2(groups_idList.at(i-1), listPathes.at(i).toStdString().c_str(), H5P_DEFAULT);
            if (group_id < 0)
            {
                throw GenericExc(QObject::tr("Ошибка открытия группы файла"));
            }
             else
            {
                groups_idList.append(group_id);
            }
        }
        dataset_id = H5Dopen2 (groups_idList.at(groups_idList.size()-1), listPathes.at(listPathes.size()-1).toStdString().c_str(), H5P_DEFAULT);
        if (dataset_id < 0)
        {
            throw GenericExc(QObject::tr("Ошибка открытия set данных"));
        }
        status = H5Dread (dataset_id, GetByteType(byteType), H5S_ALL, H5S_ALL,
                               H5P_DEFAULT, data);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка чтения данных"));
        }

        status = H5Dclose(dataset_id);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка закрытия set данных"));
        }

        for (int i = groups_idList.size()-1; i >= 0; i-- )
        {
            status = H5Gclose(groups_idList.at(i));
            if (status < 0)
            {
                throw GenericExc(QObject::tr("Ошибка закрытия группы"));
            }
        }
        status = H5Fclose (fileHandle);
        if (status < 0)
        {
            throw GenericExc(QObject::tr("Ошибка закрытия файла"));
        }

        return true;
    } catch (const GenericExc& exc)
    {
        m_errDescription = exc.GetWhat();
        return false;
    } catch (...)
    {
        m_errDescription = QObject::tr("Неизвестная ошибка при чтении данных из файла");
        return false;
    }
}


QString& HDF5FormatLib::GetErrorDiscription()
{
    return m_errDescription;
}


struct opdata {
    unsigned        recurs;
    struct opdata   *prev;
    haddr_t         addr;
};

static int count = 0;
static int countGroup = 0;
static QString filePathJson;

int group_check (struct opdata *od, haddr_t target_addr)
{
    if (od->addr == target_addr)
        return 1;
   else if (!od->recurs)
        return 0;
    else
        return group_check (od->prev, target_addr);
}

herr_t op_func (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data)
{
    herr_t          status, return_val = 0;
    H5O_info_t      infobuf;
    struct opdata   *od = (struct opdata *) operator_data;
    unsigned        spaces = 2*(od->recurs+1);
    FILE* pfile;
    pfile = fopen(filePathJson.toStdString().c_str(), "a");
    if (!pfile)
    {
       throw GenericExc(QObject::tr("Ошибка открытия json файла"));

    }
    status = H5Oget_info_by_name (loc_id, name, &infobuf, H5P_DEFAULT);
    if (status < 0)
    {
        throw GenericExc(QObject::tr("Ошибка получения информации о файле"));
    }
    switch (infobuf.type) {
        case H5O_TYPE_GROUP: {
            if (countGroup != 0) {
                fprintf (pfile, "{\n");
                fprintf (pfile, "%*s", spaces, "");
            }
            fprintf (pfile, "\"%s\" : ", name);
            countGroup++;
            if ( group_check (od, infobuf.addr) ) {
            }
            else {
                struct opdata nextod;
                nextod.recurs = od->recurs + 1;
                nextod.prev = od;
                nextod.addr = infobuf.addr;
                fclose(pfile);
                return_val = H5Literate_by_name (loc_id, name, H5_INDEX_NAME,
                            H5_ITER_NATIVE, NULL, op_func, (void *) &nextod,
                            H5P_DEFAULT);
            }
            pfile = fopen(filePathJson.toStdString().c_str(), "a");
            if (!pfile)
            {
               throw GenericExc(QObject::tr("Ошибка открытия json файла"));

            }
            if (count == 0) {
                fprintf (pfile, "%*s}\n", spaces, "");
            } else {
                fprintf (pfile, "\n%*s]\n", spaces, "");
                count = 0;
            }
            break;
        }
        case H5O_TYPE_DATASET:
            if (count == 0)
            {
                fprintf (pfile, "[\n");
            } else
            {
                fprintf(pfile, ",\n");
            }
            fprintf (pfile, "%*s", spaces, "");
            fprintf (pfile, "\"%s\"", name);
            count++;
            countGroup = 0;
            break;
        default:
            break;
    }
    fclose(pfile);
    return return_val;
}



u::logic HDF5FormatLib::GetHDF5StructFile(QString filePathJson1, QString filePathHdf)
{
    hid_t           file;
    herr_t          status;
    H5O_info_t      infobuf;
    struct opdata   od;
    filePathJson = filePathJson1;
    try
    {
    FILE *pfile1;
    pfile1 = fopen(filePathJson1.toStdString().c_str(), "w");
    if (!pfile1)
    {
       throw GenericExc(QObject::tr("Ошибка открытия json файла"));

    }
    fprintf(pfile1, "%s", "{\n");
    fclose(pfile1);

    file = H5Fopen (filePathHdf.toStdString().c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file < 0)
    {
        throw GenericExc(QObject::tr("Ошибка открытия файла"));
    }
    status = H5Oget_info (file, &infobuf);
    if (status < 0)
    {
        throw GenericExc(QObject::tr("Ошибка получения информации о файле"));
    }
    od.recurs = 0;
    od.prev = NULL;
    od.addr = infobuf.addr;

    status = H5Literate (file, H5_INDEX_NAME, H5_ITER_NATIVE, NULL, op_func,
                (void *) &od);
    if (status < 0)
    {
        throw GenericExc(QObject::tr("Ошибка получения структуры файла"));
    }
    status = H5Fclose (file);
    if (status < 0)
    {
        throw GenericExc(QObject::tr("Ошибка закрытия файла"));
    }
    pfile1 = fopen(filePathJson.toStdString().c_str(), "a");
    if (!pfile1)
    {
       throw GenericExc(QObject::tr("Ошибка открытия json файла"));

    }
    fprintf(pfile1, "%s", "}");
    fclose(pfile1);
    return true;
    } catch (const GenericExc& exc)
    {
        m_errDescription = exc.GetWhat();
        return false;
    } catch (...)
    {
        m_errDescription = QObject::tr("Неизвестная ошибка при получении структуры данных");
        return false;
    }
}

QStringList HDF5FormatLib::ParseFile(QString& dataSetPath)
{
    return dataSetPath.split(QObject::tr("/"));
}

int HDF5FormatLib::GetByteType(int type)
{
    switch (type) {
    case 1: return H5T_NATIVE_CHAR;
    case 2: return H5T_NATIVE_SHORT;
    case 3: return H5T_NATIVE_INT;
    case 4: return H5T_NATIVE_FLOAT;
    case 5: return H5T_NATIVE_DOUBLE;
    case 9: return H5T_NATIVE_LDOUBLE; //2*double (?)
    case 12: return H5T_NATIVE_USHORT;
    default: throw GenericExc(m_errDescription = QObject::tr("Неизвестный формах данных"), -1);
    break;
    }
}
