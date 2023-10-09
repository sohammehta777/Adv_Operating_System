#include<stdio.h>
#include<stdlib.h>

#define NUM 20
typedef struct {
	int name;
	int burstTime;
	int arrivalTime;
	int waitTime;
}process;

typedef struct {
	process list[NUM];
	int size;
	int front;
	int rear;
}queue;

void initQueue(queue *pq) {
	pq->size=0;
	pq->front=0;
	pq->rear=-1;	
}

void enQueue(queue *pq, process *p) {
	pq->size++;
	pq->rear=(pq->rear+1)%NUM;
	pq->list[pq->rear]=*p;		
}

void deQueue(queue *pq, int time, int ct[]) {
    ct[pq->list[pq->front].name] = time;
	pq->size--;
	pq->front=(pq->front+1)%NUM;	
}

void sortQueue(queue *pq){
	int i,j;
	process temp=pq->list[pq->front];
	for(i=pq->front; i<pq->rear; i++) {
		pq->list[i]=pq->list[i+1];
	}
	pq->list[pq->rear]=temp;	
}

void sortListByAT(process list[]){
	int i,j;
	for(i=0; i<NUM; i++) {
		for(j=i+1; j<NUM; j++) {
			if(list[i].arrivalTime > list[j].arrivalTime){
				process temp=list[i];
				list[i]=list[j];
				list[j]=temp;
			}
		}
	}
}

void checkProcessQueue(queue *pq, process list[], int time){
	int i;
	for(i=0; i<NUM; i++){
		if(time==list[i].arrivalTime) {
			enQueue(pq, &list[i]);
		}
	} 
}

void run(process *p){
	printf("%d ",p->name);
	p->burstTime--;	
}

void wait(process *p){
	p->waitTime++;
}

int main(){
	srand(200);
	int i,j,time, timeQuantum;
	process list[NUM];
	queue queue;
	int totalWaitTime=0;	
	int totalBurstTime=0;
	int timeProcessed=0;
    int bt[NUM], tat = 0;
    int lower = 0, upper = NUM;
    int completionTime[NUM];
	float max=0.0, min=0.0;
	printf("***Round Robin NP - preemptive scheduling***\n");
	for(i=0; i<NUM; i++){	
		list[i].burstTime = (rand() % 10) + 1;
		bt[i] = list[i].burstTime;
		list[i].arrivalTime = rand()% 100;
		list[i].name=(i);
		list[i].waitTime=0;
		totalBurstTime+=list[i].burstTime;
	}
  
	sortListByAT(list);
	initQueue(&queue);
	for(time=0; time<totalBurstTime; time++) {
		timeProcessed++;
		checkProcessQueue(&queue,list,time);
		
		if(queue.size!=0)
			run(&queue.list[queue.front]);
		else 
			printf("CPU Idle\n");
				
		for(i=queue.front+1; i<=queue.rear; i++){
			wait(&queue.list[i]);
		}
		if(queue.list[queue.front].burstTime==0){
			totalWaitTime += queue.list[queue.front].waitTime;
			deQueue(&queue, timeProcessed, completionTime);
			timeProcessed=0;
		}
		
		else if(time != 0 && ((timeProcessed) % timeQuantum)==0){
			sortQueue(&queue);		
			timeProcessed=0;
		}			
	}	
    for(i=0; i<NUM; i++){
	    tat += list[i].waitTime + bt[i];
    }

    for(int l=0; l<NUM; l++){
        if(min > list[l].arrivalTime)
            min=list[l].arrivalTime;
    }

    for(int ll=0; ll<NUM; ll++){
        if(max < completionTime[ll])
            max=completionTime[ll];
    }

	printf("\nRound Robin NP Algorithm:\n");
    printf("\nProcess ID \t Arrival time \t Burst time\n");
    for(i=0; i<NUM; i++){
        printf("%d \t\t\t %d \t\t\t  %d \n", list[i].name, list[i].arrivalTime, list[i].burstTime);
    }

    float throughPut = NUM/(max-min);
    printf("\nAverage wait time - %f\n", (float)(totalWaitTime)/NUM);
    printf("\nAverage turn around time - %f\n", (float)(tat)/NUM);
    printf("\nAverage response time - %f\n", (float)(totalWaitTime)/NUM);
    printf("Throughput - %f\n", throughPut);
    return 0;
}
