#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


class Battlefield
{
protected:
    string line;

    string robotType  = "no robot type";   //RobotCop
    string robotID = "no robot id";   //eg RC01_Star
    string posX = "-1";
    string posY = "-1";

    Vector<Robot*> vecRobots;

    List<Robot*> listRobots;
    Queue<Robot*> waitingRobots;
    Queue<Robot*> destroyedRobots;

    int width, height, turns, numRobot;

public:
    //Vector<Robot*> vecRobots;

    Battlefield(int w=0, int h=0, string type="") : width(w), height(h), robotType(type) {}

    void setVecRobots(Vector<Robot*> vec)
    {
        vecRobots = vec;
    }
    Vector<Robot*>& getVecRobots()
    {
        return vecRobots;
    }

    void setWaitingRobots(Queue<Robot*> wait)
    {
        waitingRobots = wait;
    }
    Queue<Robot*>& getWaitingRobots()
    {
        return waitingRobots;
    }

    void setDestroyedRobots(Queue<Robot*> destroyed)
    {
        destroyedRobots = destroyed;
    }
    Queue<Robot*>& getDestroyedRobots()
    {
        return destroyedRobots;
    }

    void setListRobots(List<Robot*> List)
    {
        listRobots = List;
    }
    List<Robot*> getListRobots()
    {
        return listRobots;
    }

    void setNumRobot(int r)
    {
        numRobot = r;
    }
    int getNumRobot()
    {
        return numRobot;
    }

    void setTurns(int t)
    {
        turns = t;
    }
    int getTurns()
    {
        return turns;
    }

    void setWidth(int w)
    {
        width = w;
    }
    int getWidth() const
    {
        return width;
    }

    void setHeight(int h)
    {
        height = h;
    }
    int getHeight() const
    {
        return height;
    }

    void setRobotType(string type)
    {
        robotType = type;
    }
    string getRobotType()
    {
        return robotType;
    }

    // output
    void displayBattleField(ofstream &outputFile)
    {
        cout << "  ";  //space for the number of width
        outputFile << "  ";

        for(int w=0; w < width; w++)
        {
            if(w < 10)
            {
                 cout << "  0" << w << " " ;
                 outputFile << "  0" << w << " " ;
            }
            else
            {
                cout << "  " << w << " " ;
                outputFile << "  " << w << " " ;
            }
        }
        cout << endl;
        outputFile << endl;

        for(int h=0; h<height; h++)
        {
            cout << "  ";
            outputFile << "  ";
            for(int i=0; i < width; i++)
            {
                cout << "+----";
                outputFile << "+----";
            }
            cout << "+" << endl;
            outputFile << "+" << endl;

            if(h < 10)
            {
                cout << "0" << h ;
                outputFile << "0" << h ;
            }
            else
            {
                cout << h ;
                outputFile << h;
            }

            cout << "|";
            outputFile << "|";
            for(int col=0; col < width; col++)
            {
                bool robotPlaced = false;
                for ( auto robot : vecRobots )
                {
                    if (robot->x() == col && robot->y() == h) // changing col, h
                    {
                        cout << setw(4) << robot->id().substr(0, 4) << "|";
                        outputFile << setw(4) << robot->id().substr(0, 4) << "|";
                        robotPlaced = true;
                        break;
                    }
                }
                if ( !robotPlaced )
                {
                    cout << "    |";
                    outputFile << "    |";
                }
            }
            cout << endl;
            outputFile << endl;
        }

        cout << "  ";
        outputFile << "  ";
        for(int i=0; i < width; i++)
        {
            cout << "+----";
            outputFile << "+----";
        }
        cout << "+" << endl;
        outputFile << "+" << endl;
    }


    Robot* searchRobot(int x, int y)
    {
        for(auto robot : getVecRobots())
        {
            if(robot->x() == x && robot->y() == y)
            {
                return robot;
            }
        }

        return nullptr;
    }

    void eraseRobotById(string id, ofstream &outputFile)
    {
        for (int i=0; i<vecRobots.size(); ++i)
        {
            if (vecRobots[i]->id() == id)
            {
                vecRobots.erase(vecRobots.begin() + i);
            }
        }
    }

    void manageWaitingRobots(ofstream &outputFile)
    {
        if (!waitingRobots.empty())
        {
            Robot* robot = getWaitingRobots().front();

            int randX = rand() % width;
            int randY = rand() % height;
            robot->setX(randX);
            robot->setY(randY);

            if(waitingRobots.size() != 0)
            {
                cout << "Waiting list: " << endl;
                outputFile << "Waiting list: " << endl;

                for(int i = 0; i < waitingRobots.size(); i++)
                {
                    cout << *waitingRobots.get(i) << endl;
                    outputFile << *waitingRobots.get(i) << endl;
                }
                cout << endl;
                outputFile << endl;
            }

            cout << "Dequeueing robot ID " << *robot << endl;
            outputFile << "Dequeueing robot ID " << *robot << endl;

            getWaitingRobots().dequeue();
            getVecRobots().push_back(robot);
        }
    }


};
