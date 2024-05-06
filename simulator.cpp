int N;
float simulationTime;
int currentTime;

#define Infinity 999999

using namespace std;

class Process
{
private:
    int processId;
    int processType;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int responseTime;
    int leftBurstTime;
    int virtualRunTime;
    int timeSlice;
    int staticPriority;

public:
    Process()
    {
        responseTime = Infinity;
    }

    ~Process()
    {
    }

    Process(int pid, int aTime, int bTime, int nice, int processType)
    {
        responseTime = Infinity;
        processId = pid;
        arrivalTime = aTime;
        burstTime = bTime;
        leftBurstTime = bTime;
        staticPriority = 120 + nice;
        virtualRunTime = nice + aTime + bTime + pid;
        processType = processType;
        cout<<"Process: processType = "<<processType<<endl;
        
        if (staticPriority < 110 && staticPriority >= 100)
        {
            timeSlice = 4;
        }
        else if (staticPriority < 120 && staticPriority >= 110)
        {
            timeSlice = 3;
        }
        else if (staticPriority < 130 && staticPriority >= 120)
        {
            timeSlice = 2;
        }
        else
        {
            timeSlice = 1;
        }
    }

    void setCompletionTime(int cTime)
    {
        completionTime = cTime;
    }

    void setTurnAroundTime()
    {
        turnAroundTime = completionTime - arrivalTime;
    }

    void setWaitingTime()
    {
        waitingTime = turnAroundTime - burstTime;
    }

    void setResponseTime(int resTime)
    {
        responseTime = resTime - arrivalTime;
    }

    void print()
    {
        cout << "\n Process Id : " << processId << endl;
        cout << " Nice : " << staticPriority - 120 << endl;
        cout << " Arrival Time : " << arrivalTime << " ms" << endl;
        cout << " Burst Time : " << burstTime << " ms" << endl;
        cout << " Completion Time : " << completionTime << " ms" << endl;
        cout << " Turn Around Time : " << turnAroundTime << " ms" << endl;
        cout << " Waiting Time : " << waitingTime << " ms" << endl;
        cout << " Response Time : " << responseTime << " ms" << endl;
        cout << " Process Type : " << processType << endl;
    }

    friend class Process_Creator;
    friend class Scheduler;
    friend class MinHeap;
    friend bool compare(const Process &p1, const Process &p2);
    friend class Simulator;
    void friend callAlgo();
};

class Process_Creator
{
private:
    vector<Process> *array = new vector<Process>();

public:
    Process_Creator()
    {
        array->resize(N);
        srand(time(0));

        for (int i = 0; i < N; i++)
        {
            random_device rd;
            mt19937 gen(rd());
            int processtype = i % 2;   // process type 0 = I/O bound, 1 = CPU bound
            cout<<"processtype: "<<processtype<<endl;
            int lbound, rbound;
            if(processtype == 0) {
                lbound = -20;
                rbound = -10;
            } else {
                lbound = -10;
                rbound = 19;
            }
            uniform_int_distribution<int> distArrTime(0, 9);
            uniform_int_distribution<int> distBurstTime(1, 9);
            uniform_int_distribution<int> distNice(lbound, rbound);

            int aTime = distArrTime(gen);
            int bTime = distBurstTime(gen);
            int nice = distNice(gen);

            array->at(i) = Process(i, aTime, bTime, nice, processtype);
        }
    }

    friend class Scheduler;
    friend class Simulator;
};

class MinHeap
{
private:
    vector<Process> *arr;
    int leftChild(int parent) { return 2 * parent + 1; }
    int rightChild(int parent) { return 2 * parent + 2; }
    int parent(int child) { return (child - 1) / 2; }

    void minHeapify(int parent)
    {
        int left = leftChild(parent);
        int right = rightChild(parent);
        int smallest = parent;

        if (left < arr->size() && arr->at(left).arrivalTime < arr->at(smallest).arrivalTime)
        {
            smallest = left;
        }
        else if (left < arr->size() && arr->at(left).arrivalTime == arr->at(smallest).arrivalTime)
        {
            if (arr->at(left).processId < arr->at(smallest).processId)
            {
                smallest = left;
            }
            else
            {
                smallest = parent;
            }
        }
        else
        {
            smallest = parent;
        }

        if (right < arr->size() && arr->at(right).arrivalTime < arr->at(smallest).arrivalTime)
        {
            smallest = right;
        }

        if (right < arr->size() && arr->at(right).arrivalTime == arr->at(smallest).arrivalTime)
        {
            if (arr->at(right).processId < arr->at(smallest).processId)
            {
                smallest = right;
            }
        }

        if (smallest != parent)
        {
            swap(arr->at(smallest), arr->at(parent));
            minHeapify(smallest);
        }
    }

public:
    MinHeap(vector<Process> *array)
    {
        arr = array;
        buildMinHeap();
    }

    void buildMinHeap()
    {
        for (int start = (arr->size() / 2) - 1; start >= 0; --start)
        {
            minHeapify(start);
        }
    }

    Process getMin() { return arr->at(0); }

    void pop()
    {
        arr->at(0) = arr->back();
        arr->pop_back();
        minHeapify(0);
    }
};

bool compare(const Process &p1, const Process &p2)
{
    if(p1.virtualRunTime != p2.virtualRunTime)
    {
        return p1.virtualRunTime < p2.virtualRunTime;
    }
    
    if(p1.staticPriority != p2.staticPriority)
    {
        return p1.staticPriority < p2.staticPriority;
    }

    return p1.arrivalTime < p2.arrivalTime;
    
}

class Scheduler
{
private:
    vector<Process> *readyQueue = new vector<Process>();
    vector<Process> *array;

public:
    Scheduler(Process_Creator &obj)
    {
        array = obj.array;
    }

    void fcfs(vector<Process> *runningQueue);
    void rr(vector<Process> *runningQueue);
    void cfs(Process p, vector<Process> *runningQueue);

    friend class RedBlackTree;
    friend class MinHeap;
    friend class Simulator;
};

void Scheduler::fcfs(vector<Process> *runningQueue)
{
    runningQueue->push_back(readyQueue->at(0));
    readyQueue->erase(readyQueue->begin());
}

class Simulator
{
private:
    vector<Process> *array;
    vector<Process> *runningQueue = new vector<Process>();
    vector<Process> *readyQueue;
    Scheduler *sch;
    int timeQuantum, choice;

    void CaptureValues()
    {
        for (int i = 0; i < array->size(); i++)
        {
            array->at(i).setTurnAroundTime();
            array->at(i).setWaitingTime();
        }
    }

public:
    Simulator(Scheduler &schObj, int a)
    {
        readyQueue = schObj.readyQueue;
        array = schObj.array;
        sch = &schObj;
        choice = a;
    }

    void printProcesses()
    {
        for (int i = 0; i < array->size(); i++)
        {
            array->at(i).print();
        }
    }

    void Run()
    {    
        
        if (choice == 1)
        {
            runRR();
        }
        else
        {
            runCFS();
        }

        for (int i = 0; i < runningQueue->size(); i++)
        {
            array->push_back(runningQueue->at(i));
        }

        delete readyQueue;
        delete runningQueue;

        CaptureValues();
        printProcesses();
        cout << "\n";
    }

    void runRR();
    void runCFS();

    void friend callAlgo();
};
