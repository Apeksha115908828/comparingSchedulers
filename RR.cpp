void Scheduler::rr(vector<Process> *runningQueue)
{
    if (!readyQueue->empty())
    {
        runningQueue->push_back(readyQueue->at(0));
        readyQueue->erase(readyQueue->begin());

        if (runningQueue->back().responseTime == Infinity)
        {
            runningQueue->back().setResponseTime(currentTime);
        }
    }
}

void Simulator::runRR()
{
    cout << "\n Hey! Round Robin Here\n"
         << endl;
    cout << " Enter the Time Quantum : ";
    cin >> timeQuantum;

    MinHeap heap(array);

    if (heap.getMin().arrivalTime != 0)
    {
        currentTime = heap.getMin().arrivalTime;
    }

    ofstream status_file("status.txt");
    status_file << "\n\tRound Robin: \n";
    status_file << "\n\tTime Stamp\tProcess Id\tStatus\n\n";

    while (currentTime < simulationTime)
    {
        while (!array->empty())
        {
            if (heap.getMin().arrivalTime <= currentTime)
            {
                readyQueue->push_back(heap.getMin());
                status_file << "\t" << readyQueue->back().arrivalTime
                            << "\t\t" << readyQueue->back().processId << "\t\t\tArrived"
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
                readyQueue->push_back(runningQueue->back());

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

        if (readyQueue->empty() && array->empty())
        {
            break;
        }

        sch->rr(runningQueue);

        status_file << "\t" << currentTime
                    << "\t\t" << runningQueue->back().processId << "\t\t\tRunning"
                    << "\n";

        if (!runningQueue->empty())
        {
            if (runningQueue->back().leftBurstTime >= timeQuantum)
            {
                currentTime += timeQuantum;
                runningQueue->back().leftBurstTime -= timeQuantum;
            }
            else if (runningQueue->back().leftBurstTime != 0 && runningQueue->back().leftBurstTime < timeQuantum)
            {
                currentTime += runningQueue->back().leftBurstTime;
                runningQueue->back().leftBurstTime = 0;
            }
            else
            {
                currentTime += 0;
            }

            runningQueue->back().setCompletionTime(currentTime);
        }
    }

    status_file.close();
}
