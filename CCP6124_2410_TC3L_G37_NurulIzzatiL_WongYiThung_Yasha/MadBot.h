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


class MadBot: public ShootingRobot
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

    MadBot(string id = "", int x = -1, int y = -1, int numOfLive = 3)
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

    virtual ~MadBot() {}

    virtual void actionFire(ofstream &outputFile)
    {
        cout << "\nShootingRobot actionFire" << endl;
        outputFile << "\nShootingRobot actionFire" << endl;

        int fires[8][2] =
        {
            {-1, -1}, {0, -1}, {1, -1}, // top-left, top, top-right
            {-1,  0},          {1,  0}, // left,       right
            {-1,  1}, {0,  1}, {1,  1}  // bottom-left, bottom, bottom-right
        };

        int fireX, fireY;
        do
        {
            int randFire = rand() % 8;

            fireX = fires[randFire][0];
            fireY = fires[randFire][1];

            fireX = robotPositionX + fireX;
            fireY = robotPositionY + fireY;

        } while( fireX < 0 || fireY < 0 || fireX >= battlefield()->getWidth() || fireY >= battlefield()->getHeight() );

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

                if(robot->numLives()>0)
                {
                    //move robot to waiting list

                    UpdateWaitingList(robot->id(), robot->x(), robot->y(),robot->numLives(), battlefield_, outputFile);
                    cout << "Robot " << *robot << ", has been fired but still alive and moved to waitingRobot queue." << endl;
                    outputFile << "Robot " << *robot << ", has been fired but still alive and moved to waitingRobot queue." << endl;
                }
                else
                {
                    //move to destroy list
                    UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                    cout << "Robot " << *robot << ", has been destroyed and moved to the destroyed list." << endl;
                    outputFile << "Robot " << *robot << ", has been destroyed and moved to the destroyed list." << endl;
                }

                if(this->numOfKills_ == 3)
                {
                    int intNewId = robotAutoIncrement() + 1;
                    string newId = "no robot";

                    if(intNewId >= 1 && intNewId <= 9)
                    {
                        newId = "RT0" + to_string(intNewId);
                    }
                    else
                    {
                        newId = "RT" + to_string(intNewId);
                    }

                    robot->setId(newId);

                    cout << "\nMadBot has killed 3 robots" << endl;
                    outputFile << "\nMadBot has killed 3 robots" << endl;

                    cout << "MadBot has upgraded to RoboTank." << endl;
                    outputFile << "MadBot has upgraded to RoboTank." << endl;

                    cout << "Upgraded robot new ID = " << robot->id() << endl;
                    outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                    battlefield()->eraseRobotById(id_, outputFile);
                    upgradeRobot(newId, robotPositionX, robotPositionY, battlefield_, outputFile);
                }
            }
            break;
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
int MadBot::robotAutoIncrement_ = 0;
