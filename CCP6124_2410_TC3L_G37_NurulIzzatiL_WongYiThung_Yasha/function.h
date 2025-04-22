#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


void battlefield_actionLook(int robotPositionX, int robotPositionY, Battlefield* battlefield, ofstream &outputFile)
{
    for(int X = robotPositionX-1; X <= robotPositionX+1; X++)
    {
        cout << " " ;
        outputFile << " ";
        if(X < 10 && X >= 0)
        {
            cout << "  0" << X << " " ;
            outputFile << "  0" << X << " " ;
        }
        else
        {
            cout << "  " << X << " " ;
            outputFile << "  " << X << " " ;
        }
    }
    cout << endl;
    outputFile << endl;

    for(int Y=robotPositionY-1; Y<=robotPositionY+1; Y++)
    {
        cout << "  ";
        outputFile << " ";

        for(int X=robotPositionX-1; X<=robotPositionX+1; X++)
        {
            cout << "+----";
            outputFile << "+----";
        }
        cout << "+" << endl;
        outputFile << "+" << endl;

        if( Y >= 0 && Y < 10 )
        {
            cout << "0" << Y ;
            outputFile << "0" << Y ;
        }
        else
        {
            cout << Y;
            outputFile << Y;
        }

        cout << "|";
        outputFile << "|";
        for(int X = robotPositionX-1; X <= robotPositionX+1; X++)
        {
            bool robotPlaced = false;
            for (auto robot : battlefield->getVecRobots())
            {
                if ( robot->x() == X && robot->y() == Y )
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
    for(int i = robotPositionX-1; i <= robotPositionX+1; i++)
    {
        cout << "+----";
        outputFile << "+----";
    }
    cout << "+" << endl;
    outputFile << "+" << endl;

    cout << endl;
    outputFile << endl;
}


//read input file
void read(const string& filename, Battlefield* battlefield, ofstream &outputFile)
{
    int width, height, turns, numRobot;
    string robotType;   //RobotCop
    string robotID;   //eg RC01_Star
    string posX;
    string posY;
    string line;

    ifstream inputFile(filename);

    if (!inputFile)
    {
        cerr << "Unable to open file" << endl;
        outputFile << "Unable to open file" << endl;
        return;
    }

    while(getline(inputFile, line))
    {
        //cout << "Line: " << line << endl;
        stringstream ss(line);
        string token;

        getline(ss, token, ':');   //read line until ':'
        //cout << "Token: " << token << endl;

        if (token == "M by N")
        {
            ss >> width >> height; // Read width and height

        }
        else if (token == "turns")
        {
            ss >> turns; // Read the number of turns
        }
        else if (token == "robots")
        {
            ss >> numRobot; // Read the number of turns
        }
        else
        {
            // Process robot lines
            ss.clear();     // Clear the stringstream state
            ss.str(line);    // Reset the stringstream with the current line
            ss >> robotType >> robotID >> posX >> posY;


            //eg. to get the "RC01" ect.
            size_t underscore = robotID.find('_');
            if (underscore != string::npos)
            {
                robotID = robotID.substr(0, underscore);
            }

            cout << "Robot ID: " << robotID << " at (" << posX << ", " << posY << ")" << endl;
            outputFile << "Robot ID: " << robotID << " at (" << posX << ", " << posY << ")"<< endl;
            if(posX == "random")
            {
                posX = to_string(rand() % width);
                cout << robotID << " random X: " << posX << endl;
                outputFile << robotID << " random X: " << posX << endl;

                // random generate x and y
                // loop every robot to check no clash
            }

            if(posY == "random")
            {
                posY = to_string(rand() % height);
                cout << robotID << " random Y: " << posY << endl;
                outputFile << robotID << " random Y: " << posY << endl;
            }

            //store robots to the Vector vecRobots
            if(robotType == "RoboCop")
            {
                RoboCop* robot = new RoboCop( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "Terminator")
            {
                Terminator* robot = new Terminator( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "TerminatorRoboCop")
            {
                TerminatorRoboCop* robot = new TerminatorRoboCop( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "BlueThunder")
            {
                BlueThunder* robot = new BlueThunder( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "MadBot")
            {
                MadBot* robot = new MadBot( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "RoboTank")
            {
                RoboTank* robot = new RoboTank( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "UltimateRobot")
            {
                UltimateRobot* robot = new UltimateRobot( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "LazerBot")
            {
                LazerBot* robot = new LazerBot( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "JumperBot")
            {
                JumperBot* robot = new JumperBot( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else if(robotType == "ShooterBot")
            {
                ShooterBot* robot = new ShooterBot( robotID, stoi(posX), stoi(posY) );
                robot->setBattlefield(battlefield);
                battlefield->getVecRobots().push_back( robot );
            }
            else
            {
                cout << "Error message: Unknown robot type " << robotType << endl;
                outputFile << "Error message: Unknown robot type" << robotType << endl;
                exit(1);
            }
        }
    }

    battlefield->setWidth(width);
    battlefield->setHeight(height);
    battlefield->setNumRobot(numRobot);
    battlefield->setTurns(turns);
    battlefield->setRobotType(robotType);
}


void runRobotActions(Battlefield* bf, ofstream &outputFile)
{
    int turns = bf->getTurns();
    //cout << "numRobot: " << bf.vecRobots.size() << endl;

    Robot* robot ;

    for(int i = 0; i < turns; i++)
    {
        int numRobot = bf->getVecRobots().size();
        if(numRobot > 0)
        {
            int targetRobotIndex = i % numRobot;
            robot = bf->getVecRobots()[targetRobotIndex];
            //cout << "TargetRobot: " << targetRobotIndex << endl;

            cout << "Turn " << i+1 << ":" << endl;
            outputFile << "Turn " << i+1 << ":" << endl;

            cout << robot->id() << " at (" << robot->x() << ", " << robot->y() << ")" << endl;
            outputFile << robot->id() << " at (" << robot->x() << ", " << robot->y() << ")" << endl;

            cout << "Number of lives remaining: " << robot->numLives() << endl;
            outputFile << "Number of lives remaining: " << robot->numLives() << endl;

            cout << "Number of kills: " << robot->numOfKills() << endl;
            outputFile  << "Number of kills: " << robot->numOfKills() << endl;

            robot->actions(outputFile);
            cout << endl;

            bf->displayBattleField(outputFile);

            //dequeue robot from waiting queue
            bf->manageWaitingRobots(outputFile);

            if(bf->getVecRobots().size() == 1)
            {
                cout << "\nOnly one robot left. The game is ended." << endl;
                break;
            }

            cout << endl << endl;
            outputFile << endl << endl;
        }
        else
        {
            break;
        }
    }
}


void UpdateWaitingList(string id, int x, int y, int numOfLive, Battlefield* battlefield, ofstream &outputFile)
{
    int indexToRemove = -1;

    string prefix;
    for (char c : id)
    {
        if (std::isalpha(c))
        {
            prefix += c;
        }
        else
        {
            break;
        }
    }

    if(prefix == "RC")
    {
        RoboCop* robot = new RoboCop( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "T")
    {
        Terminator* robot = new Terminator( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x
                 && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }

    }
    else if(prefix == "TR")
    {
        TerminatorRoboCop* robot = new TerminatorRoboCop( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x
                 && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }

    else if(prefix == "BT")
    {
        BlueThunder* robot = new BlueThunder( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x
                 && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }

    else if(prefix == "MB")
    {
        MadBot* robot = new MadBot( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }

    }
    else if(prefix == "RT")
    {
        RoboTank* robot = new RoboTank( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }

    }
    else if(prefix == "UR")
    {
        UltimateRobot* robot = new UltimateRobot( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "LB")
    {
        LazerBot* robot = new LazerBot( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "JB")
    {
        JumperBot* robot = new JumperBot( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "SB")
    {
        ShooterBot* robot = new ShooterBot( id, x, y, numOfLive );
        robot->setBattlefield(battlefield);
        battlefield->getWaitingRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else
    {
        cout << "Error message: Unknown robot type in waiting list: " << battlefield->getRobotType() << endl;
        outputFile << "Error message: Unknown robot type in waiting list: " << battlefield->getRobotType() << endl;
        exit(1);
    }

    //cout << battlefield->getWaitingRobots().get(1) << endl;
}


void UpdateDestroyedList(string id, int x, int y, Battlefield* battlefield, ofstream &outputFile)
{
    int indexToRemove = -1;
    string prefix;
    for (char c : id)
    {
        if (std::isalpha(c))
        {
            prefix += c;
        }
        else
        {
            break;
        }
    }

    if(prefix == "RC")
    {
        RoboCop* robot = new RoboCop( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }

        //cout << battlefield->getWaitingRobots() << endl;
    }
    else if(prefix == "T")
    {
        Terminator* robot = new Terminator( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }

    else if(prefix == "TR")
    {
        TerminatorRoboCop* robot = new TerminatorRoboCop( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }

    else if(prefix == "BT")
    {
        BlueThunder* robot = new BlueThunder( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }

    else if(prefix == "MB")
    {
        MadBot* robot = new MadBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "RT")
    {
        RoboTank* robot = new RoboTank( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "UR")
    {
        //battlefield->destroyedRobots.enqueue( new UltimateRobot( id, x, y ));
        UltimateRobot* robot = new UltimateRobot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "LB")
    {
        LazerBot* robot = new LazerBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "JB")
    {
        JumperBot* robot = new JumperBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else if(prefix == "SB")
    {
        ShooterBot* robot = new ShooterBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getDestroyedRobots().enqueue( robot );

        for(int i = 0; i < battlefield->getVecRobots().size(); i++)
        {
            if ( battlefield->getVecRobots()[i]->x() == x && battlefield->getVecRobots()[i]->y() == y )
            {
                battlefield->getVecRobots().erase(battlefield->getVecRobots().begin() + i);
                break;
            }
        }
    }
    else
    {
        cout << "UNKNOWN ROBOT Type in destroyed list: " << battlefield->getRobotType() << endl;
        outputFile << "UNKNOWN ROBOT Type in destroyed list: " << battlefield->getRobotType() << endl;
        exit(1);
    }

}


void upgradeRobot(string id, int x, int y, Battlefield* battlefield, ofstream &outputFile)
{
    int indexToRemove = -1;
    string prefix;
    for (char c : id)
    {
        if (std::isalpha(c))
        {
            prefix += c;
        }
        else
        {
            break;
        }
    }

    if(prefix == "RC")
    {
        RoboCop* robot = new RoboCop( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "T")
    {
        Terminator* robot = new Terminator( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "TR")
    {
        TerminatorRoboCop* robot = new TerminatorRoboCop( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );

    }
    else if(prefix == "BT")
    {
        BlueThunder* robot = new BlueThunder( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "MB")
    {
        MadBot* robot = new MadBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "RT")
    {
        RoboTank* robot = new RoboTank( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "UR")
    {
        UltimateRobot* robot = new UltimateRobot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "LB")
    {
        LazerBot* robot = new LazerBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "JB")
    {
        JumperBot* robot = new JumperBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else if(prefix == "SB")
    {
        ShooterBot* robot = new ShooterBot( id, x, y );
        robot->setBattlefield(battlefield);
        battlefield->getVecRobots().push_back( robot );
    }
    else
    {
        cout << "Error message: Unknown robot type during upgrading robot: " << battlefield->getRobotType() << endl;
        outputFile << "Error message: Unknown robot type during upgrading robot: " << battlefield->getRobotType() << endl;
        exit(1);
    }
}


