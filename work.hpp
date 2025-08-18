#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <getopt.h>
using namespace std;

enum class Type : int {
    Track,
    Road,
    Treadmill
};

struct Runner {
    int distance;

    int time;

    Type type;

    int load;

    bool jog = false;

    int scale = -1;

    double five = 0.0;

    int warmUp = 0;

    int coolDown = 0;

    int workoutMilage = 0;
};

class Program {
public:
    void get_options(int argc, char **argv);

    void read_data();

    void run();

    void print(int value);

private:
    bool isBasic = false;

    bool isAdvanced = false;

    bool isTest = false;

    bool isPace = false;

    bool policy = false;

    bool isDistance = false;

    Runner athlete;
};