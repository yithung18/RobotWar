#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


class MovingRobot: virtual public Robot
{
protected:
    //data memeber

public:
    virtual ~MovingRobot() { }
    virtual void actionMove(ofstream &outputFile) = 0;
};

