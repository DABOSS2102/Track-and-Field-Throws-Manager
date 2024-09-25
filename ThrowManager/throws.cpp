#include "throws.h"
#include <chrono>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>

using namespace std;

//location functions
location::location(){
    locationName = "";
    state = "";
    locationId = -1;
}
location::location(string locationName, string state, int locationId){
    this -> locationName = locationName;
    this -> state = state;
    this -> locationId = locationId;
}
int location::getLocationId(){
    return this -> locationId;
}
string location::getState(){
    return this -> state;
}
string location::getLocationName(){
    return this -> locationName;
}
string location::getDataForCSVLocation(){
    string temp = "2,";
    temp += to_string(getLocationId()) + ",";
    temp += getLocationName() + ",";
    temp += getState();
    return temp;
}

//meet functions
meet::meet():location(){
    meetName = "";
    date = QDate::fromString("1/1/1900", "MM,dd/yyyy"); //1/1/1900
    meetId = -1;
}
meet::meet(string locationName, string state, int locationId, string meetName, QDate date, int meetId):location(locationName, state, locationId){
    this -> meetName = meetName;
    this -> date = date;
    this -> meetId = meetId;
}
int meet::getMeetId(){
    return this->meetId;
}
QDate meet::getDate(){
    return this->date;
}
string meet::getMeetName(){
    return this->meetName;
}
string meet::getDataForCSVMeet(){
    string temp = "1,";
    temp += to_string(getMeetId()) + ",";
    temp += to_string(getLocationId()) + ",";
    temp += getMeetName() + ",";
    char buffer[11];
    temp += getDate().toString("MM/dd/yyyy").toStdString();
    return temp;
}

//throws functions
throws::throws()
{
    athleteName = "";
    throwId = -1;
    meetId = -1;
    schoolId = -1;
    throwType = throwType::Hammer;
    distance = -10;
}
throws::throws(string athleteName, int throwId, int meetId, int schoolId, enum throwType throwType, double distance)
{
    this -> athleteName = athleteName;
    this -> throwId = throwId;
    this -> meetId = meetId;
    this -> schoolId = schoolId;
    this -> throwType = throwType;
    this -> distance = distance;
}
string throws::getAthleteName(){
    return this->athleteName;
}
int throws::getThrowId(){
    return this->throwId;
}
int throws::getMeetId(){
    return this->meetId;
}
int throws::getSchoolId(){
    return this->schoolId;
}
enum throwType throws::getThrowType(){
    return this->throwType;
}
double throws::getDistance(){
    return this->distance;
}
string throws::getDataForCSVThrow(){
    string temp = "0,";
    temp += to_string(getThrowId()) + ",";
    temp += to_string(getMeetId()) + ",";
    temp += getAthleteName() + ",";
    temp += to_string(getDistance()) + ",";
    temp += to_string(getSchoolId()) + ",";
    if(getThrowType() == throwType::Hammer){
        temp += "0";
    }
    else if(getThrowType() == throwType::Weight){
        temp += "1";
    }
    return temp;
}

//holder functions
holder::holder(){}
void holder::readCSV(){
    QString filePath = "../ThrowManager/data.csv";
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "1. Failed to open file: " << file.errorString();
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if(parts[0].toInt() == 2){
            locations.push_back(location(parts[2].toStdString(),parts[3].toStdString(),parts[1].toInt()));
        }
    }
    in.seek(0);
    throwType throwType = throwType::Hammer;
    location tempLocation;
    QDate tempTime;
    while(!in.atEnd()){
        int count = 0, dataType = -1;
        QString line = in.readLine();
        QStringList parts = line.split(',');
        dataType = parts[0].toInt();
        parts.removeFirst();
        switch(dataType){
            case 0:
                if(parts[5].toInt() == 0){
                    throwType = throwType::Hammer;
                }
                else if(parts[5].toInt() == 1){
                    throwType = throwType::Weight;
                }
                throwsList.push_back(throws(parts[2].toStdString(),parts[0].toInt(),parts[1].toInt(),parts[4].toInt(),throwType,parts[3].toDouble()));
            break;
            case 1:
                for(location location : locations){
                    if(location.getLocationId() == parts[1].toInt()){
                        tempLocation = location;
                    }
                }
                tempTime = QDate::fromString(parts[3], "MM/dd/yyyy");
                //qDebug() << tempTime;
                meets.push_back(meet(tempLocation.getLocationName(),tempLocation.getState(),tempLocation.getLocationId(),parts[2].toStdString(),tempTime,parts[0].toInt()));
            break;
            case 2:
                //do nothing
            break;
            default:
                qDebug() << "Shouldn't be here";
            break;
        }
    }
    //qDebug() << "throws: " << throwsList.size() << " locations: " << locations.size() << " meets: " << meets.size();
    file.close();
}
void holder::setCSV(){
    QString filePath = "../ThrowManager/data.csv";
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "2. Failed to open file: " << file.errorString();
        return;
    }

    QTextStream out(&file);
    for(location location : locations){
        out << QString::fromStdString(location.getDataForCSVLocation()) << "\n";
    }
    for(meet meet: meets){
        out << QString::fromStdString(meet.getDataForCSVMeet()) << "\n";
    }
    for(throws throws: throwsList){
        out << QString::fromStdString(throws.getDataForCSVThrow()) << "\n";
    }
    file.close();
}
vector<location> holder::getLocationVector(){
    return this->locations;
}
vector<meet> holder::getMeetVector(){
    return this->meets;
}
vector<throws> holder::getThrowsVector(){
    return this->throwsList;
}
location holder::findLocation(int id){
    location temp = location();
    for(location i : locations){
        if(i.getLocationId() == id){
            temp = i;
        }
    }
    return temp;
}
meet holder::findMeet(int id){
    meet temp = meet();
    for(meet i : meets){
        if(i.getMeetId() == id){
            temp = i;
        }
    }
    return temp;
}
void holder::setLocationVector(vector<location> data){
    this->locations = data;
    setCSV();
}
void holder::setMeetVector(vector<meet> data){
    this->meets = data;
    setCSV();
}
void holder::setThrowsVector(vector<throws> data){
    this->throwsList = data;
    setCSV();
}
vector<string> holder::findAthleteInfo(int id){
    vector<string> temp;

    return temp;
}
vector<string> holder::findMeetInfo(int id){
    vector<string> temp;

    return temp;
}
vector<string> holder::findLocationInfo(int id){
    vector<string> temp;

    return temp;
}

