#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QtDebug>
#include <QApplication>
#include <iostream>
#include <windows.h>
#include <setupapi.h>
#include <string>
#include <QtCore/QCoreApplication>
#include <QClipboard>
#include <guiddef.h>
#include <QString>
#include <QTableWidgetItem>


QString deneme(int asdqwe);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    //ui->label->setText("Alper"); // textini değiştir
    //QMessageBox::about(this, "My Title", "Mesajin ta kendisi!!!"); // ekrana popup message olayı
    // about, critical-kırmızı, information-mavi i, question- mavi?, warning-sari!
    QMessageBox::StandardButton reply = QMessageBox::question(this,"My Title", "Mesajin ta kendisi!!!", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QApplication::quit();
    }
    else{
        qDebug() << "No tiklandi";
    }
}


void MainWindow::on_pushButton_7_clicked()
{
    //    SecDialog secDeialog;
    //    secDeialog.setModal(true);
    //    secDeialog.exec();
    hide();
    secDialog = new SecDialog(this); //2.ye geçme kodu
    secDialog->show();

}


void MainWindow::on_pushButton_alper_clicked()
{
    //    QString alperrrr = "alperxd";
    //    ui->listWidget->addItem(alperrrr);

    //ui->label->setText("gösterildi");
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    QStringList headers;
    headers << "Description" << "Serial Numbers" << "InstanceID" << "Friendly Name"; // Sütun isimlerini ekle
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;

    int sayac1 = 0;
    int sayac2 = 0;

    GUID class_guids[8] = {
        { 0x4d36e965, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // cd-rom
        { 0x4d36e979, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, //printer
        { 0xca3e7ab9, 0xb4c3, 0x4ae6, { 0x82, 0x51, 0x57, 0x9e, 0xf9, 0x33, 0x89, 0x0f } }, // camera
        { 0xe0cbf06c, 0xcd8b, 0x4647, { 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74 } }, // bluetooth
        { 0x50dd5230, 0xba8a, 0x11d1, { 0xbf, 0x5d, 0x00, 0x00, 0xf8, 0x05, 0xf5, 0x30 } }, // card reader
        { 0x4d36e972, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // network adapter
        { 0x4d36e96d, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // modem
        { 0x4d36e967, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } } // disk
    };


    for (int j = 0; j < 8; j++) {
        GUID class_guid = class_guids[j];

        HDEVINFO device_info_set = SetupDiGetClassDevsA(&class_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
        if (device_info_set == INVALID_HANDLE_VALUE)
        {
            qDebug() << "Failed to get device information set. Error code: " << GetLastError() << Qt::endl;
            //return 1;
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
            qDebug() << sayac1;
            sayac1++;
            DWORD property_type = 0;
            CHAR description[MAX_PATH] = { 0 };
            DWORD required_size = 0;
            if (SetupDiGetDeviceRegistryPropertyA(device_info_set, &device_info_data, SPDRP_DEVICEDESC, &property_type, (BYTE*)description, MAX_PATH, &required_size))
            {
                std::string description_str(description);
                //qDebug() << "Description: " << description_str << Qt::endl;
                qDebug() << "Description: " << QString::fromStdString(description_str)/* << Qt::endl*/;
                //ui->listWidget->addItem(QString::fromStdString(description_str));
                //ui->listWidget->addItem("");
                ui->tableWidget->insertRow(row);
                //ui->secilen_gruplar_tableWidget->setItem(row, 0, new QTableWidgetItem(template_name)); // Şablon isimlerini ekleyin
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(description_str)));

            }

            DWORD property_type1 = 0;
            DWORD required_size1 = 0;

            CHAR description1[MAX_PATH] = { 0 };

            if (SetupDiGetDeviceRegistryPropertyA(device_info_set, &device_info_data, SPDRP_FRIENDLYNAME, &property_type1, (BYTE*)description1, MAX_PATH, &required_size1))
            {
                std::string description_str1(description1);
                //std::cout << "Description1: " << description_str1 << std::endl << std::endl;
                qDebug() << "Friendly Name: " << QString::fromStdString(description_str1)/* << Qt::endl*/;
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(description_str1)));

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
                    //qDebug() << "Device ID: " << QString::fromStdString(substring) << Qt::endl;
                    //ui->listWidget_3->addItem(QString::fromStdString(substring));
                    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(substring)));
                    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(device_id)));


                    //ui->listWidget->addItem(QString::fromStdString(substring));
                }
                //ui->listWidget->addItem("");
                //ui->listWidget->addItem("");

            }

            /*DWORD device_type = 0;
        if (SetupDiGetDeviceRegistryPropertyA(device_info_set, &device_info_data, SPDRP_DEVTYPE, &property_type, (BYTE*)&device_type, sizeof(device_type), &required_size))
        {
            qDebug() << "Device Type: " << device_type << Qt::endl << Qt::endl;
        }*/

            //ui->secilen_gruplar_tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("time").toString()));
            row++;


        }

        SetupDiDestroyDeviceInfoList(device_info_set);
    }
    //return 0;
}


