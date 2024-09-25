#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "throws.h"

#include <QWidget>
#include <iostream>
#include <algorithm>
#include <QDebug>
using namespace std;

bool changedSort, changeTableType;
int displayOption, sortOption, throwTypeOption;
double minDistance, maxDistance;
QString locationFilter;
QDate startDate, endDate;
holder dataHolder;

void MainWindow::setTableRowsColsHeaders(int rows, int cols, QStringList headers, vector<QString> data){
    ui->dataTable->setRowCount(rows);
    ui->dataTable->setColumnCount(cols);
    ui->dataTable->setHorizontalHeaderLabels(headers);
    int count = 0;
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            QTableWidgetItem *item = new QTableWidgetItem(data.at(count));
            ui->dataTable->setItem(row, col, item);
            count++;
        }
    }
    return;
}

void MainWindow::buildTable(){
    QStringList headers, sortOptions, throwTypeOptions;
    vector<QString> data;
    location tempL;
    meet tempM;
    vector<throws> tempTV;
    vector<location> tempLV;
    vector<meet> tempMV;
    enableAdds();
    switch(displayOption){
        case 0:
            setDefaultValues();
            ui->FilterLabel->setText("Throw Type");
            headers << "Throw Id" << "Meet" << "Athlete Name" << "Distance (m)" << "School" << "Date" << "Event";
            sortOptions << "" << "Furthest - Shortest" << "Shortest - Furthest" << "Athlete Name (A-Z)" << "Athlete Name (Z-A)" << "Meet Name (A-Z)" << "Meet Name (Z-A)" << "Athletes School (A-Z)" << "Athletes School (Z-A)" << "Newest - Oldest" << "Oldest - Newest";
            throwTypeOptions << "All" << "Hammer" << "Weight";
            tempTV = sortThrowsVector(dataHolder,sortOption);
            tempTV = filterThrowsVector(tempTV,dataHolder, minDistance, maxDistance, startDate, endDate, throwTypeOption);
            for(throws ct : tempTV){
                tempL = dataHolder.findLocation(ct.getSchoolId());
                tempM = dataHolder.findMeet(ct.getMeetId());
                data.push_back(QString::number(ct.getThrowId()));
                data.push_back(QString::fromStdString(tempM.getMeetName()));
                data.push_back(QString::fromStdString(ct.getAthleteName()));
                if(ct.getDistance() == -1){
                    data.push_back("FOUL");
                }
                else{
                    data.push_back(QString::number(ct.getDistance(), 'f', 2));
                }
                data.push_back(QString::fromStdString(tempL.getLocationName()+" ("+tempL.getState()+")"));
                data.push_back(tempM.getDate().toString("MM/dd/yyyy"));
                if(ct.getThrowType() == throwType::Hammer){
                    data.push_back("Hammer");
                }
                else if(ct.getThrowType() == throwType::Weight){
                    data.push_back("Weight");
                }
            }
            setTableRowsColsHeaders(tempTV.size(),7,headers,data);
        break;
        case 1:
            if(changeTableType){
                locationFilter = "";
            }
            setDefaultValues();
            ui->verticalLayout_3->setEnabled(false);
            ui->label_3->setEnabled(false);
            ui->minDistance->setEnabled(false);
            ui->verticalLayout_7->setEnabled(false);
            ui->label_7->setEnabled(false);
            ui->maxDistance->setEnabled(false);
            ui->FilterLabel->setText("State or Location");
            headers << "Meet Id" << "Meet Name" << "Date" << "Location" << "Number of Athletes" << "Winning Athlete and Distance" << "Average Throw";
            sortOptions << "" << "Meet Name (A-Z)" << "Meet Name (Z-A)" << "Newest - Oldest" << "Oldest - Newest" << "Location (A-Z)" << "Location (Z-A)";
            throwTypeOptions << "";
            tempMV = sortMeetsVector(dataHolder,sortOption);
            tempMV = filterMeetsVector(tempMV, locationFilter, startDate, endDate);
            for(meet m : dataHolder.getMeetVector()){
                if(!throwTypeOptions.contains(QString::fromStdString(m.getState()))){
                    throwTypeOptions.push_back(QString::fromStdString(m.getState()));
                }
            }
            for(meet m : dataHolder.getMeetVector()){
                if(!throwTypeOptions.contains((QString::fromStdString(m.getLocationName()+" ("+m.getState()+")")))){
                    throwTypeOptions.push_back(QString::fromStdString(m.getLocationName()+" ("+m.getState()+")"));
                }
            }
            tempTV = dataHolder.getThrowsVector();
            for(meet cm : tempMV){
                data.push_back(QString::number(cm.getMeetId()));
                data.push_back(QString::fromStdString(cm.getMeetName()));
                data.push_back(cm.getDate().toString("MM/dd/yyyy"));
                data.push_back(QString::fromStdString(cm.getLocationName()+" ("+cm.getState()+")"));
                int numberOfThrows = 0;
                double totalDistance = 0;
                throws winningThrow = throws();
                QStringList throwers;
                throwers << "";
                for(int i = 0; i < tempTV.size(); i++){
                    if(tempTV[i].getMeetId()==cm.getMeetId()){
                        if(winningThrow.getDistance() == -10){
                            winningThrow = tempTV[i];
                        }
                        else if(winningThrow.getDistance()<tempTV[i].getDistance()){
                            winningThrow = tempTV[i];
                        }
                        if(tempTV[i].getDistance() != -1){
                            totalDistance += tempTV[i].getDistance();
                            numberOfThrows++;
                        }
                        if(!throwers.contains(QString::fromStdString(tempTV[i].getAthleteName()))){
                            throwers.push_back(QString::fromStdString(tempTV[i].getAthleteName()));
                        }
                    }
                }
                data.push_back(QString::number(throwers.size()-1));
                if(throwers.size() <= 1){
                    data.push_back("n/a");
                    data.push_back("n/a");
                }
                else{
                    data.push_back(QString::fromStdString(winningThrow.getAthleteName())+" ("+QString::number(winningThrow.getDistance())+"m)");
                    data.push_back(QString::number(totalDistance/numberOfThrows)+"m");
                }
            }
            setTableRowsColsHeaders(tempMV.size(),7,headers,data);
        break;
        case 2:
            ui->verticalLayout_3->setEnabled(false);
            ui->label_3->setEnabled(false);
            ui->minDistance->setEnabled(false);
            ui->verticalLayout_7->setEnabled(false);
            ui->label_7->setEnabled(false);
            ui->maxDistance->setEnabled(false);
            ui->verticalLayout_2->setEnabled(false);
            ui->FilterLabel->setEnabled(false);
            ui->throwTypeDD->setEnabled(false);
            ui->verticalLayout_5->setEnabled(false);
            ui->label_5->setEnabled(false);
            ui->startDate->setEnabled(false);
            ui->verticalLayout_6->setEnabled(false);
            ui->label_6->setEnabled(false);
            ui->endDate->setEnabled(false);
            headers << "Location Id" << "School" << "State" << "Number of Meets Held" << "Number of Athletes" << "Average Distance" << "Furthest Distance";
            sortOptions << "" << "Location Name (A-Z)" << "Location Name (Z-A)" << "State (A-Z)" << "State (Z-A)";
            tempLV = sortLocationVector(dataHolder, sortOption);
            for(location cl : tempLV){
                data.push_back(QString::number(cl.getLocationId()));
                data.push_back(QString::fromStdString(cl.getLocationName()));
                data.push_back(QString::fromStdString(cl.getState()));
                int numOfMeets = 0, numOfAthletes = 0, numberOfThrows = 0;
                double totalAverage = 0;

                for(meet m : dataHolder.getMeetVector()){
                    if(m.getLocationId() == cl.getLocationId()){
                        numOfMeets++;
                    }
                }
                tempTV = dataHolder.getThrowsVector();
                throws best = throws();
                QStringList throwers;
                throwers << "";
                for(int i = 0; i < tempTV.size(); i++){
                    if(tempTV[i].getSchoolId()==cl.getLocationId()){
                        if(best.getDistance() == -10){
                            best = tempTV[i];
                        }
                        else if(best.getDistance()<tempTV[i].getDistance()){
                            best = tempTV[i];
                        }
                        if(!throwers.contains(QString::fromStdString(tempTV[i].getAthleteName()))){
                            throwers.push_back(QString::fromStdString(tempTV[i].getAthleteName()));
                        }
                        if(tempTV[i].getDistance() != -1){
                            totalAverage += tempTV[i].getDistance();
                            numberOfThrows++;
                        }
                    }
                }

                data.push_back(QString::number(numOfMeets));
                if(throwers.size() <= 1){
                    data.push_back("n/a");
                    data.push_back("n/a");
                    data.push_back("n/a");
                }
                else{
                    data.push_back(QString::number(numOfAthletes));
                    data.push_back(QString::number(totalAverage/numberOfThrows)+"m");
                    data.push_back(QString::fromStdString(best.getAthleteName())+" ("+QString::number(best.getDistance())+"m)");
                }
            }
            setTableRowsColsHeaders(dataHolder.getLocationVector().size(),7,headers,data);
        break;
        case 3:
        ui->verticalLayout_3->setEnabled(false);
        ui->label_3->setEnabled(false);
        ui->minDistance->setEnabled(false);
        ui->verticalLayout_7->setEnabled(false);
        ui->label_7->setEnabled(false);
        ui->maxDistance->setEnabled(false);
        ui->verticalLayout_2->setEnabled(false);
        ui->FilterLabel->setEnabled(false);
        ui->throwTypeDD->setEnabled(false);
        ui->verticalLayout_5->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->startDate->setEnabled(false);
        ui->verticalLayout_6->setEnabled(false);
        ui->label_6->setEnabled(false);
        ui->endDate->setEnabled(false);
        ui->verticalLayout_4->setEnabled(false);
        ui->label_4->setEnabled(false);
        ui->sortByDD->setEnabled(false);
        headers << "Athlete Name" << "School" << "Total # of Throws" << "# of Non-Fouled Throws" << "# of Fouled Throws" << "Average Distance (Non-Fouled)" << "Furthest Distance" << "Shortest Distance (Non-Fouled)" << "Foul %";
            tempTV = dataHolder.getThrowsVector();
            QStringList throwersList;
            for(int i = 0; i < tempTV.size(); i++){
                QString name = QString::fromStdString(tempTV[i].getAthleteName());
                if(!throwersList.contains(name)){
                    int numberOfThrows = 0, numberOfFouls = 0, numberOfNonFouls = 0;
                    double averageDistance = 0, bestDistance = -10, worstNonFoul = -10, foulPercentage=0.0;
                    throwersList.push_back(name);
                    data.push_back(name);
                    data.push_back(QString::fromStdString(dataHolder.findLocation(tempTV[i].getSchoolId()).getLocationName()));
                    for(int j = i; j < tempTV.size(); j++){
                        if(QString::fromStdString(tempTV[j].getAthleteName())==name){
                            numberOfThrows++;
                            if(tempTV[j].getDistance() != -1){
                                numberOfNonFouls++;
                                averageDistance += tempTV[j].getDistance();
                                if(bestDistance == -10){
                                    bestDistance = tempTV[j].getDistance();
                                    worstNonFoul = tempTV[j].getDistance();
                                }
                                else{
                                    if(tempTV[j].getDistance()>bestDistance){
                                        bestDistance = tempTV[j].getDistance();
                                    }
                                    if(tempTV[j].getDistance()<worstNonFoul){
                                        worstNonFoul = tempTV[j].getDistance();
                                    }
                                }
                            }
                            else{
                                numberOfFouls++;
                            }
                        }
                    }
                    if(numberOfNonFouls == 0){
                        averageDistance = averageDistance/numberOfThrows;
                    }
                    else{
                        averageDistance = averageDistance/numberOfNonFouls;
                    }
                    foulPercentage = (numberOfFouls/(static_cast<double>(numberOfThrows))*static_cast<double>(100));

                    data.push_back(QString::number(numberOfThrows));
                    data.push_back(QString::number(numberOfNonFouls));
                    data.push_back(QString::number(numberOfFouls));
                    if(bestDistance == -10){
                        data.push_back("n/a");
                        data.push_back("n/a");
                        data.push_back("n/a");
                    }
                    else{
                        data.push_back(QString::number(averageDistance));
                        data.push_back(QString::number(bestDistance));
                        data.push_back(QString::number(worstNonFoul));
                    }

                    data.push_back(QString::number(foulPercentage) + "%");
                }
            }
            setTableRowsColsHeaders(throwersList.size(),9,headers,data);
        break;
    }
    if(changeTableType){
        ui->sortByDD->clear();
        ui->sortByDD->addItems(sortOptions);
        ui->throwTypeDD->clear();
        ui->throwTypeDD->addItems(throwTypeOptions);
        changeTableType = false;
        ui->sortByDD->currentIndexChanged(0);
        ui->throwTypeDD->currentIndexChanged(0);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataHolder.readCSV();
    endDate = QDate::currentDate();
    startDate.setDate(1901,1,1);
    maxDistance = 100;
    minDistance = -1;
    displayOption=0;
    sortOption=0;
    ui->minDistance->setValue(0);
    ui->maxDistance->setValue(100);
    ui->minDistance->setMaximum(maxDistance);
    ui->maxDistance->setMaximum(100);
    ui->minDistance->setMinimum(0);
    ui->maxDistance->setMinimum(0);
    setDefaultValues();
    changedSort = false;
    changeTableType = false;
    QStringList sortOptions,throwTypeOptions;
    sortOptions << "" << "Furthest - Shortest" << "Shortest - Furthest" << "Athlete Name (A-Z)" << "Athlete Name (Z-A)" << "Meet Name (A-Z)" << "Meet Name (Z-A)" << "Athletes School (A-Z)" << "Athletes School (Z-A)" << "Newest - Oldest" << "Oldest - Newest";
    throwTypeOptions << "All" << "Hammer" << "Weight";

    ui->sortByDD->addItems(sortOptions);
    ui->throwTypeDD->addItems(throwTypeOptions);

    buildTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getInfoButton_clicked()
{
    if(changeTableType){
        sortOption = 0;
        ui->sortByDD->setCurrentIndex(0);
    }
    buildTable();
}

void MainWindow::on_sortByDD_currentIndexChanged(int index)
{
    sortOption = index;
    changedSort = true;
}


void MainWindow::on_throwTypeDD_currentIndexChanged(int index)
{
    throwTypeOption = index;
    changedSort = true;
}


void MainWindow::on_startDate_userDateChanged(const QDate &date)
{
    startDate = date;
    ui->endDate->setMinimumDate(startDate);
}


void MainWindow::on_endDate_userDateChanged(const QDate &date)
{
    endDate = date;
    ui->startDate->setMaximumDate(endDate);
}

void MainWindow::on_displayChoice_currentIndexChanged(int index)
{
    displayOption = index;
    changeTableType = true;
    changedSort = false;
}

void MainWindow::on_dataTable_itemDoubleClicked(QTableWidgetItem *item){

}


void MainWindow::on_minDistance_valueChanged(double arg1)
{
    if(arg1 == 0){
        minDistance = -1;
    }
    else{
        minDistance = arg1;
    }
    ui->maxDistance->setMinimum(arg1);
}


void MainWindow::on_maxDistance_valueChanged(double arg1)
{
    if(arg1 == 0){
        maxDistance = -1;
    }
    else{
        maxDistance = arg1;
    }
    ui->minDistance->setMaximum(arg1);
}


void MainWindow::on_throwTypeDD_currentTextChanged(const QString &arg1)
{
    locationFilter = arg1;
    changedSort = true;
}

void MainWindow::setDefaultValues(){
    ui->verticalLayout_3->setEnabled(true);
    ui->label_3->setEnabled(true);
    ui->minDistance->setEnabled(true);
    ui->verticalLayout_7->setEnabled(true);
    ui->label_7->setEnabled(true);
    ui->maxDistance->setEnabled(true);
    ui->verticalLayout_2->setEnabled(true);
    ui->FilterLabel->setEnabled(true);
    ui->throwTypeDD->setEnabled(true);
    ui->verticalLayout_5->setEnabled(true);
    ui->label_5->setEnabled(true);
    ui->startDate->setEnabled(true);
    ui->verticalLayout_6->setEnabled(true);
    ui->label_6->setEnabled(true);
    ui->endDate->setEnabled(true);
    ui->verticalLayout_4->setEnabled(true);
    ui->label_4->setEnabled(true);
    ui->sortByDD->setEnabled(true);

    //set starting date values
    ui->endDate->setDate(endDate);
    ui->startDate->setDate(startDate);
    //set max date values
    ui->endDate->setMaximumDate(endDate);
    ui->startDate->setMaximumDate(endDate);
    //set minimum date value for date values
    ui->endDate->setMinimumDate(startDate);
    ui->startDate->setMinimumDate(startDate);
}

void MainWindow::enableAdds(){
    ui->addInput1->setEnabled(false);
    ui->addLabel1->setEnabled(false);
    ui->addInput2->setEnabled(false);
    ui->addLabel2->setEnabled(false);
    ui->addInput3->setEnabled(false);
    ui->addLabel3->setEnabled(false);
    ui->addInput4->setEnabled(false);
    ui->addLabel4->setEnabled(false);
    ui->addInput5->setEnabled(false);
    ui->addLabel5->setEnabled(false);
    ui->addInput6->setEnabled(false);
    ui->addLabel6->setEnabled(false);
    ui->addInput7->setEnabled(false);
    ui->addLabel7->setEnabled(false);

    ui->addInfo->setEnabled(true);

    ui->addLabel1->setText("");
    ui->addLabel2->setText("");
    ui->addLabel3->setText("");
    ui->addLabel4->setText("");
    ui->addLabel5->setText("");
    ui->addLabel6->setText("");
    ui->addLabel7->setText("");

    ui->addInput1->setText("");
    ui->addInput2->setText("");
    ui->addInput3->setCurrentIndex(0);
    ui->addInput4->setCurrentIndex(0);
    ui->addInput5->setCurrentIndex(0);
    ui->addInput6->setDate(QDate::currentDate());
    ui->addInput7->setMinimum(-1);
    ui->addInput7->setValue(40);

    QStringList throwTypes, meetList, locationList;
    throwTypes << "";
    meetList << "";
    locationList << "";

    switch (displayOption) {
    case 0:
        ui->addLabel->setText("Add New Throw Instance");

        ui->addInput1->setEnabled(true);
        ui->addLabel1->setEnabled(true);
        ui->addInput3->setEnabled(true);
        ui->addLabel3->setEnabled(true);
        ui->addInput4->setEnabled(true);
        ui->addLabel4->setEnabled(true);
        ui->addInput5->setEnabled(true);
        ui->addLabel5->setEnabled(true);
        ui->addInput7->setEnabled(true);
        ui->addLabel7->setEnabled(true);

        ui->addLabel1->setText("Athlete's Name");
        ui->addLabel3->setText("Throw Type");
        ui->addLabel4->setText("Meet");
        ui->addLabel5->setText("Athlete's School");
        ui->addLabel7->setText("Distance (less than 0 for foul)");

        for(int i = static_cast<int>(throwType::Hammer); i <= static_cast<int>(throwType::Weight); i++){
            throwTypes.push_back(QString::fromStdString(throwTypeToString(static_cast<throwType>(i))));
        }

        for(meet m:dataHolder.getMeetVector()){
            meetList.push_back(QString::fromStdString(m.getMeetName())+" ("+m.getDate().toString()+")");
        }

        for(location l:dataHolder.getLocationVector()){
            locationList.push_back(QString::fromStdString(l.getLocationName()+" ("+l.getState()+")"));
        }

        ui->addInput3->clear();
        ui->addInput4->clear();
        ui->addInput5->clear();
        ui->addInput3->addItems(throwTypes);
        ui->addInput4->addItems(meetList);
        ui->addInput5->addItems(locationList);

    break;
    case 1:
        ui->addLabel->setText("Add New Meet");

        ui->addInput1->setEnabled(true);
        ui->addLabel1->setEnabled(true);
        ui->addInput3->setEnabled(true);
        ui->addLabel3->setEnabled(true);
        ui->addInput6->setEnabled(true);
        ui->addLabel6->setEnabled(true);

        ui->addLabel1->setText("Meet Name");
        ui->addLabel3->setText("Location of Meet");
        ui->addLabel6->setText("Date of Meet");

        for(location l:dataHolder.getLocationVector()){
            locationList.push_back(QString::fromStdString(l.getLocationName()+" ("+l.getState()+")"));
        }

        ui->addInput3->clear();
        ui->addInput3->addItems(locationList);

    break;
    case 2:
        ui->addLabel->setText("Add New Location");

        ui->addInput1->setEnabled(true);
        ui->addLabel1->setEnabled(true);
        ui->addInput2->setEnabled(true);
        ui->addLabel2->setEnabled(true);

        ui->addLabel1->setText("Location Name");
        ui->addLabel2->setText("State");


    break;
    case 3:
        ui->addInfo->setEnabled(false);
    break;
    default:
    break;
    }
}


void MainWindow::on_addInfo_clicked()
{

    vector<throws> tempThrows = dataHolder.getThrowsVector();
    vector<meet> tempMeets = dataHolder.getMeetVector();
    vector<location> tempLocations = dataHolder.getLocationVector();
    string text1, text2;
    int newId, select1, select2, select3;
    QDate date;
    double distance;
    enum throwType throwType;
    location tempLocation;
    bool changesMade = false;

    switch(displayOption){
    case 0:
        //throws(string athleteName, int throwId, int meetId, int schoolId, enum throwType throwType, double distance)
        text1 = ui->addInput1->text().toStdString();
        newId = tempThrows.size();
        select1 = ui->addInput3->currentIndex()-1;
        select2 = ui->addInput4->currentIndex()-1;
        select3 = ui->addInput5->currentIndex()-1;
        throwType = static_cast<enum::throwType>(select1);
        distance = ui->addInput7->value();
        if(distance < 0){
            distance = -1;
        }
        if(text1 == "" || select1 == -1 || select2 == -1 || select3 == -1){
            ui->addLabel->setText("Add New Throw Instance... Incorrect Values");
        }
        else{
            tempThrows.push_back(throws(text1,newId,select2,select3,throwType,distance));
            dataHolder.setThrowsVector(tempThrows);
            changesMade = true;
        }

    break;
    case 1:
        //meet(string locationName, string state, int locationId, string meetName, QDate date, int meetId):location(locationName, state, locationId)
        text1 = ui->addInput1->text().toStdString();
        select1 = ui->addInput3->currentIndex()-1;
        date = ui->addInput6->date();
        newId = tempMeets.size();
        tempLocation = dataHolder.findLocation(select1);
        qDebug() << text1;
        qDebug() << select1;
        if(text1 == "" || select1 == -1){
            qDebug() << text1;
            qDebug() << select1;
            ui->addLabel->setText("Add New Meet... Incorrect Values");
        }
        else{
            tempMeets.push_back(meet(tempLocation.getLocationName(),tempLocation.getState(),tempLocation.getLocationId(),text1,date,newId));
            dataHolder.setMeetVector(tempMeets);
            changesMade = true;
        }
    break;
    case 2:
        //location(string locationName, string state, int locationId)
        text1 = ui->addInput1->text().toStdString();
        text2 = ui->addInput2->text().toStdString();
        newId = tempLocations.size();
        if(text1 == "" || text2 == ""){
           ui->addLabel->setText("Add New Location... Incorrect Values");
        }
        else{
           tempLocations.push_back(location(text1,text2,newId));
           dataHolder.setLocationVector(tempLocations);
           changesMade = true;
        }
    break;
    }
    if(changesMade){
        buildTable();
    }
}

