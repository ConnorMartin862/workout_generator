#include <algorithm>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <getopt.h>
#include "work.hpp"
using namespace std;

// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *command) {
    cout << "Usage: " << command << "-h\n";
    cout << "This program will read a list of coodinates or a map,\n";
    cout << "create a map, and simulate a search for specified treasure \n";
    cout << "given by $" << endl;
}  // printHelp()

// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void Program::get_options(int argc, char **argv) {
    int option_index = 0, option = 0;

    // Don't display getopt error messages about options
    opterr = false;

    // use getopt to find command line options
    struct option longOpts[] = {
        {  "help", no_argument, nullptr,  'h' },
        { "basic", no_argument, nullptr,  'b' },
        { "advanced", no_argument, nullptr, 'a' },
        { "test", no_argument, nullptr, 't' },
        { "pace", no_argument, nullptr, 'p' },
        { "distance", no_argument, nullptr, 'd'},
        { nullptr,           0, nullptr, '\0' },
    };  // longOpts[]
    // NOLINTEND

    bool opts_provided = false;
    // Fill in the double quotes, to match the mode and help options.
    while ((option = getopt_long(argc, argv, "hbatpd", longOpts, &option_index)) != -1) {
        opts_provided = true;
        switch (option) {
        case 'h':
            policy = true;
            printHelp(*argv);
            exit(0);

        case 'b':
            isBasic = true;
            policy = true;
            break;

        case 'a':
            isAdvanced = true;
            policy = true;
            break;

        case 't':
            isTest = true;
            policy = true;
            break;

        case 'p':
            isPace = true;
            policy = true;
            break;

        case 'd':
            isDistance = true;
            policy = true;
            break;

        }  // switch ..choice
    }  // while

    // After all the options have been processed,
    // check to make sure a sorting policy has been selected.
    // If one has not been selected, we will default to
    // sorting by song title (name).
    if (!policy && opts_provided) throw std::runtime_error("Unknown option");

}  // getOptions()

double generate(int distance) {
    double temp = 0;
    temp = pow(distance, 0.02636);
    temp *= 3.17852;
    temp -= 2.978921;
    return temp;    
}

int get_random() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 6);
    int rando = distrib(gen);
    return rando;
}

void Program::read_data() {
    string line;
    char junk;
    getline(cin, line);
    cin >> athlete.distance;
    cin >> athlete.time;
    athlete.time *= 60;
    cin >> junk;
    int temp;
    cin >> temp;
    athlete.time += temp;
    cin >> athlete.load;
    string a;
    cin >> a;
    if (a == "Track") {
        athlete.type = Type::Track;
    }
    else if (a == "Road") {
        athlete.type = Type::Road;
    }
    else if (a == "Treadmill") {
        athlete.type = Type::Treadmill;
    }
    else {
        throw std::runtime_error("Invalid Option");
    }

    if (isAdvanced) {
        string junk;
        cin >> junk;

        int z;
        cin >> z;
        athlete.scale = z;
        char y;
        cin >> y;
        if (y == 'I') {
            athlete.jog = true;
        }
        else if (y == 'D') {
            athlete.jog = false;
        }
        else {
            throw std::runtime_error("Invalid Option");
        }
    }
}