void MainWindow::on_pushButton_8_clicked()
{
    //QListWidget *listWidget = ui->listWidget;
    //QListWidgetItem *item = listWidget->currentItem();

    //int i_nin_degeri = listWidget->row(item);
    //qDebug() << "Sayısı: " << i_nin_degeri;
    QString listteyazdir = "";
    //listteyazdir = deneme(i_nin_degeri);

    qDebug() << listteyazdir;

    //ui->listWidget_2->addItem(listteyazdir);

    //    QString adana = item->text();

    //    int row = listWidget->row(item);
    //    ui->listWidget_2->addItem(adana);
    //    QListWidget *listWidget_2 = ui->listWidget_2;
    //    listWidget_2->addItem(row);
    //    listWidget->takeItem(row);
    //    delete item;

}

QString deneme(int asdqwe)
{
    QString idididid = "";
    int alper = asdqwe;
    qDebug() << "koda girdi helal: " << alper;

    HDEVINFO device_info_set1 = SetupDiGetClassDevsA(NULL, "USB", NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if (device_info_set1 == INVALID_HANDLE_VALUE)
    {
        qDebug() << "Failed to get device information set. Error code: " << GetLastError() << Qt::endl;
        //return 1;
    }

    SP_DEVINFO_DATA device_info_data1 = { 0 };


    //    SP_DEVINFO_DATA device_info_data = {
    //        sizeof(SP_DEVINFO_DATA), // initialize the cbSize member
    //        { 0 }, // initialize the ClassGuid member to an empty GUID
    //        0, // initialize the DevInst member to 0
    //        0 // initialize the Reserved member to 0
    //    };

    device_info_data1.cbSize = sizeof(SP_DEVINFO_DATA);

    if (SetupDiEnumDeviceInfo(device_info_set1, alper, &device_info_data1))
    {

        DWORD property_type = 0;
        CHAR description[MAX_PATH] = { 0 };
        DWORD required_size = 0;
        if (SetupDiGetDeviceRegistryPropertyA(device_info_set1, &device_info_data1, SPDRP_DEVICEDESC, &property_type, (BYTE*)description, MAX_PATH, &required_size))
        {
            std::string description_str(description);
            //qDebug() << "Description: " << description_str << Qt::endl;
            qDebug() << "Description: " << QString::fromStdString(description_str)/* << Qt::endl*/;
            //ui->listWidget->addItem(QString::fromStdString(description_str));
            //ui->listWidget->addItem("");
        }

        CHAR device_id[255] = { 0 };
        if (SetupDiGetDeviceInstanceIdA(device_info_set1, &device_info_data1, device_id, 255, NULL))
        {
            //qDebug() << "Device ID: " << device_id << Qt::endl;

            std::string device_id_str(device_id);
            std::size_t found = device_id_str.find_last_of("\\");
            if (found != std::string::npos)
            {
                std::string substring = device_id_str.substr(found + 1);
                //qDebug() << "Device ID: " << substring << Qt::endl << Qt::endl;
                qDebug() << "Device ID: " << QString::fromStdString(substring) << Qt::endl;
                //ui->listWidget->addItem(QString::fromStdString(substring));
                idididid = QString::fromStdString(substring);
            }
            //ui->listWidget->addItem("");
            //ui->listWidget->addItem("");
        }
        /*DWORD device_type = 0;
        if (SetupDiGetDeviceRegistryPropertyA(device_info_set, &device_info_data, SPDRP_DEVTYPE, &property_type, (BYTE*)&device_type, sizeof(device_type), &required_size))
        {
            qDebug() << "Device Type: " << device_type << Qt::endl << Qt::endl;
        }*/
    }

    SetupDiDestroyDeviceInfoList(device_info_set1);

    return idididid;

}

void MainWindow::on_pushButton_9_clicked()
{
    //    QClipboard *clipboard = QGuiApplication::clipboard();
    //QListWidget *listWidget3 = ui->listWidget_3;
    QTableWidget *tableWidget = ui->tableWidget;
    //    QListWidgetItem *item2 = listWidget2->currentItem();

    //    QString originalText = item2->;
    //    // etc.
    //    clipboard->setText(newText);

    // Get the current item of the list widget
    //QListWidgetItem *currentItem = listWidget3->currentItem();
    QTableWidgetItem *currentItem = tableWidget->currentItem();


    // Check if a current item is selected
    if (currentItem) {
        // Get the text of the current item
        QString itemText = currentItem->text();

        // Copy the text to the clipboard
        QApplication::clipboard()->setText(itemText);
    }


}


void MainWindow::on_pushButton_11_clicked()
{

}


void MainWindow::on_pushButtonx_clicked()
{
    GUID class_guids[8] = {
        { 0x4d36e965, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // cd-rom
        { 0x4d36e979, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, //printer
        { 0xca3e7ab9, 0xb4c3, 0x4ae6, { 0x82, 0x51, 0x57, 0x9e, 0xf9, 0x33, 0x89, 0x0f } }, // camera
        { 0xe0cbf06c, 0xcd8b, 0x4647, { 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74 } }, // bluetooth
        { 0x50dd5230, 0xba8a, 0x11d1, { 0xbf, 0x5d, 0x00, 0x00, 0xf8, 0x05, 0xf5, 0x30 } }, // card reader
        { 0x4d36e972, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // network adapter
        { 0x4d36e96d, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // modem
        { 0x4d36e967, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } } // disk
    };

    for (int j = 0; j < 8; j++) {
        GUID class_guid = class_guids[j];

        HDEVINFO device_info_set = SetupDiGetClassDevsA(&class_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
        if (device_info_set == INVALID_HANDLE_VALUE)
        {
            qDebug() << "Failed to get device information set. Error code: " << GetLastError() << Qt::endl;
            //return 1;
        }

        SP_DEVINFO_DATA device_info_data = { 0 };

        device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

        for (DWORD i = 0; SetupDiEnumDeviceInfo(device_info_set, i, &device_info_data); i++)
        {

            CHAR device_id[255] = { 0 };
            if (SetupDiGetDeviceInstanceIdA(device_info_set, &device_info_data, device_id, 255, NULL))
            {
                QTableWidget *tableWidget = ui->tableWidget;
                QTableWidgetItem *currentItem = tableWidget->currentItem();
                if (currentItem) {
                    // Get the text of the current item
                    QString itemText = currentItem->text();
                    if (itemText == device_id)
                    {
                        //qDebug() << "AlperAlperAlper " << device_id << "AlperAlperAlper " << itemText << Qt::endl;
                        SP_DEVINFO_DATA devInfoData = { sizeof(SP_DEVINFO_DATA) };
                        BOOL bResult = SetupDiOpenDeviceInfoA(device_info_set, device_id, NULL, 0, &devInfoData);
                        if (!bResult)
                        {
                            std::cerr << "Error: SetupDiOpenDeviceInfo failed. GetLastError() returned " << GetLastError() << std::endl;
                            SetupDiDestroyDeviceInfoList(device_info_set);

                        }

                        // Set the device state to disable
                        SP_PROPCHANGE_PARAMS propChangeParams = { sizeof(SP_PROPCHANGE_PARAMS) };
                        propChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
                        propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
                        propChangeParams.StateChange = DICS_DISABLE;
                        propChangeParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
                        propChangeParams.HwProfile = 0;
                        bResult = SetupDiSetClassInstallParams(device_info_set, &devInfoData, &propChangeParams.ClassInstallHeader, sizeof(SP_PROPCHANGE_PARAMS));
                        if (!bResult)
                        {
                            std::cerr << "Error: SetupDiSetClassInstallParams failed. GetLastError() returned " << GetLastError() << std::endl;
                            SetupDiDestroyDeviceInfoList(device_info_set);

                        }
                        // Call the class installer function to disable the device
                        bResult = SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, device_info_set, &devInfoData);
                        if (!bResult)
                        {
                            std::cerr << "Error: SetupDiCallClassInstaller failed. GetLastError() returned " << GetLastError() << std::endl;
                            SetupDiDestroyDeviceInfoList(device_info_set);

                        }
                    }
                }

                //qDebug() << "Device ID: " << device_id << Qt::endl;
            }
        }

        SetupDiDestroyDeviceInfoList(device_info_set);
    }
}


void MainWindow::on_pushButton_Enable_clicked()
{
    GUID class_guids[8] = {
        { 0x4d36e965, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // cd-rom
        { 0x4d36e979, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, //printer
        { 0xca3e7ab9, 0xb4c3, 0x4ae6, { 0x82, 0x51, 0x57, 0x9e, 0xf9, 0x33, 0x89, 0x0f } }, // camera
        { 0xe0cbf06c, 0xcd8b, 0x4647, { 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74 } }, // bluetooth
        { 0x50dd5230, 0xba8a, 0x11d1, { 0xbf, 0x5d, 0x00, 0x00, 0xf8, 0x05, 0xf5, 0x30 } }, // card reader
        { 0x4d36e972, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // network adapter
        { 0x4d36e96d, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } }, // modem
        { 0x4d36e967, 0xe325, 0x11ce, { 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 } } // disk
    };

    for (int j = 0; j < 8; j++) {
        GUID class_guid = class_guids[j];

        HDEVINFO device_info_set = SetupDiGetClassDevsA(&class_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
        if (device_info_set == INVALID_HANDLE_VALUE)
        {
            qDebug() << "Failed to get device information set. Error code: " << GetLastError() << Qt::endl;
            //return 1;
        }

        SP_DEVINFO_DATA device_info_data = { 0 };

        device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

        for (DWORD i = 0; SetupDiEnumDeviceInfo(device_info_set, i, &device_info_data); i++)
        {

            CHAR device_id[255] = { 0 };
            if (SetupDiGetDeviceInstanceIdA(device_info_set, &device_info_data, device_id, 255, NULL))
            {
                QTableWidget *tableWidget = ui->tableWidget;
                QTableWidgetItem *currentItem = tableWidget->currentItem();
                if (currentItem) {
                    // Get the text of the current item
                    QString itemText = currentItem->text();
                    if (itemText == device_id)
                    {
                        //qDebug() << "AlperAlperAlper " << device_id << "AlperAlperAlper " << itemText << Qt::endl;
                        SP_DEVINFO_DATA devInfoData = { sizeof(SP_DEVINFO_DATA) };
                        BOOL bResult = SetupDiOpenDeviceInfoA(device_info_set, device_id, NULL, 0, &devInfoData);
                        if (!bResult)
                        {
                            std::cerr << "Error: SetupDiOpenDeviceInfo failed. GetLastError() returned " << GetLastError() << std::endl;
                            SetupDiDestroyDeviceInfoList(device_info_set);

                        }

                        // Set the device state to disable
                        SP_PROPCHANGE_PARAMS propChangeParams = { sizeof(SP_PROPCHANGE_PARAMS) };
                        propChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
                        propChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
                        propChangeParams.StateChange = DICS_ENABLE;
                        propChangeParams.Scope = DICS_FLAG_CONFIGSPECIFIC;
                        propChangeParams.HwProfile = 0;
                        bResult = SetupDiSetClassInstallParams(device_info_set, &devInfoData, &propChangeParams.ClassInstallHeader, sizeof(SP_PROPCHANGE_PARAMS));
                        if (!bResult)
                        {
                            std::cerr << "Error: SetupDiSetClassInstallParams failed. GetLastError() returned " << GetLastError() << std::endl;
                            SetupDiDestroyDeviceInfoList(device_info_set);

                        }
                        // Call the class installer function to disable the device
                        bResult = SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, device_info_set, &devInfoData);
                        if (!bResult)
                        {
                            std::cerr << "Error: SetupDiCallClassInstaller failed. GetLastError() returned " << GetLastError() << std::endl;
                            SetupDiDestroyDeviceInfoList(device_info_set);

                        }
                    }
                }

                //qDebug() << "Device ID: " << device_id << Qt::endl;
            }
        }

        SetupDiDestroyDeviceInfoList(device_info_set);
    }
}


void MainWindow::on_pushButtonDiskGoster_clicked()
{
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(2);
    QStringList headers;
    headers << "Drive Letter" << "Volume Name"; // Sütun isimlerini ekle
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);
    int row = 0;

    char buffer[256];
    DWORD drives = GetLogicalDrives();

    for (int i = 0; i < 26; i++)
    {
        if ((drives & (1 << i)) != 0) {
            char driveLetter = 'A' + i;
            snprintf(buffer, sizeof(buffer), "%c:\\", driveLetter);
            UINT driveType = GetDriveTypeA(buffer);

            if (driveType == DRIVE_REMOVABLE) {
                char volumeName[MAX_PATH + 1];
                char fileSystemName[MAX_PATH + 1];
                DWORD serialNumber;

                if (GetVolumeInformationA(buffer, volumeName, sizeof(volumeName), &serialNumber, NULL, NULL, fileSystemName, sizeof(fileSystemName))) {
                    std::cout << "Drive Letter: " << driveLetter << std::endl;
                    std::cout << "Volume Name: " << volumeName << std::endl;
                    //std::cout << "File System: " << fileSystemName << std::endl;
                    //::cout << "Serial Number: " << serialNumber << std::endl;
                    std::cout << std::endl;
                    ui->tableWidget_2->insertRow(row);
                    QString driveLetterString(driveLetter);

                    ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(driveLetterString));

                    //ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(driveLetter));
                    ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(volumeName));

                    row ++;

                }
            }



        }


    }
}


void MainWindow::on_pushButtonEject_clicked()
{
    QTableWidget *tableWidget_2 = ui->tableWidget_2;
    QTableWidgetItem *currentItem = tableWidget_2->currentItem();
    if (currentItem) {
        // Get the text of the current item
        QString itemText = currentItem->text();

        QByteArray byteArray = itemText.toLatin1();
        char driveLetter = byteArray.at(0);



        std::string path = R"(\\.\)";
        path.push_back(driveLetter);
        path += ":";

        const char* drivePath = path.c_str();

        std::cout << "Drive path: " << drivePath << std::endl;

        // Open the device
        HANDLE hDevice = CreateFileA(drivePath,
                                     GENERIC_READ | GENERIC_WRITE,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                                     NULL,
                                     OPEN_EXISTING,
                                     0,
                                     NULL);

        // Issue the eject control code
        DWORD bytesReturned;
        BOOL result = DeviceIoControl(hDevice,
                                      IOCTL_STORAGE_EJECT_MEDIA,
                                      NULL,
                                      0,
                                      NULL,
                                      0,
                                      &bytesReturned,
                                      NULL);

        if (result)
        {
            std::cout << "USB drive ejected successfully." << std::endl;
        }
        else
        {
            std::cout << "Failed to eject the USB drive." << std::endl;
        }

        // Close the device handle
        CloseHandle(hDevice);
    }
}