string throwTypeToString(throwType type){
    switch (type) {
        case throwType::Hammer:
            return "Hammer";
        case throwType::Weight:
            return "Weight";
        default:
            return "Unknown";
    }
}

vector<throws> sortThrowsVector(holder data, int option){
    vector<throws> tempVector = data.getThrowsVector();
    int minIndex, vectorSize = tempVector.size();
    switch(option){
        case 0:
        break;
        case 1:
            //furthest to shortest
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (tempVector[j].getDistance() > tempVector[minIndex].getDistance()) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 2:
            //shortest to furthest
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (tempVector[j].getDistance() < tempVector[minIndex].getDistance()) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 3:
            //Athlete Name A-Z
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (stringToLower(tempVector[j].getAthleteName()).compare(stringToLower(tempVector[minIndex].getAthleteName())) <= 0) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 4:
            //Athlete Name Z-A
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (stringToLower(tempVector[j].getAthleteName()).compare(stringToLower(tempVector[minIndex].getAthleteName())) > 0) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 5:
            //Meet Name A-Z
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (stringToLower(data.findMeet(tempVector[j].getMeetId()).getMeetName()).compare(stringToLower(data.findMeet(tempVector[minIndex].getMeetId()).getMeetName())) <= 0) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 6:
            //Meet Name Z-A
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (stringToLower(data.findMeet(tempVector[j].getMeetId()).getMeetName()).compare(stringToLower(data.findMeet(tempVector[minIndex].getMeetId()).getMeetName())) > 0) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 7:
            //School Name A-Z
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (stringToLower(data.findLocation(tempVector[j].getSchoolId()).getLocationName()).compare(stringToLower(data.findLocation(tempVector[minIndex].getSchoolId()).getLocationName())) <= 0) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 8:
            //School Name Z-A
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (stringToLower(data.findLocation(tempVector[j].getSchoolId()).getLocationName()).compare(stringToLower(data.findLocation(tempVector[minIndex].getSchoolId()).getLocationName())) > 0) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 9:
            //Newest to Oldest
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (data.findMeet(tempVector[j].getMeetId()).getDate() > data.findMeet(tempVector[minIndex].getMeetId()).getDate()) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 10:
            //Oldest to Newest
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                    if (data.findMeet(tempVector[j].getMeetId()).getDate() < data.findMeet(tempVector[minIndex].getMeetId()).getDate()) {
                        minIndex = j;
                    }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        default:

        break;
    }
    return tempVector;
}

vector<throws> filterThrowsVector(vector<throws> data, holder extraData, double minDistance, double maxDistance, QDate minDate, QDate maxDate, int throwType){
    vector<throws> temp;
    QStringList throwTypeList;
    switch(throwType){
        case 1:
            throwTypeList << "Hammer";
        break;
        case 2:
            throwTypeList << "Weight";
        break;
        default:
            throwTypeList << "Hammer" << "Weight";
        break;
    }
    for(throws item : data){
        if((item.getDistance() >= minDistance) && (item.getDistance() <= maxDistance) && (extraData.findMeet(item.getMeetId()).getDate() >= minDate) &&  (extraData.findMeet(item.getMeetId()).getDate() <= maxDate) && (throwTypeList.contains(QString::fromStdString(throwTypeToString(item.getThrowType()))))){
            temp.push_back(item);
        }
    }
    return temp;
}

