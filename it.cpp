#include<bits/stdc++.h>
using namespace std;

double clk;
double granularity;
typedef pair<int,pair<pair<double,double>,double>> pa;
vector<int>instances;

class Task {
public:
	int id;
	double period;
	double execution;
	double startTime;
};

vector<Task*> input;
double firstPoint = 0.0;
double lastPoint = 0.0;
class comp {
public:
  bool operator() (const pa &lhs, const pa &rhs) {
    if(lhs.second.first.second != rhs.second.first.second) {
    	return (lhs.second.first.second > rhs.second.first.second);
    }
    else return (lhs.second.first.first > rhs.second.first.first);
  }
};

class comp1 {
public:
  bool operator() (const pa &lhs, const pa &rhs) {
    if(lhs.second.first.second != rhs.second.first.second) {
    	return (lhs.second.first.second > rhs.second.first.second);
    }
    else return (lhs.second.first.first > rhs.second.first.first);
  }
};

priority_queue< pa, vector<pa>, comp >  queueTask;
priority_queue< pa, vector<pa>, comp1 >  encounter;

double intialTemp = 0.0,resistance, capacitance, tamb = 0.0;  

void edfSchedule() {
	while(!queueTask.empty()) {
		cout<<"CLOCK : "<<clk<<endl;
		cout<<"TOP OF QUEUE : "<<queueTask.top().first<<"(Period, Start, Exec) : " << queueTask.top().second.first.first << " "<< queueTask.top().second.first.second << " "<<queueTask.top().second.second<<endl;;
		if(!encounter.empty())
			cout<<"TOP OF ENC : "<<encounter.top().first << "(Period, Start, Exec) : " << encounter.top().second.first.first << " "<< encounter.top().second.first.second << " "<<encounter.top().second.second<<endl;
		for(int i=0;i<instances.size();i++) {
			cout<<"I:"<<i<<" Ins : "<<instances[i]<<" ";
		}
		cout<<endl;
		cout<<"<<<--BEGINS--->>:"<<endl;
		if(!encounter.empty()) {
			if(encounter.top().second.first.second == clk) {
					pa c = encounter.top();
					cout<<"[ENC]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
					instances[c.first-1]+=1;
					encounter.pop();
					encounter.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, instances[c.first-1]*c.second.first.first) , c.second.second )) );
					if(clk + c.second.second > instances[c.first-1]*c.second.first.first) {
						cout<<"NOT SCHEDULABLE";
						exit(0);
					}
					clk+=c.second.second;

			}
			else {
				if(queueTask.top().second.second + clk <= encounter.top().second.first.second) {
					pa c = queueTask.top();
					cout<<"[QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
					instances[c.first-1]+=1;
					encounter.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, instances[c.first-1]*c.second.first.first ), input[c.first-1]->execution)) );
					queueTask.pop();
					if(queueTask.empty()) {
						lastPoint = clk; 
					}

					if( clk + c.second.second > instances[c.first-1]*c.second.first.first) {
						cout<<"NOT SCHEDULABLE";
						exit(0);
					}
					clk+=c.second.second;
					
				}
				else {
					pa c = queueTask.top();
					cout<<"[PRE, QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<clk+encounter.top().second.second<<endl;
					clk+=encounter.top().second.second;
					queueTask.pop();
					queueTask.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, 0.0),(c.second.second - encounter.top().second.second) )));
				}
			}
		}
		else {
			pa c = queueTask.top();
			encounter.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, clk+ c.second.first.first ) , c.second.second )) );
			firstPoint = c.second.second;
			instances[c.first-1]+=1;
			cout<<"[QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
			clk+=input[c.first-1]->execution;
			
			queueTask.pop();
		}
		cout<<endl;
	}
}	

//<--granularity, 
int main(int argc, char *argv[]) {
	clk = 0.0;
	//Taking the input from the argument
	//<-- p1 e1 id1 p2 e2 id2....---> 
	if(argc >=2 && (argc-1)%3 == 0) {	
		for(int i=1;i<=argc-3;i=i+3) {
			Task* task = new Task;
			task->period = stod(argv[i+1],NULL);
			task->execution = stod(argv[i],NULL);
			task->id = stod(argv[i+2],NULL);
			task->startTime = 0;
			cout<<task->id<<endl;
			input.push_back(task);
		}
	}	
	else {
		perror("Argument input is wrong");
	}
	/*for(int i=0;i<input.size();i++) {
		cout<<input[i]->id<<endl;
	}*/
	for(int i=0;i<input.size();i++) {
		instances.push_back(0);
	}
	for(int i=0;i<input.size();i++) {
		if(input[i]->period <= input[i]->execution) {
			perror("Please look into the values. Execution cannot be more than period of a task");
			exit(0);
		}
		queueTask.push(make_pair(input[i]->id ,  make_pair(make_pair(input[i]->period, input[i]->startTime),input[i]->execution) ));
		cout<<queueTask.top().second.second<<endl;
	}
	edfSchedule();	
	cout<<"Data Freshness for the [TASKSET] : "<<lastPoint - firstPoint<<endl;
	
}
