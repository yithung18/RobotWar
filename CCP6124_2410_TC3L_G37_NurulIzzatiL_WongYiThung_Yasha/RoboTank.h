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


class RoboTank: public ShootingRobot
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

    RoboTank(string id = "", int x = -1, int y = -1, int numOfLive = 3)
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

    virtual ~RoboTank() {}

    virtual void actionFire(ofstream &outputFile)
    {
        cout << "\nShootingRobot actionFire" << endl;
        outputFile << "\nShootingRobot actionFire" << endl;

        int fireX, fireY;
        while (true)
        {
            int randDx = rand() % battlefield()->getWidth()-2;
            int randDy = rand() % battlefield()->getHeight()-2;

            fireX = robotPositionX + randDx;
            fireY = robotPositionY + randDy;

            if (fireX >= 0 && fireY >= 0 && fireX < battlefield()->getWidth() && fireY < battlefield()->getHeight() && fireX != robotPositionX) {
                break;
            }
        }
        cout << "Fire at (" << fireX << ", " << fireY << ")" << endl;
        outputFile << "Fire at (" << fireX << ", " << fireY << ")" << endl;

        bool enemyOccured = false;
        for (auto robot : battlefield()->getVecRobots())
        {
            if(robot->x() == fireX && robot->y() == fireY)
            {
                cout << "The location (" << fireX << ", " << fireY << ") has robot id " << robot->id() << endl;
                outputFile << "The location (" << fireX << ", " << fireY << ") has robot id " << robot->id() << endl;

                enemyOccured = true;
                robot->deductLive();
                this->incrementNumOfKills();


                if( robot->numLives()>0 )
                {
                    //move robot to waiting list

                    UpdateWaitingList(robot->id(), robot->x(), robot->y(),robot->numLives(), battlefield_, outputFile);
                    cout << "Robot id " << *robot << ", has been fired but still alive and moved to waitingRobot queue." << endl;
                    outputFile << "Robot id " << *robot << ", has been fired but still alive and moved to waitingRobot queue." << endl;
                }
                else if( robot->numLives() == 0 )
                {
                    //move to destroy list
                    UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                    cout << "Robot id " << *robot << ", has been destroyed and moved to the destroyed list." << endl;
                    outputFile << "Robot id " << *robot << ", has been destroyed and moved to the destroyed list." << endl;
                }

                if(this->numOfKills_ == 3)
                {
                        int intNewId = robotAutoIncrement() + 1;
                        string newId = "no robot";

                        if(intNewId >= 1 && intNewId <= 9)
                        {
                            newId = "UR0" + to_string(intNewId);
                        }
                        else
                        {
                            newId = "UR" + to_string(intNewId);
                        }

                        robot->setId(newId);

                        cout << "\nRoboTank has killed 3 robots" << endl;
                        outputFile << "\nRoboTank has killed 3 robots" << endl;

                        cout << "RoboTank has upgraded to UltimateRobot." << endl;
                        outputFile << "RoboTank has upgraded to UltimateRobot." << endl;

                        cout << "Upgraded robot new ID = " << robot->id() << endl;
                        outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                        battlefield()->eraseRobotById(id_, outputFile);
                        upgradeRobot(newId, robotPositionX, robotPositionY, battlefield_, outputFile);
                }
                break;
            }
        }

        if(!enemyOccured)
        {
            cout << "There is no enemy to fire." << endl;
            outputFile << "There is no enemy to fire." << endl;
        }
    }

    virtual void actions(ofstream &outputFile)
    {
        actionFire(outputFile);
    }

};
int RoboTank::robotAutoIncrement_ = 0;
