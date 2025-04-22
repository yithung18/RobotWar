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


class UltimateRobot: public SeeingRobot, public MovingRobot, public ShootingRobot, public SteppingRobot
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

    UltimateRobot(string id = "", int x = -1, int y = -1, int numOfLive = 3)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        numOfLives_ = numOfLive;

        robotAutoIncrement_++;
    }

    virtual ~UltimateRobot() {}

    virtual void actionLook(ofstream &outputFile)
    {
        cout << "\nSeeingRobot actionLook" << endl;
        outputFile << "\nSeeingRobot actionLook" << endl;

        battlefield_actionLook(robotPositionX, robotPositionY,battlefield_, outputFile);
    }
    virtual void actionMove(ofstream &outputFile)
    {
        cout << "MovingRobot actionMove" << endl;
        outputFile << "MovingRobot actionMove" << endl;

        int moves[8][2] =
        {
            {-1, -1}, {0, -1}, {1, -1}, // top-left, top, top-right
            {-1,  0},          {1,  0}, // left,       right
            {-1,  1}, {0,  1}, {1,  1}  // bottom-left, bottom, bottom-right
        };

        // find enemy robot
        int checkX, checkY;
        bool enemyOccured = false;
        for(int i = 0; i < 8; i++)
        {
            int checkX;
            int checkY;

            int dx = moves[i][0];
            int dy = moves[i][1];

            checkX = robotPositionX + dx;
            checkY = robotPositionY + dy;

            Robot* robot;
            if (checkX >= 0 && checkX < battlefield()->getWidth() && checkY >= 0 && checkY < battlefield()->getHeight())
            {
                robot = battlefield()->searchRobot(checkX, checkY);

                if(robot != nullptr) //means have enemy robot
                {
                    cout << "Move and kill the robot id " << robot->id() << " at (" << checkX << ", " << checkY << ")" << endl;
                    outputFile << "Move and kill the robot id " << robot->id() << " at (" << checkX << ", " << checkY << ")" << endl;

                    enemyOccured = true;
                    robot->deductLive();
                    this->incrementNumOfKills();

                    if(robot->numLives() > 0)
                    {
                        //move robot to waiting list
                        UpdateWaitingList(robot->id(), robot->x(), robot->y(),robot->numLives(), battlefield_, outputFile);
                        cout << "Robot id " << *robot << " has been killed but still alive and moved to waitingRobot queue." << endl;
                        outputFile << "Robot id " << *robot << " has been killed but still alive and moved to waitingRobot queue." << endl;
                    }
                    else
                    {
                        //move to destroy list
                        UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                        cout << "Robot id " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                        outputFile << "Robot id" << *robot << " has been destroyed and moved to the destroyed list." << endl;
                    }

                    setX(checkX);
                    setY(checkY);

                    break;
                }
            }

        }
        //if there is no enemy within the 8 neighbors, then random move
        if( !enemyOccured )
        {
            cout << "There is no enemy robot around." << endl;
            outputFile << "There is no enemy robot around." << endl;

            int moveIndex = rand() % 8;
            int dx = moves[moveIndex][0];
            int dy = moves[moveIndex][1];

            checkX = robotPositionX + dx;
            checkY = robotPositionY + dy;

            if (checkX >= 0 && checkX < battlefield()->getWidth() && checkY >= 0 && checkY < battlefield()->getHeight())
            {
                setX(checkX);
                setY(checkY);

                cout << "Moving randomly to (" << checkX << ", " << checkY << ")" << endl;
                outputFile << "Moving randomly to (" << checkX << ", " << checkY << ")" << endl;
            }

        }
        cout << endl;
        outputFile << endl;
    }

    virtual void actionFire(ofstream &outputFile)
    {
        cout << "ShootingRobot actionFire" << endl;
        outputFile << "ShootingRobot actionFire" << endl;

        for(int fire = 3; fire > 0; fire--)
        {
            int fireX, fireY;
            while (true)
            {
                int randDx = rand() %battlefield()->getWidth() - 3;
                int randDy = rand() %battlefield()->getHeight() - 3;

                fireX = robotPositionX + randDx;
                fireY = robotPositionY + randDy;

                if (fireX >= 0 && fireY >= 0 && fireX < battlefield()->getWidth() && fireY < battlefield()->getHeight() && fireY != robotPositionY )
                {
                    break;
                }
            }

            cout << "Fire to (" << fireX << ", " << fireY << ")" << endl;
            outputFile << "Fire to (" << fireX << ", " << fireY << ")" << endl;

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
                        UpdateWaitingList(robot->id(), robot->x(), robot->y(),robot->numLives(), battlefield_, outputFile);
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
                }
                break;
            }

            if(!enemyOccured)
            {
                cout << "There is no enemy to fire." << endl;
                outputFile << "There is no enemy to fire." << endl;
            }
        }
        cout << endl;
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
        for(int i = 0; i <= 7; i++)
        {
            int checkX = 0;
            int checkY = 0;

            int dx = steps[i][0];
            int dy = steps[i][1];

            checkX = robotPositionX + dx;
            checkY = robotPositionY + dy;

            Robot* robot = battlefield()->searchRobot(checkX, checkY);    //search robot, if != nullptr then kill (function inside class Battlefield)
            if(robot != nullptr) //means have robot
            {
                cout << "The robot " << robot->id() << " found in (" << checkX << ", " << checkY << ") has been killed." << endl;
                outputFile << "The robot " << robot->id() << " found in (" << checkX << ", " << checkY << ") has been killed." << endl;

                enemyOccured = true;
                robot->deductLive();
                this->incrementNumOfKills();

                // check whether is within boundary
                if (checkX >= 0 && checkX < battlefield()->getWidth() && checkY >= 0 && checkY < battlefield()->getHeight() && checkX != robotPositionX && checkY != robotPositionY )
                {
                    setX(checkX);
                    setY(checkY);
                }

                if(robot->numLives()>0)
                {
                    //move robot to waiting list
                    UpdateWaitingList(robot->id(), robot->x(), robot->y(),robot->numLives(), battlefield_, outputFile);
                    cout << "Robot " << *robot << " has been killed but still alive and moved to waitingRobot queue." << endl;
                    outputFile << "Robot " << *robot << " has been killed but still alive and moved to waitingRobot queue." << endl;
                }
                else if(robot->numLives() == 0)
                {
                    //move to destroy list
                    UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                    cout << "Robot " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                    outputFile << "Robot " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                }

                break;
            }
        }

        //if there is no enemy within the 8 neighbours, then random move
        if(!enemyOccured)
        {
            int checkX, checkY;

            while (true)
            {
                int stepsIndex = rand() % 8;
                int dx = steps[stepsIndex][0];
                int dy = steps[stepsIndex][1];

                checkX = robotPositionX + dx;
                checkY = robotPositionY + dy;

                if (checkX >= 0 && checkY >= 0 && checkX < battlefield()->getWidth() && checkY < battlefield()->getHeight()) {
                    break;
                }
            }

            setX(checkX);
            setY(checkY);
            cout << "Stepping randomly to (" << checkX << ", " << checkY << ")" << endl;
            outputFile << "Stepping randomly to (" << checkX << ", " << checkY << ")" << endl;
        }


    }
    virtual void actions(ofstream &outputFile)
    {
        actionLook(outputFile);
        actionMove(outputFile);
        actionFire(outputFile);
        actionStep(outputFile);
    }
};
int UltimateRobot::robotAutoIncrement_ = 0;

