#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <map>
#include "CFS.cpp"
#include "RR.cpp"
// #define Infinity 999999

// int N;
// float simulationTime;
// int currentTime;

// using namespace std;

void callAlgo()
{
    int a;
    vector<Process> *f;

    cout << "1) Round Robin" << endl;
    cout << "2) Completely Fair Scheduler\n"
         << endl;

    cout << " Choose the Scheduling Algorithm (1/2): ";
    cin >> a;

    if (!(a == 1 || a == 2 || a == 3))
    {
        cout << "\n Error! invalid input" << endl;
        return;
    }

    Process_Creator obj;
    Scheduler sch(obj);
    Simulator sim(sch, a);
    sim.Run();

    f = sim.array;

    ofstream output_file("processes.txt");

    if (a == 1)
    {
        output_file << "\n\tRound Robin: \n\n";
    }
    else
    {
        output_file << "\tID\tAT\tBT\tCT\tTAT\tWT\tRT\t\tNICE\tVRT\tPT\n";

        for (int i = 0; i < f->size(); i++)
        {
            output_file << "\t" << f->at(i).processId << "\t"
                        << f->at(i).arrivalTime << "\t" << f->at(i).burstTime << "\t" << f->at(i).completionTime
                        << "\t" << f->at(i).turnAroundTime << "\t" << f->at(i).waitingTime << "\t" << f->at(i).responseTime
                        << "\t\t" << f->at(i).staticPriority - 120 << "\t\t" << f->at(i).virtualRunTime << "\t\t" << f->at(i).processType << "\n";
        }
    }

    if (a == 1)
    {
        output_file << "\tID\tAT\tBT\tCT\tTAT\tWT\tRT\n";

        for (int i = 0; i < f->size(); i++)
        {
            output_file << "\t" << f->at(i).processId << "\t"
                        << f->at(i).arrivalTime << "\t" << f->at(i).burstTime << "\t" << f->at(i).completionTime
                        << "\t" << f->at(i).turnAroundTime << "\t" << f->at(i).waitingTime << "\t" << f->at(i).responseTime << "\n";
        }
    }

    output_file.close();

    delete f;
    return;
}

int main()
{
    cout << "\n In, the arrival time and burst time " << endl;
    cout << " will be set in the range of (1-10) milli seconds " << endl;
    cout << "\n Enter the Number of Processes : ";
    cin >> N;

    cout << "\n Enter the Simulation Time (secs) : ";
    cin >> simulationTime;

    // Converting from seconds to milliseconds
    simulationTime *= 1000;

    callAlgo();

    return 0;
}
