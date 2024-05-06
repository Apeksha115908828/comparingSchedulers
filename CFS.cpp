#include "simulator.cpp"
void Scheduler::cfs(Process Q, vector<Process> *runningQueue)
{
    runningQueue->push_back(Q);

    if (runningQueue->back().responseTime == Infinity)
    {
        runningQueue->back().setResponseTime(currentTime);
    }
}


void Simulator::runCFS()
{
    cout << "\n Hey! Completely Fair Scheduler Here\n"<< endl;

    MinHeap heap(array);
    map<Process, bool, decltype(&compare)> redBlackTree(&compare);
    auto it = redBlackTree.begin();
    ofstream status_file("status.txt");
    // status_file << "\n\tCompletely Fair Scheduler: \n";
    status_file << "\tTime Stamp\tProcess Id\tStatus\n";
    if (heap.getMin().arrivalTime != 0)
    {
        currentTime = heap.getMin().arrivalTime;
    }
    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            
            if (heap.getMin().arrivalTime <= currentTime)
            {
                redBlackTree[heap.getMin()] = true;
                status_file << "\t" << heap.getMin().arrivalTime
                            << "\t\t" << heap.getMin().processId << "\t\t\tArrived"
                            << "\n";
                heap.pop();
            }
            else
            {
                break;
            }
        }
        
        if (!runningQueue->empty())
        {   
            if (runningQueue->back().leftBurstTime != 0)
            {
                redBlackTree[runningQueue->back()] = true;
                status_file << "\t" << currentTime
                            << "\t\t" << runningQueue->back().processId << "\t\t\tArrived"
                            << "\n";
                runningQueue->erase(runningQueue->end() - 1);
            }
            else
            {
                status_file << "\t" << currentTime
                            << "\t\t" << runningQueue->back().processId << "\t\t\tExit"
                            << "\n";
            }
        }
        if (redBlackTree.empty() && array->empty())
        {
            break;
        }
        it = redBlackTree.begin();
        Process Q = it->first;

        sch->cfs(Q, runningQueue);

        redBlackTree.erase(it);

        status_file << "\t" << currentTime
                    << "\t\t" << runningQueue->back().processId << "\t\t\tRunning"
                    << "\n";

        if (!runningQueue->empty())
        {
            if (runningQueue->back().leftBurstTime >= runningQueue->back().timeSlice)
            {
                currentTime += runningQueue->back().timeSlice;
                runningQueue->back().leftBurstTime -= runningQueue->back().timeSlice;
            }
            else if (runningQueue->back().leftBurstTime != 0 && runningQueue->back().leftBurstTime < runningQueue->back().timeSlice)
            {
                currentTime += runningQueue->back().leftBurstTime;
                runningQueue->back().leftBurstTime = 0;
            }
            else
            {
                currentTime += 0;
            }
        }

        int cpuTime = runningQueue->back().burstTime - runningQueue->back().leftBurstTime;
        runningQueue->back().virtualRunTime += (cpuTime * runningQueue->back().staticPriority);

        runningQueue->back().setCompletionTime(currentTime);
    }

    status_file.close();
}