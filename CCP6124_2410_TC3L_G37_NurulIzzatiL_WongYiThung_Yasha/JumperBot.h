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


class JumperBot: public SteppingRobot
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

    JumperBot(string id = "", int x = -1, int y = -1, int numOfLive = 3)
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

    virtual ~JumperBot() {}

    virtual void actionStep(ofstream &outputFile)
    {
        cout << "\nSteppingRobot actionStep" << endl;
        outputFile << "\nSteppingRobot actionStep" << endl;

        int steps[8][2] =
        {
            {-1, -1}, {0, -1}, {1, -1}, // top-left, top, top-right
            {-1,  0},          {1,  0}, // left,       right
            {-1,  1}, {0,  1}, {1,  1}  // bottom-left, bottom, bottom-right
        };

        int stepX;
        int stepY;

        //find robot
        for(int step = 0; step < 3; step++)
        {
            do
            {
                int stepIndex = rand() % 8;
                int dx = steps[stepIndex][0];
                int dy = steps[stepIndex][1];

                robotPositionX = robotPositionX + dx;
                robotPositionY = robotPositionY + dy;

            } while ( robotPositionX < 0 || robotPositionY < 0 || robotPositionX >= battlefield()->getWidth()
                     || robotPositionY >= battlefield()->getHeight() );

            cout << "Step to (" << robotPositionX << ", " << robotPositionY << ")" << endl;
            outputFile << "Step to (" << robotPositionX << ", " << robotPositionY << ")" << endl;


            bool enemyOccured = false;
            for (auto robot : battlefield()->getVecRobots())
            {
                if(robot->x() == robotPositionX && robot->y() == robotPositionY)
                {
                    enemyOccured = true;
                    robot->deductLive();
                    this->incrementNumOfKills();

                    cout << "The location (" << robotPositionX << ", " << robotPositionY << ") has robot " << *robot << endl;
                    outputFile << "The location (" << robotPositionX << ", " << robotPositionY << ") has robot " << *robot << endl;

                    if(robot->numLives()>0)
                    {
                        //move robot to waiting list
                        UpdateWaitingList(robot->id(), robot->x(), robot->y(), robot->numLives(), battlefield_, outputFile);
                        cout << "Robot ID " << robot->id() << " has been fired but still alive and moved to waitingRobot queue." << endl ;
                        outputFile << "Robot ID " << robot->id() << " has been fired but still alive and moved to waitingRobot queue." << endl ;
                    }
                    else
                    {
                        //move to destroy list
                        UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                        cout << "Robot ID " << robot->id() << " has been destroyed and moved to the destroyed list." << endl;
                        outputFile << "Robot ID " << robot->id() << " has been destroyed and moved to the destroyed list." << endl;
                    }
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

                    cout << "\nJumperBot has killed 3 robots" << endl;
                    outputFile << "\nJumperBot has killed 3 robots" << endl;

                    cout << "JumperBot has upgraded to UltimateRobot." << endl;
                    outputFile << "JumperBot has upgraded to UltimateRobot." << endl;

                    cout << "Upgraded robot new ID = " << robot->id() << endl;
                    outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                    battlefield()->eraseRobotById(id_, outputFile);
                    upgradeRobot(newId, robotPositionX, robotPositionY, battlefield_, outputFile);
                }
                break;
            }

            setX(robotPositionX);
            setY(robotPositionY);

            if( !enemyOccured )
            {
                cout << "There is no enemy to step." << endl ;
                outputFile << "There is no enemy to step." << endl ;
            }
        }
    }

    virtual void actions(ofstream &outputFile)
    {
        actionStep(outputFile);
    }
};
int JumperBot::robotAutoIncrement_ = 0;
