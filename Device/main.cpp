#include "mainwindow.h"
#include <QDebug>
#include <QtDebug>
#include <QApplication>
#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <string>
#include <QtCore/QCoreApplication>
#include <Windows.h>
#include <SetupAPI.h>
int alper();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "Alper: " << Qt::endl;
    //int annn = alper();
    return a.exec();

}
int alper()
{
    //    GUID guid = {
    //        0x00000000, // initialize the Data1 member to 0
    //        0x0000, // initialize the Data2 member to 0
    //        0x0000, // initialize the Data3 member to 0
    //        { 0, 0, 0, 0, 0, 0, 0, 0 } // initialize the Data4 member to an array of 8 bytes, all set to 0
    //    };

    HDEVINFO device_info_set = SetupDiGetClassDevsA(NULL, "USB", NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if (device_info_set == INVALID_HANDLE_VALUE)
    {
        qDebug() << "Failed to get device information set. Error code: " << GetLastError() << Qt::endl;
        return 1;
    }

    SP_DEVINFO_DATA device_info_data = { 0 };


    //    SP_DEVINFO_DATA device_info_data = {
    //        sizeof(SP_DEVINFO_DATA), // initialize the cbSize member
    //        { 0 }, // initialize the ClassGuid member to an empty GUID
    //        0, // initialize the DevInst member to 0
    //        0 // initialize the Reserved member to 0
    //    };

    device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(device_info_set, i, &device_info_data); i++)
    {

        DWORD property_type = 0;
        CHAR description[MAX_PATH] = { 0 };
        DWORD required_size = 0;
        if (SetupDiGetDeviceRegistryPropertyA(device_info_set, &device_info_data, SPDRP_DEVICEDESC, &property_type, (BYTE*)description, MAX_PATH, &required_size))
        {
            std::string description_str(description);
            //qDebug() << "Description: " << description_str << Qt::endl;
            qDebug() << "Description: " << QString::fromStdString(description_str)/* << Qt::endl*/;

        }

        CHAR device_id[255] = { 0 };
        if (SetupDiGetDeviceInstanceIdA(device_info_set, &device_info_data, device_id, 255, NULL))
        {
            //qDebug() << "Device ID: " << device_id << Qt::endl;

            std::string device_id_str(device_id);
            std::size_t found = device_id_str.find_last_of("\\");
            if (found != std::string::npos)
            {
                std::string substring = device_id_str.substr(found + 1);
                //qDebug() << "Device ID: " << substring << Qt::endl << Qt::endl;
                qDebug() << "Device ID: " << QString::fromStdString(substring) << Qt::endl;

            }

        }

        /*DWORD device_type = 0;
        if (SetupDiGetDeviceRegistryPropertyA(device_info_set, &device_info_data, SPDRP_DEVTYPE, &property_type, (BYTE*)&device_type, sizeof(device_type), &required_size))
        {
            qDebug() << "Device Type: " << device_type << Qt::endl << Qt::endl;
        }*/
    }

    SetupDiDestroyDeviceInfoList(device_info_set);

    return 0;
}
