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


class Terminator: public SeeingRobot, public MovingRobot
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

    Terminator(string id = "", int x = -1, int y = -1, int numOfLive = 3)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        numOfLives_ = numOfLive;

        robotAutoIncrement_++;
    }

    virtual ~Terminator() {}

    static int robotAutoIncrement()
    {
        return robotAutoIncrement_;
    }

    virtual void actionLook(ofstream &outputFile)
    {
        cout << "\nSeeingRobot actionLook" << endl;
        outputFile << "\nSeeingRobot actionLook" << endl;

        battlefield_actionLook(robotPositionX, robotPositionY, battlefield_, outputFile);
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
            checkX = -99;
            checkY = -99;

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
                        UpdateWaitingList(robot->id(), robot->x(), robot->y(), robot->numLives(), battlefield_, outputFile);
                        cout << "Robot " << *robot << " has been killed but still alive and moved to waitingRobot queue." << endl;
                        outputFile << "Robot " << *robot << " has been killed but still alive and moved to waitingRobot queue." << endl;
                    }
                    else
                    {
                        //move to destroy list
                        UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);
                        cout << "Robot " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                        outputFile << "Robot " << *robot << " has been destroyed and moved to the destroyed list." << endl;
                    }

                    if(this->numOfKills_ == 3)
                    {
                        int intNewId = Terminator::robotAutoIncrement() + 1;
                        string newId = "no robot id";
                        if ( intNewId >= 1 && intNewId <= 9 )
                        {
                            newId = "TR0" + to_string(intNewId);
                        }
                        else
                        {
                            newId = "TR" + to_string(intNewId);
                        }

                        robot->setId(newId);

                        cout << "\nTerminator has killed 3 robots" << endl;
                        outputFile << "\nTerminator has killed 3 robots" << endl;

                        cout << "Terminator has upgraded to TerminatorRoboCop." << endl;
                        outputFile << "Terminator has upgraded to TerminatorRoboCop." << endl;

                        cout << "Upgraded robot new ID = " << robot->id() << endl;
                        outputFile << "Upgraded robot new ID = " << robot->id() << endl;

                        battlefield()->eraseRobotById(id_, outputFile);
                        upgradeRobot(newId, robotPositionX, robotPositionY, battlefield_, outputFile);
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

            if (checkX >= 0 && checkX <= battlefield()->getWidth() && checkY >= 0 && checkY <= battlefield()->getHeight())
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


    virtual void actions(ofstream &outputFile)
    {
        actionLook(outputFile);
        actionMove(outputFile);
    }
};
int Terminator::robotAutoIncrement_ = 0;
