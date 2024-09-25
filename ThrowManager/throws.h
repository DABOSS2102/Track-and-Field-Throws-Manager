#ifndef THROWS_H
#define THROWS_H

#include "qdatetime.h"
using namespace std;
#include <vector>
#include <string>

enum class throwType{
    Hammer,
    Weight
};

string throwTypeToString(throwType type);

class location{
    protected:
        string locationName, state;
        int locationId;
    public:
        location();
        location(string locationName, string state, int locationId);
        int getLocationId();
        string getLocationName();
        string getState();
        string getDataForCSVLocation();
};

class meet : public location{
    protected:
        string meetName;
        QDate date;
        int meetId;
    public:
        meet();
        meet(string locationName, string state, int locationId, string meetName, QDate date, int meetId);
        int getMeetId();
        QDate getDate();
        string getMeetName();
        string getDataForCSVMeet();
};

class throws {
    protected:
        string athleteName;
        int throwId, meetId, schoolId;
        throwType throwType;
        double distance;
    public:
        throws();
        throws(string athleteName, int throwId, int meetId, int schoolId, enum throwType throwType, double distance);
        string getAthleteName();
        int getThrowId();
        int getMeetId();
        int getSchoolId();
        enum throwType getThrowType();
        double getDistance();
        string getDataForCSVThrow();
};

class holder{
    protected:
        vector<location> locations;
        vector<meet> meets;
        vector<throws> throwsList;
    public:
        holder();
        void readCSV();
        void setCSV();
        vector<location> getLocationVector();
        vector<meet> getMeetVector();
        vector<throws> getThrowsVector();
        void setLocationVector(vector<location> data);
        void setMeetVector(vector<meet> data);
        void setThrowsVector(vector<throws> data);
        location findLocation(int id);
        meet findMeet(int id);
        vector<string> findAthleteInfo(int id);
        vector<string> findMeetInfo(int id);
        vector<string> findLocationInfo(int id);
};

vector<throws> sortThrowsVector(holder data, int option);
vector<throws> filterThrowsVector(vector<throws> data, holder extraData, double minDistance, double maxDistance, QDate minDate, QDate maxDate, int throwType);

vector<meet> sortMeetsVector(holder data, int option);
vector<meet> filterMeetsVector(vector<meet> data, QString locationFilter, QDate minDate, QDate maxDate);

vector<location> sortLocationVector(holder data, int option);

string stringToLower(string input);

#endif // THROWS_H