vector<meet> sortMeetsVector(holder data, int option){
    vector<meet> tempVector = data.getMeetVector();
    int minIndex, vectorSize = tempVector.size();
    switch(option){
        case 0:
        break;
        case 1:
            //Meet Name A-Z
            for(int i=0; i<vectorSize; i++){
                    minIndex = i;
                    for (int j = i + 1; j < vectorSize; j++) {
                        if (tempVector[j].getMeetName().compare(tempVector[minIndex].getMeetName()) <= 0) {
                            minIndex = j;
                        }
                    }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 2:
            //Meet Name Z-A
            for(int i=0; i<vectorSize; i++){
                    minIndex = i;
                    for (int j = i + 1; j < vectorSize; j++) {
                        if (tempVector[j].getMeetName().compare(tempVector[minIndex].getMeetName()) > 0) {
                            minIndex = j;
                        }
                    }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 3:
            //Newest to Oldest
            for(int i=0; i<vectorSize; i++){
                    minIndex = i;
                    for (int j = i + 1; j < vectorSize; j++) {
                        if (tempVector[j].getDate() > tempVector[minIndex].getDate()) {
                            minIndex = j;
                        }
                    }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 4:
            //Oldest to Newest
            for(int i=0; i<vectorSize; i++){
                    minIndex = i;
                    for (int j = i + 1; j < vectorSize; j++) {
                        if (tempVector[j].getDate() < tempVector[minIndex].getDate()) {
                            minIndex = j;
                        }
                    }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 5:
            //Location A-Z
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                        if (tempVector[j].getLocationName().compare(tempVector[minIndex].getLocationName()) <= 0) {
                            minIndex = j;
                        }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        case 6:
            //Location Z-A
            for(int i=0; i<vectorSize; i++){
                minIndex = i;
                for (int j = i + 1; j < vectorSize; j++) {
                        if (tempVector[j].getLocationName().compare(tempVector[minIndex].getLocationName()) > 0) {
                            minIndex = j;
                        }
                }
                std::swap(tempVector[minIndex],tempVector[i]);
            }
        break;
        default:
        break;
    }

    return tempVector;
}
vector<meet> filterMeetsVector(vector<meet> data, QString locationFilter, QDate minDate, QDate maxDate){
    vector<meet> temp;
    for(meet item : data){
        if((item.getDate()>=minDate)&&(item.getDate()<=maxDate)&&(QString::fromStdString(item.getLocationName()+" ("+item.getState()+")")==locationFilter)){
                temp.push_back(item);
        }
    }
    if(temp.empty()){
        for(meet item : data){
            if((item.getDate()>=minDate)&&(item.getDate()<=maxDate)&&(QString::fromStdString(item.getState())==locationFilter)){
                    temp.push_back(item);
            }
        }
    }
    if(temp.empty()){
        for(meet item : data){
            if((item.getDate()>=minDate)&&(item.getDate()<=maxDate)&&(locationFilter=="")){
                    temp.push_back(item);
            }
        }
    }
    return temp;
}

vector<location> sortLocationVector(holder data, int option){
    vector<location> tempVector = data.getLocationVector();
    int minIndex, vectorSize = tempVector.size();
    switch(option){
    case 0:
        break;
    case 1:
        //Location Name A-Z
        for(int i=0; i<vectorSize; i++){
            minIndex = i;
            for (int j = i + 1; j < vectorSize; j++) {
                    if (tempVector[j].getLocationName().compare(tempVector[minIndex].getLocationName()) <= 0) {
                            minIndex = j;
                    }
            }
            std::swap(tempVector[minIndex],tempVector[i]);
        }
        break;
    case 2:
        //Location Name Z-A
        for(int i=0; i<vectorSize; i++){
            minIndex = i;
            for (int j = i + 1; j < vectorSize; j++) {
                    if (tempVector[j].getLocationName().compare(tempVector[minIndex].getLocationName()) > 0) {
                            minIndex = j;
                    }
            }
            std::swap(tempVector[minIndex],tempVector[i]);
        }
        break;
    case 3:
        //State A-Z
        for(int i=0; i<vectorSize; i++){
            minIndex = i;
            for (int j = i + 1; j < vectorSize; j++) {
                    if (tempVector[j].getState().compare(tempVector[minIndex].getState()) <= 0) {
                            minIndex = j;
                    }
            }
            std::swap(tempVector[minIndex],tempVector[i]);
        }
        break;
    case 4:
        //State Z-A
        for(int i=0; i<vectorSize; i++){
            minIndex = i;
            for (int j = i + 1; j < vectorSize; j++) {
                    if (tempVector[j].getState().compare(tempVector[minIndex].getState()) > 0) {
                            minIndex = j;
                    }
            }
            std::swap(tempVector[minIndex],tempVector[i]);
        }
        break;
    }

    return tempVector;
}

string stringToLower(string input){
    string str = input;
    std::transform(str.begin(), str.end(), str.begin(),[](unsigned char c) { return std::tolower(c); });
    return str;
}
