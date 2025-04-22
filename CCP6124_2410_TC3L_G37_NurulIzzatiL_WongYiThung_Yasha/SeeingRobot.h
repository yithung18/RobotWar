#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;

class SeeingRobot: virtual public Robot
{
protected:
    //data member

public:
    virtual ~SeeingRobot() { }
    virtual void actionLook(ofstream &outputFile) = 0;
};

