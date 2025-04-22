// *********************************************************
// Program: TC3L_G37.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC3L
// Tutorial Class: T12L
// Trimester: 2410
// Member_1: 1091106071 | NURUL IZZATI AFINA BINTI SUKRI | 1091106071@student.mmu.edu.my | PHONE
// Member_2: 1211209226 | Wong Yi Thung | 1211209226@student.mmu.edu.my | 0135939974
// Member_3: 1221308310 | YASHA DURGASHINE | 1221308310@student.mmu.edu.my | PHONE
// *********************************************************
// Task Distribution
// Member_1:
// Member_2:
// Member_3:
// *********************************************************


#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
#include "Vector.h"
#include "List.h"
#include "Queue.h"
#include "Robot.h"
#include "Battlefield.h"
#include "SeeingRobot.h"
#include "MovingRobot.h"
#include "ShootingRobot.h"
#include "SteppingRobot.h"
#include "RoboCop.h"
#include "Terminator.h"
#include "TerminatorRoboCop.h"
#include "BlueThunder.h"
#include "MadBot.h"
#include "RoboTank.h"
#include "UltimateRobot.h"
#include "LazerBot.h"
#include "JumperBot.h"
#include "ShooterBot.h"
#include "function.h"
using namespace std;


int main()
{
    srand(1091106071);

    //Robot* robot = new UltimateRobot("UR01", 7, 7);
    ofstream output;
    output.open("fileOutput3.txt");

    Battlefield* bf = new Battlefield();
    read("fileInput3.txt", bf, output);  //read file

    cout << endl << endl;
    output << endl << endl;

    bf->displayBattleField(output);

    cout << endl << endl;
    output << endl << endl;

    runRobotActions(bf, output);

    delete bf;

    return 0;

}
