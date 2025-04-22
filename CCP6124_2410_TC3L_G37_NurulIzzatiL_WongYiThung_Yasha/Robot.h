#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


class Robot
{
protected:
    int robotPositionX = -1;
    int robotPositionY = -1;

    string id_ = "";  //eg RC01, to display robot id on the Battlefield
    string robotType_ = "";
    string robotName_ = "";  //eg RC01_Star

    int numOfKills_ = 0;
    int numOfLives_ = 3;

public:
    Robot(string id = "", int x = 0, int y = 0)
        : id_(id), robotPositionX(x), robotPositionY(y) {}

    virtual ~Robot() { }

    void setX(int x)
    {
        robotPositionX = x;
    }
    int x()
    {
        return robotPositionX;
    }

    void setY(int y)
    {
        robotPositionY = y;
    }
    int y()
    {
        return robotPositionY;
    }

    void setId(string id)
    {
        id_ = id;
    }
    string id()
    {
        return id_;
    }

    void setRobotType(int robotType)
    {
        robotType_ = robotType;
    }
    string robotType()
    {
        return robotType_;
    }

//    void setNumOfLives( int numLives )
//    {
//        numOfLives_ = numLives ;
//    }
    int numLives()
    {
        return numOfLives_ ;
    }
    void deductLive()
    {
        numOfLives_ = numOfLives_ - 1;
    }
    void setNumOfKills( int numOfKills )
    {
        numOfKills_ = numOfKills;
    }
    int numOfKills()
    {
        return numOfKills_;
    }
    void incrementNumOfKills()
    {
        numOfKills_++;
    }

    friend ostream& operator<<(ostream &out, const Robot& r)
    {
        out << r.id_ << " at (" << r.robotPositionX << ", " << r.robotPositionY << "), "
            << "#kills: " << r.numOfKills_ << ", #lives: " << r.numOfLives_;

        return out;
    }

    virtual void actions(ofstream &outputFile) = 0;

};

