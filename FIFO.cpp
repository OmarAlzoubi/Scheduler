#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process
{
  string name;
  int procTime;
  int arrivalTime;
  int endTime;
  int turnAroundTime;
  int delay;
  int response;

  Process(string name, int arrivalTime, int procTime)
  {
    this->name=name;
    this->arrivalTime=arrivalTime;
    this->procTime=procTime;
  }
  string print()
  {
    return name + ": (response=" + to_string(response) + ",turnaround=" + to_string(turnAroundTime) + ",delay=" + to_string(delay) + ")";
  }
};

int compare(Process proc1, Process proc2)
{
  return proc1.arrivalTime < proc2.arrivalTime;
}

void read_from_file(vector<Process> &processes)
{
  // Create and open a text file
  ifstream inputFile("in.txt", ios::in);
  if(!inputFile)
  {
    cout<<"Couldn't open the file\n";
  }
  else
  {
    string line;
    int procNumber = 0;
    //Skip first line
    getline(inputFile, line);

    string name;
    int arrivalTime, procTime;

    //Loop until end of file
    while (inputFile>>name>>arrivalTime>>procTime)
    {
      processes.push_back(Process(name,arrivalTime,procTime));
    }

  }

  inputFile.close();
}

string sorting_queuing(vector<Process> &processes)
{
  string processesOrder = "";
  sort(processes.begin(), processes.end(), compare);

  for (int i = 0; i < processes.size(); i++)
  {
    processesOrder += processes[i].name;

    //If this is the first process
    if (i == 0)
    {
      processes[i].endTime = processes[i].procTime+processes[i].arrivalTime;      
      processes[i].delay = 0;
      processes[i].response = 0;
      processes[i].turnAroundTime = processes[i].procTime;
    }

    else
    {
      //No OverLab
      if(processes[i-1].endTime<=processes[i].arrivalTime)
      {
        processes[i].endTime = processes[i].arrivalTime+processes[i].procTime;
        processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;
        processes[i].delay = processes[i].turnAroundTime - processes[i].procTime;
        processes[i].response = processes[i].delay;        
      }

      //OverLab
      else
      {
        processes[i].endTime = processes[i-1].endTime+processes[i].procTime;
        processes[i].turnAroundTime = processes[i].endTime - processes[i].arrivalTime;
        processes[i].delay = processes[i].turnAroundTime - processes[i].procTime;
        processes[i].response = processes[i].delay;
      }

    }
  }

  return processesOrder;
}

void output_to_file(vector<Process> processes,string processesOrder)
{
  ofstream outputFile("out.txt", ios::out);
  outputFile << processesOrder << endl;

  for (int i = 0; i < processes.size(); i++)
  {
    string line = processes[i].print();
    outputFile << line << endl;
  }
  // Close the file
  outputFile.close();
}

int main()
{
  vector<Process> processes;
  read_from_file(processes);
  string processesOrder = sorting_queuing(processes);
  output_to_file(processes,processesOrder);
}