#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

struct Process {
    int pid; 
    int bt;   
    int art; 
    int start_time;
    int completion_time; 
    int waiting_time; 
    int turnaround_time; 
};

void findFCFS(vector<Process>& procs) {
    int time = 0;
    for (auto& p : procs) {
        if (time < p.art)
            time = p.art;
        p.start_time = time;
        p.completion_time = p.start_time + p.bt;
        p.waiting_time = p.start_time - p.art;
        p.turnaround_time = p.completion_time - p.art;
        time = p.completion_time;
    }
}

void findSRTF(vector<Process>& procs) {
    int n = procs.size();
    vector<int> rt(n);
    for (int i = 0; i < n; i++) {
        rt[i] = procs[i].bt;
        procs[i].waiting_time = 0;
        procs[i].start_time = -1;
    }

    int complete = 0, t = 0, minm = INT_MAX;
    int shortest = 0, finish_time;
    bool check = false;

    while (complete != n) {
        for (int j = 0; j < n; j++) {
            if ((procs[j].art <= t) && (rt[j] < minm) && rt[j] > 0) {
                minm = rt[j];
                shortest = j;
                check = true;
            }
        }

        if (!check) {
            t++;
            continue;
        }

        if (procs[shortest].start_time == -1) procs[shortest].start_time = t;
        rt[shortest]--;
        minm = rt[shortest] ? rt[shortest] : INT_MAX;

        if (rt[shortest] == 0) {
            complete++;
            check = false;
            finish_time = t + 1;
            procs[shortest].completion_time = finish_time;
            procs[shortest].turnaround_time = finish_time - procs[shortest].art;
            procs[shortest].waiting_time = procs[shortest].turnaround_time - procs[shortest].bt;
        }
        t++;
    }
}

void findRR(vector<Process>& procs, int quantum) {
    int n = procs.size();
    queue<int> q;
    vector<int> rt(n);
    vector<bool> is_in_queue(n, false);

    for (int i = 0; i < n; i++) {
        rt[i] = procs[i].bt;
        procs[i].waiting_time = 0;
        procs[i].start_time = -1;
    }

    int t = 0;
    int current = 0;

    while (true) {
        bool done = true;
        for (int i = 0; i < n; i++) {
            if (rt[i] > 0) {
                done = false;
                if (procs[i].art <= t && !is_in_queue[i]) {
                    q.push(i);
                    is_in_queue[i] = true;
                }
            }
        }
        if (done)
            break;

        if (!q.empty()) {
            current = q.front();
            q.pop();
            is_in_queue[current] = false;
            int slice = min(rt[current], quantum);
            if (procs[current].start_time == -1) procs[current].start_time = t;
            t += slice;
            rt[current] -= slice;
            if (rt[current] == 0) {
                procs[current].completion_time = t;
                procs[current].turnaround_time = t - procs[current].art;
                procs[current].waiting_time = procs[current].turnaround_time - procs[current].bt;
            }
        } else {
            t++;
        }
    }
}

void printResults(vector<Process>& procs) {
    cout << " P\tBT\tWT\tTAT\n";
    float total_wt = 0, total_tat = 0;
    for (auto& p : procs) {
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
        cout << " " << p.pid << "\t"
             << p.bt << "\t"
             << p.waiting_time << "\t"
             << p.turnaround_time << endl;
    }
    cout << "\nAverage waiting time = "
         << fixed << setprecision(1) << total_wt / procs.size()
         << "\nAverage turn around time = "
         << fixed << setprecision(1) << total_tat / procs.size() << endl;
}

int main() {
    vector<Process> procs = {{1, 6, 0}, {2, 2, 3}, {3, 8, 1}, {4, 3, 4}, {5, 4, 6}};
    int quantum = 4;

    cout << "FCFS Scheduling:\n";
    findFCFS(procs);
    printResults(procs);

    cout << "\nSRTF Scheduling:\n";
    findSRTF(procs);
    printResults(procs);

    cout << "\nRound Robin Scheduling:\n";
    findRR(procs, quantum);
    printResults(procs);

    return 0;
}

 رابط الفيدو 
https://drive.google.com/file/d/10_pWvxLgRUfTNAPEOYUqNFJBFZinp863/view?usp=sharing