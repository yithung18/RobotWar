#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cctype>
#include <string>
#include <sstream>
using namespace std;


class SteppingRobot: virtual public Robot
{
protected:
    //data memeber

public:
    virtual ~SteppingRobot() { }

    virtual void actionStep(ofstream &outputFile) = 0;
};

