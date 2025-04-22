#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


void battlefield_actionLook(int robotPositionX, int robotPositionY, Battlefield* battlefield, ofstream &outputFile);
void UpdateWaitingList(string id, int x, int y, int numOfLives, Battlefield* battlefield, ofstream &outputFile);
void UpdateDestroyedList(string id, int x, int y, Battlefield* battlefield, ofstream &outputFile);
void upgradeRobot(string id, int x, int y, Battlefield* battlefield, ofstream &outputFile);



class RoboCop: public SeeingRobot, public MovingRobot, public ShootingRobot
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

    RoboCop(string id = "", int x = -1, int y = -1, int numOfLive = 3)
    {
        id_ = id;
        robotPositionX = x;
        robotPositionY = y;
        numOfLives_ = numOfLive;

        robotAutoIncrement_++;
    }

    virtual ~RoboCop() {}

    static int robotAutoIncrement()
    {
        return robotAutoIncrement_;
    }

    virtual void actionLook(ofstream &outputFile)
    {
        cout << "\nSeeingRobot actionLook" << endl;
        outputFile << "\nSeeingRobot actionLook" << endl;

        int x = robotPositionX;
        int y = robotPositionY;

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

        // Randomly select one of the 8 moves
        int checkX, checkY;
        while (true)
        {
            int moveIndex = rand() % 8;
            int dx = moves[moveIndex][0];
            int dy = moves[moveIndex][1];

            checkX = robotPositionX + dx;
            checkY = robotPositionY + dy;

            if (checkX >= 0 && checkY >= 0 && checkX < battlefield()->getWidth() && checkY < battlefield()->getHeight()) {
                break;
            }

        }
        //check whether the location has robot or not
        bool locationOccupied = false;
        for (auto robot : battlefield()->getVecRobots())
        {
            if (robot->x() == checkX && robot->y() == checkY)
            {
                locationOccupied = true;
                cout << "This location (" << checkX << ", " << checkY << ") has robot id " << robot->id() << endl;
                outputFile << "This location (" << checkX << ", " << checkY << ") has robot id " << robot->id() << endl;

                cout << "Remained at current location and cannot move to (" << checkX << ", " << checkY << ")" << endl;
                outputFile << "Remained at current location and cannot move to (" << checkX << ", " << checkY << ")" << endl;
                break;
            }
        }

        if ( !locationOccupied )
        {
            // If the position is not occupied, move the robot
            robotPositionX = checkX;
            robotPositionY = checkY;

            setX(robotPositionX);
            setY(robotPositionY);

            cout << "Moving to (" << robotPositionX << ", " << robotPositionY << ")" << endl;
            outputFile << "Moving to (" << robotPositionX << ", " << robotPositionY << ")" << endl;
        }

        cout << endl;
        outputFile << endl;
    }

    virtual void actionFire(ofstream &outputFile)
    {
        cout << "ShootingRobot actionFire" << endl;
        outputFile << "ShootingRobot actionFire" << endl;

        for(int fire = 1; fire <= 3; fire++)
        {
            int randDx, randDy, total, fireX, fireY;
            randDx = -1;
            randDy = -1;
            total = 0;
            fireX = -1;
            fireY = -1;
            do{
                do {
                    randDx = rand() % 10 ; // Generates numbers in range [0, 10]
                    randDy = rand() % 10 ; // Generates numbers in range [0, 10]
                }
                while ( randDx == 0 && randDy == 0 );

                total = randDx + randDy;

                fireX = robotPositionX + randDx;
                fireY = robotPositionY + randDy;

            } while ( total < 1 || total > 10 || fireX < 0 || fireY < 0 || fireX >= battlefield()->getWidth() || fireY >= battlefield()->getHeight() );

            cout << "Fire to (" << fireX << ", " << fireY << ")" << endl;
            outputFile << "Fire to (" << fireX << ", " << fireY << ")" << endl;

            bool enemyOccured = false;
            for (auto robot : battlefield()->getVecRobots())
            {
                if(robot->x() == fireX && robot->y() == fireY)
                {
                    enemyOccured = true;

                    this->incrementNumOfKills();
                    robot->deductLive();

                    cout << "The location (" << fireX << ", " << fireY << ") has robot " << *robot << endl;
                    outputFile << "The location (" << fireX << ", " << fireY << ") has robot " << *robot << endl;

                    if(robot->numLives() > 0)
                    {
                        //move robot to waiting robots queue
                        cout << "Robot id " << robot->id() << " has been killed but still alive and moved to waitingRobot queue" << endl;
                        outputFile << "Robot id " << robot->id() << " has been killed but still alive and moved to waitingRobot queue" << endl;
                        UpdateWaitingList(robot->id(), robot->x(), robot->y(), robot->numLives(), battlefield_, outputFile);
                    }
                    else if(robot->numLives() == 0)
                    {
                        //move robot to destroyed robots queue
                        UpdateDestroyedList(robot->id(), robot->x(), robot->y(), battlefield_, outputFile);

                        cout << "Robot id " << robot->id() << " has been destroyed and moved to the destroyed list." << endl;
                        outputFile << "Robot id " << robot->id() << " has been destroyed and moved to the destroyed list." << endl;
                    }

                    if(this->numOfKills_ == 3)
                    {
                        int intNewId = robotAutoIncrement() + 1;
                        string newId = "no robot";

                        if(intNewId >= 1 && intNewId <= 9)
                        {
                            newId = "TR0" + to_string(intNewId);
                        }
                        else
                        {
                            newId = "TR" + to_string(intNewId);
                        }

                        robot->setId(newId);

                        cout << "\nRoboCop has killed 3 robots" << endl;
                        outputFile << "\nRoboCop has killed 3 robots" << endl;

                        cout << "RoboCop has upgraded to TerminatorRoboCop." << endl;
                        outputFile << "RoboCop has upgraded to TerminatorRoboCop." << endl;

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
                cout << "No enemy robot to fire." << endl;
                outputFile << "No enemy robot to fire." << endl;
            }
        }
    }

    virtual void actions(ofstream &outputFile)
    {
        actionLook(outputFile);
        actionMove(outputFile);
        actionFire(outputFile);
    }
};
int RoboCop::robotAutoIncrement_ = 0;
