#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;

class ShootingRobot: virtual public Robot
{
protected:
    //data memeber

public:
    virtual ~ShootingRobot() { }

    virtual void actionFire(ofstream &outputFile) = 0;
};

