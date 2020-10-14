#include<bits/stdc++.h>
using namespace std;

double clk;
typedef pair<int,pair<pair<double,double>,double>> pa;
vector<int>instances;

class Task {
public:
	int id;
	double period;
	double execution;
	double startTime;
	double power;
};

vector<Task*> input;
double firstPoint = 0.0;
double lastPoint = 0.0;
double granularity = 1;

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

int notSchedulable = false;

double initialTemp,resistance = 22, capacitance = 0.0454, tamb = 25, tempTime = 0.0, tmax;
float conversionFactor = 1.9;  //For execution Time = (e)s, split is s, then we have decided that factor is s/e. 

double thermalModel(double t0, double timeInstant, double power) {
	double temperature;
	double r = exp(-timeInstant/(resistance*capacitance));
	temperature = t0 * r + (tamb + power*resistance) * (1-r);
	return temperature;
}

double calct0(double timeInstant, double power) {
	double temperature;
	double r = exp(-timeInstant/(resistance*capacitance));
	return (tmax - ((tamb + power*resistance) * (1-r)))/r;
}

double calctime(double t0, double power) {
	return log((t0 - (tamb + power*resistance))/(tmax - (tamb + power*resistance)))*resistance*capacitance;
}

pair<double,double> calculateIdleTime(double t0, double power, double exec) {
	double time = 0.0;
	if(t0 != tmax) {
		time = granularity;
		double tinstant = thermalModel(t0, exec-time, power);
		while(tinstant >= tmax) {
			time+=granularity;
			tinstant = thermalModel(t0, exec - time, power);
		}
		int m = ceil(conversionFactor * (time));
		cout<<"M : "<<m<<endl;
		return make_pair(time,m);
	}
	else {
		double m = conversionFactor * (exec);
		cout<<"M : "<<m<<endl;
		return make_pair(exec,m);
	}
}

double prev_clk = 0.0;

