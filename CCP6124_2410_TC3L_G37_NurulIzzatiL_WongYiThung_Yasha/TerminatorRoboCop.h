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
void battlefield_actionLook(int robotPositionX, int robotPositionY, Battlefield* battlefield, ofstream &outputFile);


class TerminatorRoboCop: public SeeingRobot, public ShootingRobot, public SteppingRobot
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

    TerminatorRoboCop(string id = "", int x = -1, int y = -1, int numOfLive = 3)
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

    virtual ~TerminatorRoboCop() {}

    virtual void actionLook(ofstream &outputFile)
    {
        cout << "\nSeeingRobot actionLook" << endl;
        outputFile << "\nSeeingRobot actionLook" << endl;

        battlefield_actionLook(robotPositionX, robotPositionY, battlefield_, outputFile);

        cout << endl;
        outputFile << endl;
    }

    virtual void actionFire(ofstream &outputFile)
    {
        cout << "ShootingRobot actionFire" << endl;
        outputFile << "ShootingRobot actionFire" << endl;

        for(int fire = 3; fire > 0; fire--)
        {
            int randDx, randDy, total, fireX, fireY =0;
            randDx = -1;
            randDy = -1;
            total = 0;
            fireX = -1;
            fireY = -1;
            do{
                do{
                    randDx = rand() % 11-5 ; // Generates numbers in range [0, 10]
                    randDy = rand() % 11-5 ; // Generates numbers in range [0, 10]

                } while ( randDx == 0 && randDy == 0 );

                total = randDx + randDy;

                fireX = robotPositionX + randDx;
                fireY = robotPositionY + randDy;

            } while ( total < 1 || total > 10 || fireX < 0 || fireY < 0 || fireX >= battlefield()->getWidth() || fireY >= battlefield()->getHeight() );

            cout << "Fire at (" << fireX << ", " << fireY << ")" << endl;
            outputFile << "Fire at (" << fireX << ", " << fireY << ")" << endl;

            bool enemyOccured = false;
            for (auto robot : battlefield()->getVecRobots())
            {
                if(robot->x() == fireX && robot->y() == fireY)
                {
                    enemyOccured = true;
                    robot->deductLive();
                    this->incrementNumOfKills();

                    cout << "The location (" << fireX << ", " << fireY << ") has robot " << *robot << endl;
                    outputFile << "The location (" << fireX << ", " << fireY << ") has robot " << *robot << endl;

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

                    cout << "\nTerminatorRoboCop has killed 3 robots" << endl;
                    outputFile << "\nTerminatorRoboCop has killed 3 robots" << endl;

                    cout << "TerminatorRoboCop has upgraded to TerminatorRoboCop." << endl;
                    outputFile << "TerminatorRoboCop has upgraded to TerminatorRoboCop." << endl;

                    cout << "Upgraded robot new ID = " << robot->id() << endl;
                    outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                    battlefield()->eraseRobotById(id_, outputFile);
                    upgradeRobot(newId, robotPositionX, robotPositionY, battlefield_, outputFile);
                }
                break;
            }

            if( !enemyOccured )
            {
                cout << "There is no enemy to fire." << endl ;
                outputFile << "There is no enemy to fire." << endl ;
            }
        }

        cout << endl ;
        outputFile << endl;
    }

    virtual void actionStep(ofstream &outputFile)
    {
        cout << "SteppingRobot actionStep" << endl;
        outputFile << "SteppingRobot actionStep" << endl;

        int steps[8][2] =
        {
            {-1, -1}, {0, -1}, {1, -1}, // top-left, top, top-right
            {-1,  0},          {1,  0}, // left,       right
            {-1,  1}, {0,  1}, {1,  1}  // bottom-left, bottom, bottom-right
        };

        //find robot
        bool enemyOccured = false;

        for(int i = 0; i < 8; i++)
        {
            int checkX = 0;
            int checkY = 0;

            int dx = steps[i][0];
            int dy = steps[i][1];

            checkX = robotPositionX + dx;
            checkY = robotPositionY + dy;

            if (checkX >= 0 && checkX < battlefield()->getWidth() && checkY >= 0 && checkY < battlefield()->getHeight())
            {
                Robot* robot = battlefield()->searchRobot(checkX, checkY);    //search robot, if != nullptr then kill (function inside class Battlefield)
                if(robot != nullptr) //means have robot
                {
                    cout << "Step and kill the robot id " << robot->id() << " at (" << checkX << ", " << checkY << ")" << endl;
                    outputFile << "Step and kill the robot id " << robot->id() << " at (" << checkX << ", " << checkY << ")" << endl;

                    enemyOccured = true;
                    robot->deductLive();
                    this->incrementNumOfKills();

                    if(robot->numLives()>0)
                    {
                        //move robot to waiting list
                        UpdateWaitingList(robot->id(), robot->x(), robot->y(), robot->numLives(), battlefield_, outputFile);
                        cout << "Robot " << *robot << ", has been stepped and killed but still alive and moved to waitingRobot queue." << endl;
                        outputFile << "Robot " << *robot << ", has been stepped and killed but still alive and moved to waitingRobot queue." << endl;
                    }
                    else if(robot->numLives() == 3)
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
                            newId = "UR0" + to_string(intNewId);
                        }
                        else
                        {
                            newId = "UR" + to_string(intNewId);
                        }

                        robot->setId(newId);

                        cout << "\nTerminatorRoboCop has killed 3 robots" << endl;
                        outputFile << "\nTerminatorRoboCop has killed 3 robots" << endl;

                        cout << "TerminatorRoboCop has upgraded to UltimateRobot." << endl;
                        outputFile << "TerminatorRoboCop has upgraded to UltimateRobot." << endl;

                        cout << "Upgraded robot new ID = " << robot->id() << endl;
                        outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                        battlefield()->eraseRobotById(id_, outputFile);
                        upgradeRobot(newId, checkX, checkY, battlefield_, outputFile);
                    }

                    setX(checkX);
                    setY(checkY);

                    break;
                }
            }
        }

        //if there is no enemy within the 8 neighbors, then random move
        if(!enemyOccured)
        {
            cout << "There is no robot around." << endl;
            outputFile << "There is no robot around." << endl;

            int checkX, checkY;
            do
            {
                int stepsIndex = rand() % 8;
                int dx = steps[stepsIndex][0];
                int dy = steps[stepsIndex][1];

                checkX = robotPositionX + dx;
                checkY = robotPositionY + dy;

            } while (checkX < 0 || checkX >= battlefield()->getWidth() || checkY < 0 || checkY >= battlefield()->getHeight());

            setX(checkX);
            setY(checkY);
            cout << "Stepping randomly to (" << checkX << ", " << checkY << ")" << endl;
            outputFile << "Stepping randomly to (" << checkX << ", " << checkY << ")" << endl;
        }
    }

    virtual void actions(ofstream &outputFile)
    {
        actionLook(outputFile);
        actionFire(outputFile);
        actionStep(outputFile);
    }
};
int TerminatorRoboCop::robotAutoIncrement_ = 0;


