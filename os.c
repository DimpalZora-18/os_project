#include <stdio.h>
#include <limits.h>

#define MAX 50

typedef struct {
    int id;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int completion;
    int turnaround;
    int waiting;
} Process;

void copy(Process dest[], Process src[], int n){
    for(int i=0;i<n;i++)
        dest[i]=src[i];
}

void calculate(Process p[], int n, float *avgWT, float *avgTAT){

    float totalWT=0,totalTAT=0;

    printf("\nID\tAT\tBT\tCT\tTAT\tWT\n");

    for(int i=0;i<n;i++){

        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;

        totalWT += p[i].waiting;
        totalTAT += p[i].turnaround;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
        p[i].id,
        p[i].arrival,
        p[i].burst,
        p[i].completion,
        p[i].turnaround,
        p[i].waiting);
    }

    *avgWT = totalWT/n;
    *avgTAT = totalTAT/n;

    printf("\nAverage Waiting Time: %.2f\n",*avgWT);
    printf("Average Turnaround Time: %.2f\n",*avgTAT);
}

void fcfs(Process p[], int n,float *avgWT,float *avgTAT){

    printf("\n===== FCFS =====\n");

    int time=0;

    printf("\nGantt Chart:\n| ");

    for(int i=0;i<n;i++){

        if(time < p[i].arrival)
            time = p[i].arrival;

        printf("P%d | ",p[i].id);

        time += p[i].burst;

        p[i].completion = time;
    }

    printf("\n");

    calculate(p,n,avgWT,avgTAT);
}

void sjf_non(Process p[], int n,float *avgWT,float *avgTAT){

    printf("\n===== SJF Non Preemptive =====\n");

    int completed=0,time=0;
    int visited[MAX]={0};

    printf("\nGantt Chart:\n| ");

    while(completed<n){

        int idx=-1;
        int min=INT_MAX;

        for(int i=0;i<n;i++){

            if(p[i].arrival<=time && !visited[i] && p[i].burst<min){
                min=p[i].burst;
                idx=i;
            }
        }

        if(idx!=-1){

            printf("P%d | ",p[idx].id);

            visited[idx]=1;

            time+=p[idx].burst;

            p[idx].completion=time;

            completed++;
        }
        else
            time++;
    }

    printf("\n");

    calculate(p,n,avgWT,avgTAT);
}

void sjf_pre(Process p[], int n,float *avgWT,float *avgTAT){

    printf("\n===== SJF Preemptive (SRTF) =====\n");

    int time=0,completed=0;

    for(int i=0;i<n;i++)
        p[i].remaining=p[i].burst;

    printf("\nGantt Chart:\n| ");

    while(completed<n){

        int idx=-1;
        int min=INT_MAX;

        for(int i=0;i<n;i++){

            if(p[i].arrival<=time && p[i].remaining>0 && p[i].remaining<min){
                min=p[i].remaining;
                idx=i;
            }
        }

        if(idx!=-1){

            printf("P%d | ",p[idx].id);

            p[idx].remaining--;

            time++;

            if(p[idx].remaining==0){
                p[idx].completion=time;
                completed++;
            }
        }
        else
            time++;
    }

    printf("\n");

    calculate(p,n,avgWT,avgTAT);
}

void roundRobin(Process p[], int n,int quantum,float *avgWT,float *avgTAT){

    printf("\n===== Round Robin =====\n");

    int time=0,completed=0;

    for(int i=0;i<n;i++)
        p[i].remaining=p[i].burst;

    printf("\nGantt Chart:\n| ");

    while(completed<n){

        int done=1;

        for(int i=0;i<n;i++){

            if(p[i].remaining>0 && p[i].arrival<=time){

                done=0;

                printf("P%d | ",p[i].id);

                if(p[i].remaining>quantum){

                    time+=quantum;
                    p[i].remaining-=quantum;
                }
                else{

                    time+=p[i].remaining;
                    p[i].remaining=0;
                    p[i].completion=time;
                    completed++;
                }
            }
        }

        if(done)
            time++;
    }

    printf("\n");

    calculate(p,n,avgWT,avgTAT);
}

void priority(Process p[], int n,float *avgWT,float *avgTAT){

    printf("\n===== Priority Scheduling =====\n");

    int completed=0,time=0;
    int visited[MAX]={0};

    printf("\nGantt Chart:\n| ");

    while(completed<n){

        int idx=-1;
        int high=INT_MAX;

        for(int i=0;i<n;i++){

            if(p[i].arrival<=time && !visited[i] && p[i].priority<high){
                high=p[i].priority;
                idx=i;
            }
        }

        if(idx!=-1){

            printf("P%d | ",p[idx].id);

            visited[idx]=1;

            time+=p[idx].burst;

            p[idx].completion=time;

            completed++;
        }
        else
            time++;
    }

    printf("\n");

    calculate(p,n,avgWT,avgTAT);
}

int main(){

    int n,quantum;

    Process original[MAX],temp[MAX];

    printf("Enter number of processes: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){

        original[i].id=i+1;

        printf("\nProcess %d\n",i+1);

        printf("Arrival Time: ");
        scanf("%d",&original[i].arrival);

        printf("Burst Time: ");
        scanf("%d",&original[i].burst);

        printf("Priority: ");
        scanf("%d",&original[i].priority);
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d",&quantum);

    float avgWT[5],avgTAT[5];

    copy(temp,original,n);
    fcfs(temp,n,&avgWT[0],&avgTAT[0]);

    copy(temp,original,n);
    sjf_non(temp,n,&avgWT[1],&avgTAT[1]);

    copy(temp,original,n);
    sjf_pre(temp,n,&avgWT[2],&avgTAT[2]);

    copy(temp,original,n);
    roundRobin(temp,n,quantum,&avgWT[3],&avgTAT[3]);

    copy(temp,original,n);
    priority(temp,n,&avgWT[4],&avgTAT[4]);

    printf("\n===== Algorithm Comparison =====\n");

    printf("\nAlgorithm\tAvg WT\tAvg TAT\n");

    printf("FCFS\t\t%.2f\t%.2f\n",avgWT[0],avgTAT[0]);
    printf("SJF Non\t\t%.2f\t%.2f\n",avgWT[1],avgTAT[1]);
    printf("SJF Pre\t\t%.2f\t%.2f\n",avgWT[2],avgTAT[2]);
    printf("Round Robin\t%.2f\t%.2f\n",avgWT[3],avgTAT[3]);
    printf("Priority\t%.2f\t%.2f\n",avgWT[4],avgTAT[4]);
    

    char *algo[5] = {
    "FCFS",
    "SJF Non-Preemptive",
    "SJF Preemptive",
    "Round Robin",
    "Priority"
};

int bestIndex = 0;

for(int i=1; i<5; i++) {
    if(avgWT[i] < avgWT[bestIndex]) {
        bestIndex = i;
    }
}

printf("\n Best Algorithm (Minimum Waiting Time): %s\n", algo[bestIndex]);
printf("Avg WT: %.2f\n", avgWT[bestIndex]);
printf("Avg TAT: %.2f\n", avgTAT[bestIndex]);
    return 0;
}