void edfSchedule() {
	while(!queueTask.empty()) {
		prev_clk = clk;
		cout<<"CLOCK : "<<clk<<endl;
		cout<<"Temperature : "<<initialTemp<<endl;
		cout<<"TOP OF QUEUE : "<<queueTask.top().first<<"(Period, Start, Exec) : " << queueTask.top().second.first.first << " "<< queueTask.top().second.first.second << " "<<queueTask.top().second.second<<endl;;
		if(!encounter.empty())
			cout<<"TOP OF ENC : "<<encounter.top().first << "(Period, Start, Exec) : " << encounter.top().second.first.first << " "<< encounter.top().second.first.second << " "<<encounter.top().second.second<<endl;
		for(int i=0;i<instances.size();i++) {
			cout<<"I:"<<i+1<<", Ins : "<<instances[i]<<" ";
		}
		cout<<endl;
		cout<<"<<<--BEGINS--->>:"<<endl;
		if(!encounter.empty()) {
			if(encounter.top().second.first.second == clk) {
					pa c = encounter.top();
					tempTime = thermalModel(initialTemp,input[c.first-1]->execution, input[c.first-1]->power);
					if(tempTime > tmax) {
						pair<double,double> rt = calculateIdleTime(initialTemp, input[c.first-1]->power, input[c.first-1]->execution);
						double idleTime = abs(calctime(calct0(rt.first/rt.second, (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ), (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ));
						cout<<"[----SPLIT TMAX REACHED-----][ENC]TASK : "<<c.first<<", TIME : "<<clk<<" "<<clk+idleTime<<endl;
						clk+=idleTime;
						//Temp intial after executing this task is going to be Tmax, so initialise the initialtemp
						initialTemp = tmax; 
					}
					if(initialTemp!=tmax) {
						initialTemp = tempTime;
					}
					cout<<"[ENC]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
					instances[c.first-1]+=1;
					encounter.pop();
					encounter.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, instances[c.first-1]*c.second.first.first) , c.second.second )) );
					if(clk + c.second.second > instances[c.first-1]*c.second.first.first) {
						cout<<"NOT SCHEDULABLE";
						notSchedulable = true;
						return;
					}
					clk+=c.second.second;
			}
			else {
				if(queueTask.top().second.second + clk <= encounter.top().second.first.second) {
					pa c = queueTask.top();
					int parts = 0;
					bool ex = false;
					pair<double,double> rt;
					tempTime = thermalModel(initialTemp, input[c.first-1]->execution, input[c.first-1]->power);
					if(tempTime > tmax) {
						rt = calculateIdleTime(initialTemp, input[c.first-1]->power, input[c.first-1]->execution);
						double idleTime = abs(calctime(calct0(rt.first/rt.second, (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ), (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ));
						if(clk + rt.second*(idleTime) + rt.first <= encounter.top().second.first.second) {
							cout<<"[----SPLIT TMAX REACHED-----][]TASK : "<<c.first<<", TIME : "<<clk <<" "<<clk + idleTime<<endl;
							clk+=idleTime;
							cout<<"[QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
							ex = true;
							instances[c.first-1]+=1;
							clk+=c.second.second;
						}
						else {
							while(clk + idleTime + rt.first/rt.second < encounter.top().second.first.second) {
								parts+=1;
								clk+=idleTime + rt.first/rt.second;
								cout<<"[----SPLIT TMAX REACHED-----][QUEUE]TASK : "<<c.first<<", TIME : "<<clk - idleTime - rt.first/rt.second <<" "<<clk<<endl;
								ex = true;
								initialTemp = tmax;
							}
							if(encounter.top().second.first.second - clk > 0) {
								cout<<"<------FREE TIME----->[TIME]"<<clk<<" "<<encounter.top().second.first.second<<endl;
								initialTemp = thermalModel(initialTemp, encounter.top().second.first.second - clk,0);
								clk = encounter.top().second.first.second;
							}
						}
					}
					else if(tempTime <= tmax) {
						cout<<"[QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
						clk+=c.second.second;
						ex = true;
						instances[c.first-1]+=1;
						initialTemp = tempTime;
					}
					if(parts == 0 && ex == true) {
						encounter.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, instances[c.first-1]*c.second.first.first ), input[c.first-1]->execution)) );
						queueTask.pop();
					}
					else if(parts > 0 && ex == true) {
						queueTask.pop();
						queueTask.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, 0.0),(c.second.second - parts*rt.first/rt.second))));
					}
					if(queueTask.empty()) {
						lastPoint = clk; 
					}

					if( clk + c.second.second > instances[c.first-1]*c.second.first.first) {
						cout<<"[Q]NOT SCHEDULABLE";
						notSchedulable = true;
						return;
					}
				}
				else {
					pa c = queueTask.top();
					double diff = encounter.top().second.first.second - clk;
					double parts;
					cout<<"Difference of execution is: "<<diff<<endl;
					tempTime = thermalModel(initialTemp ,diff, input[c.first-1]->power*diff/(input[c.first-1]->execution));
					if(tempTime > tmax) {
						pair<double,double> rt = calculateIdleTime(initialTemp, input[c.first-1]->power, input[c.first-1]->execution);
						double idleTime = abs(calctime(calct0(rt.first/rt.second, (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ), (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ));
						parts = 0;
						while(clk + idleTime + rt.first/rt.second < encounter.top().second.first.second) {
							parts+=1;
							clk+=idleTime + rt.first/rt.second;
							cout<<"[----SPLIT TMAX REACHED-----][PRE QUEUE]TASK : "<<c.first<<", TIME : "<<clk - idleTime - rt.first/rt.second <<" "<<clk<<endl;
							initialTemp = tmax;
						}
						
						if(encounter.top().second.first.second - clk > 0) {
							cout<<"<------FREE TIME----->[TIME]"<<clk<<" "<<encounter.top().second.first.second<<endl;
							initialTemp = thermalModel(initialTemp, encounter.top().second.first.second - clk,0);
							clk = encounter.top().second.first.second;
						}
						//Temp intial after executing this task is going to be Tmax, so initialise the initialtemp
						diff -= parts*(rt.first/rt.second);
					}
					if(tempTime <= tmax) {
						initialTemp = tempTime;
						cout<<"[PRE, QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<encounter.top().second.first.second<<endl;
						clk=encounter.top().second.first.second;
					}
					if(parts!=0) {
						queueTask.pop();
						queueTask.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, 0.0),(c.second.second - diff ))));
					}
				}
			}
		}
		else {
			pa c = queueTask.top();
			// Calculate the temp rise.
			tempTime = thermalModel(initialTemp, input[c.first-1]->execution, input[c.first-1]->power);
			cout<<"Temperature if : "<<tempTime<<endl;
			if(tempTime > tmax) {
				pair<double,double> rt = calculateIdleTime(initialTemp, input[c.first-1]->power, input[c.first-1]->execution);
				double idleTime = abs(calctime(calct0(rt.first/rt.second, (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ), (input[c.first-1]->power)*(rt.first/rt.second)/(input[c.first-1]->execution) ));
				cout<<"[----SPLIT TMAX REACHED-----][START]TASK : "<<c.first<<", TIME : "<<clk<<" "<<clk+idleTime<<endl;
				clk+=idleTime;
				//Temp intial after executing this task is going to be Tmax, so initialise the initialtemp
				initialTemp = tmax; 
			}
			if(initialTemp!=tmax) {
				initialTemp = tempTime;
			}
			instances[c.first-1]+=1;
			encounter.push(make_pair(c.first , make_pair(make_pair(c.second.first.first, instances[c.first-1]*c.second.first.first ) , c.second.second )) );
			firstPoint = c.second.second;
			cout<<"[QUEUE]TASK : "<<c.first<<", TIME : "<<clk<<" "<<(clk + c.second.second)<<endl;
			clk+=input[c.first-1]->execution;
			queueTask.pop();
		}
		if(prev_clk == clk) {
			notSchedulable = true;
			exit(0);
		}
		cout<<endl;
	}
}	

//<--granularity, 
int main(int argc, char *argv[]) {
	clk = 0.0;
	initialTemp = stod(argv[1], NULL);
	tmax = stod(argv[2],NULL);
	double util = 0.0;
	//Taking the input from the argument
	//<-- p1 e1 id1 p2 e2 id2....---> 
	if(argc > 7 && (argc-3)%4 == 0) {	
		for(int i=3;i<=argc-4;i=i+4) {
			Task* task = new Task;
			task->period = stod(argv[i+1],NULL);
			task->execution = stod(argv[i],NULL);
			util+=task->execution/task->period;
			task->id = stod(argv[i+2],NULL);
			task->power = stod(argv[i+3],NULL);
			task->startTime = 0;
			cout<<task->id<<endl;
			input.push_back(task);
		}
	}	
	else {
		cout<<argc<<endl;
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
	ofstream file;
	file.open("output.txt" , ios::app);
	file << "TMAX - InitialTemp : " << tmax - initialTemp;
	file << "      Utilization : " << util << "\n";
	edfSchedule();
	if(!notSchedulable)
		cout<<"Data Freshness for the [TASKSET] : "<<lastPoint - firstPoint<<endl;
	if(notSchedulable) {
		file << "NOT SCHED";
	}
	else {
		file << lastPoint - firstPoint;
	}
	file << " (";
	for(int i=0;i<argc;i++) {
		file << argv[i];
		file << " ";
	}
	file << "\n";
	file << "\n";
	file.close();
	return 0;
}