void Program::run() {

    double num = generate(athlete.distance);
    int a = athlete.time;
    int b = athlete.distance;
    int pace = static_cast<int>(floor(static_cast<double>(a) * 1600.0 / static_cast<double>(b)));
    athlete.five = static_cast<double>(pace) / num;

    if (isPace) {
        double milenum = generate(1600);
        double mile = static_cast<double>(athlete.five) * milenum;
        cout << "Projected Mile Pace/Time: " << (static_cast<int>(mile) / 60) << ":";
        if (static_cast<int>(mile) % 60 < 10) {
            cout << "0";
        }
        cout << (static_cast<int>(mile) % 60) << endl;

        cout << "Projected 5k Pace: " << (static_cast<int>(athlete.five) / 60) << ":";
        if (static_cast<int>(athlete.five) % 60 < 10) {
            cout << "0";
        }
        cout << (static_cast<int>(athlete.five) % 60) << endl;

        double eightnum = generate(8000);
        double eight = static_cast<double>(athlete.five) * eightnum;
        cout << "Projected 8K/Threshold Pace: " << (static_cast<int>(eight) / 60) << ":";
        if (static_cast<int>(eight) % 60 < 10) {
            cout << "0";
        }
        cout << (static_cast<int>(eight) % 60) << endl;

        double temponum = generate(16000);
        double tempo = static_cast<double>(athlete.five) * temponum;
        cout << "Projected Tempo Pace: " << (static_cast<int>(tempo) / 60) << ":";
        if (static_cast<int>(tempo) % 60 < 10) {
            cout << "0";
        }
        cout << (static_cast<int>(tempo) % 60) << endl;

        double maranum = generate(42195);
        double marathon = static_cast<double>(athlete.five) * maranum;
        cout << "Projected Marathon Pace: " << (static_cast<int>(marathon) / 60) << ":";
        if (static_cast<int>(marathon) % 60 < 10) {
            cout << "0";
        }
        cout << (static_cast<int>(marathon) % 60) << endl;
    }

    double totalMilage = athlete.load;

    if (totalMilage <= 25) {
        totalMilage = 5;
    }
    else {
        totalMilage = floor((0.17 * totalMilage) + .75);
    }
    int criticalMilage;

    if (athlete.distance <= 3200) {
        criticalMilage = 2;
    }
    else if (3200 < athlete.distance && athlete.distance < 10000) {
        criticalMilage = athlete.distance / 1600;
    }
    else {
        criticalMilage = 7;
    }

    int easyMilage = 0;
    int temp = static_cast<int>(totalMilage);
    temp -= 2;
    easyMilage += 2;
    athlete.workoutMilage += 2;
    temp -= 2;

    if (criticalMilage > 2) {
        athlete.workoutMilage++;
        temp--;
    }
    else {
        easyMilage++;
        temp--;
    }

    while (temp != 0 && athlete.workoutMilage < criticalMilage) {
        athlete.workoutMilage++;
        temp--;
        if (temp != 0) {
            easyMilage++;
            temp--;
        }
    }

    while (temp != 0) {
        athlete.workoutMilage++;
        temp--;
        if (temp != 0) {
            easyMilage++;
            temp--;
        }
        if (temp != 0) {
            easyMilage++;
            temp--;
        }
    }

    if (easyMilage % 2 == 1) {
        athlete.warmUp = easyMilage / 2 + 1;
    }
    else {
        athlete.warmUp = easyMilage / 2;
    }
    athlete.coolDown = easyMilage / 2;

    if (isDistance) {
        cout << "Total Milage: " << totalMilage << endl;
        cout << "Warm Up: " << athlete.warmUp << endl;
        cout << "Workout Miles: " << athlete.workoutMilage << endl;
        cout << "Cooldown: " << athlete.coolDown << endl;
    }

    if (isBasic) {
        int value = get_random();
        if (athlete.type == Type::Track) {
            if (value <= 2) {
                cout << "Mile Repeats" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                double threshnum = generate(8000);
                double thresh = static_cast<double>(athlete.five) * threshnum;
                if (athlete.workoutMilage > 5) {
                    cout << "Run " << (athlete.workoutMilage - 1) << " 1600 meter reps at ";
                } 
                else {
                    cout << "Run " << athlete.workoutMilage << " 1600 meter reps at ";
                }
                cout << (static_cast<int>(thresh) / 60) << ":";
                if (static_cast<int>(thresh) % 60 < 10) {
                    cout << "0";
                }
                cout << (static_cast<int>(thresh) % 60);
                if (athlete.workoutMilage > 5) {
                    cout << " with 400 jog between each rep." << endl;
                }
                else {
                    cout << " with 4:00 standing rest between each rep." << endl;
                }
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
            else if (value <= 3) {
                cout << "800 Repeats" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                cout << "Run " << (athlete.workoutMilage * 2) << " 800 meter reps at ";
                double val = athlete.five / 2;
                cout << (static_cast<int>(val) / 60) << ":";
                if (static_cast<int>(val) % 60 < 10) {
                    cout << "0";
                }
                cout << (static_cast<int>(val) % 60);
                cout << " with 3:00 standing rest between each rep." << endl;
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
            else if (value <= 4) {
                cout << "1K Repeats" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                cout << "Run " << floor(athlete.workoutMilage * 1.6) << " 1000 meter reps at ";
                double val = athlete.five * (0.625);
                cout << (static_cast<int>(val) / 60) << ":";
                if (static_cast<int>(val) % 60 < 10) {
                    cout << "0";
                }
                cout << (static_cast<int>(val) % 60);
                cout << " with 3:30 standing rest between each rep." << endl;
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
            else {
                double temponum = generate(16000);
                double tempo = static_cast<double>(athlete.five) * temponum;

                if (athlete.workoutMilage < 5) {
                    cout << "Track Tempo";
                    cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                    cout << " Run " << athlete.workoutMilage << " miles, or " << (athlete.workoutMilage * 1600) << " meters at ";
                    cout << (static_cast<int>(tempo) / 60) << ":";
                    if (static_cast<int>(tempo) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(tempo) % 60);
                    cout << " pace." << endl;
                    cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
                }
                else {
                    cout << "The Michigan" << endl;
                    cout << "Start with " << (athlete.warmUp - 1) << " easy miles." << endl;
                    cout << "Run 1600 meters at ";
                    double threshnum = generate(8000);
                    double thresh = static_cast<double>(athlete.five) * threshnum;
                    cout << (static_cast<int>(thresh) / 60) << ":";
                    if (static_cast<int>(thresh) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(thresh) % 60);
                    cout << "." << endl;
                    cout << "Run 2000 meters at ";
                    cout << (static_cast<int>(tempo) / 60) << ":";
                    if (static_cast<int>(tempo) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(tempo) % 60);
                    cout << " pace." << endl;
                    cout << "Run 1200 meters at ";
                    double fem = athlete.five * 0.75;
                    cout << (static_cast<int>(fem) / 60) << ":";
                    if (static_cast<int>(fem) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(fem) % 60);
                    cout << "." << endl;
                    cout << "Run 2000 meters at ";
                    cout << (static_cast<int>(tempo) / 60) << ":";
                    if (static_cast<int>(tempo) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(tempo) % 60);
                    cout << " pace." << endl;
                    cout << "Run 800 meters at ";
                    double twonum = generate(3200);
                    double two = static_cast<double>(athlete.five) * twonum;
                    two /= 2;
                    cout << (static_cast<int>(two) / 60) << ":";
                    if (static_cast<int>(two) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(two) % 60);
                    cout << "." << endl;
                    cout << "Run 2000 meters at ";
                    cout << (static_cast<int>(tempo) / 60) << ":";
                    if (static_cast<int>(tempo) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(tempo) % 60);
                    cout << " pace." << endl;
                    cout << "Run an ALL OUT 400" << endl;
                    cout << "Finish with " << (athlete.coolDown - 1) << " easy miles." << endl;
                }
            }
        }
        else if (athlete.type == Type::Road) {
            if (value < 4 || athlete.workoutMilage < 3) {
                bool doTempo = true;
                if (athlete.workoutMilage > 6) {
                    doTempo = false;
                }
                cout << "Road Tempo" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                cout << "Run " << athlete.workoutMilage << " miles at ";
                if (doTempo) {
                    double temponum = generate(16000);
                    double tempo = static_cast<double>(athlete.five) * temponum;
                    cout << (static_cast<int>(tempo) / 60) << ":";
                    if (static_cast<int>(tempo) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(tempo) % 60);
                }
                else {
                    double maranum = generate(42195);
                    double marathon = static_cast<double>(athlete.five) * maranum;
                    cout << (static_cast<int>(marathon) / 60) << ":";
                    if (static_cast<int>(marathon) % 60 < 10) {
                        cout << "0";
                    }
                    cout << (static_cast<int>(marathon) % 60);
                }
                cout << " pace." << endl;
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
            else {
                cout << "Road Fartlek" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                cout << "For " << athlete.workoutMilage << " miles, rotate between running 0.25 miles at ";
                double threshnum = generate(8000);
                double thresh = static_cast<double>(athlete.five) * threshnum;
                cout << (static_cast<int>(thresh) / 60) << ":";
                if (static_cast<int>(thresh) % 60 < 10) {
                    cout << "0";
                }
                cout << (static_cast<int>(thresh) % 60);
                cout << " and running 0.25 miles at an easier pace." << endl;
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
        }
        else {
            if (value < 4) {
                cout << "Treadmill Mile Repeats" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                double threshnum = generate(8000);
                double thresh = static_cast<double>(athlete.five) * threshnum;
                thresh = 3600 / thresh;
                thresh = round(thresh * 10.0) / 10.0;
                if (athlete.workoutMilage > 5) {
                    cout << "Run " << (athlete.workoutMilage - 1) << " 1 mile reps at ";
                } 
                else {
                    cout << "Run " << athlete.workoutMilage << " 1 mile reps at ";
                }
                cout << thresh << " mph";
                if (athlete.workoutMilage > 5) {
                    cout << " with 0.25 mile jog between each rep." << endl;
                }
                else {
                    cout << " with 4:00 standing rest between each rep." << endl;
                }
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
            else {
                cout << "Treadmill Tempo" << endl;
                cout << "Start with " << athlete.warmUp << " easy miles." << endl;
                cout << "Run " << athlete.workoutMilage << " miles at ";
                double temponum = generate(16000);
                double tempo = static_cast<double>(athlete.five) * temponum;
                tempo = 3600 / tempo;
                tempo = round(tempo * 10.0) / 10.0;
                cout << tempo << " mph." << endl;
                cout << "Finish with " << athlete.coolDown << " easy miles." << endl;
            }
        }
    }

    if (isAdvanced) {
        cout << "Under Construction" << endl;
    }

    if (isTest) {
        cout << athlete.distance << endl;
        cout << athlete.time << endl;
        cout << athlete.load << endl;
        if (athlete.type == Type::Track) {
            cout << "Track";
        }
        else if (athlete.type == Type::Road) {
            cout << "Road";
        }
        else {
            cout << "Treadmill";
        }
        cout << endl;
        
        if (isAdvanced) {
            cout << athlete.scale << endl;
            if (athlete.jog) {
                cout << "Jog";
            }
            else {
                cout << "Stand";
            }
            cout << endl;
        }
    }
}

int main(int argc, char **argv) {
    // This should be in all of your projects, speeds up I/O
    ios_base::sync_with_stdio(false);

    try {
        // Instantiate a music library.
        Program library;

        // Read and process the command line options.
        library.get_options(argc, argv);

        // Read in the provided file through stdin.
        library.read_data();

        library.run();

    }

    // Catch runtime_errors, print the message, and exit the
    // program with a non-zero status.
    catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // All done!
    return 0;
}