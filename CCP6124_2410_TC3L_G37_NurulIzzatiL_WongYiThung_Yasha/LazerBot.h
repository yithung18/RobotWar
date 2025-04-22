#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


void UpdateWaitingList(string id, int x, int y, int numOfLives, Battlefield* battlefield, ofstream &outputFile);
void UpdateDestroyedList(string id, int x, int y, Battlefield* battlefield, ofstream &outputFile);
void upgradeRobot(string id, int x, int y, Battlefield* battlefield, ofstream &outputFile);


class LazerBot: public ShootingRobot
{
private:
    //data members
    static int robotAutoIncrement_;
    Battlefield* battlefield_;

public:

    virtual Battlefield* battlefield()
    {
        return battlefield_;
    };
    virtual void setBattlefield(Battlefield* battlefield)
    {
        battlefield_ = battlefield;
    };

    LazerBot(string id = "", int x = -1, int y = -1, int numOfLive = 3)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        numOfLives_ = numOfLive;

        robotAutoIncrement_++;
    }

    static int robotAutoIncrement()
    {
        return robotAutoIncrement_;
    }

    virtual ~LazerBot() {}

    virtual void actionFire(ofstream &outputFile)
    {
        cout << "\nShootingRobot actionFire" << endl;
        outputFile << "\nShootingRobot actionFire" << endl;

        bool enemyOccured = false;
        for (auto it = battlefield()->getVecRobots().begin(); it != battlefield()->getVecRobots().end();)
        {
            Robot* robot = *it;
            if(robot->x() > robotPositionX)
            {
                enemyOccured = true;
                robot->deductLive();
                this->incrementNumOfKills();

                cout << "The location (" << robot->x() << ", " <<  robot->y() << ") has robot " << *robot << endl;
                outputFile << "The location (" << robot->x() << ", " << robot->y()  << ") has robot " << *robot << endl;

                if(robot->numLives()>0)
                {
                    //move robot to waiting list
                    UpdateWaitingList(robot->id(), robot->x(), robot->y(), robot->numLives(), battlefield_, outputFile);
                    cout << "Robot id " << *robot << " has been fired but still alive and moved to waitingRobot queue." << endl;
                    outputFile << "Robot id " << *robot << " has been fired but still alive and moved to waitingRobot queue." << endl;
                }

                else if(robot->numLives() == 0)
                {
                    //move to destroy list
                    UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                    cout << "Robot " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                    outputFile << "Robot " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                }

                if(this->numOfKills_ == 3)
                {
                    int intNewId = robotAutoIncrement() + 1;
                    string newId = "no robot";

                    if(intNewId >= 1 && intNewId <= 9)
                    {
                        newId = "SB0" + to_string(intNewId);
                    }
                    else
                    {
                        newId = "SB" + to_string(intNewId);
                    }

                    robot->setId(newId);

                    cout << "\nLazerBot has killed 3 robots" << endl;
                    outputFile << "\nLazerBot has killed 3 robots" << endl;

                    cout << "LazerBot has upgraded to ShooterBot." << endl;
                    outputFile << "LazerBot has upgraded to ShooterBot." << endl;

                    cout << "Upgraded robot new ID = " << robot->id() << endl;
                    outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                    battlefield()->eraseRobotById(id_, outputFile);
                    upgradeRobot(newId, robotPositionX, robotPositionY, battlefield_, outputFile);
                    break;
                }
            }
            else
            {
                ++it;
            }
        }

        if(!enemyOccured)
        {
            cout << "There is no enemy to fire." << endl;
            outputFile << "There is no enemy to fire." << endl;

        }


        cout << endl;
        outputFile << endl;
    }

    virtual void actions(ofstream &outputFile)
    {
        actionFire(outputFile);
    }
};
int LazerBot::robotAutoIncrement_ = 0;

