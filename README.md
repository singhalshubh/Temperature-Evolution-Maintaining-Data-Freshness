# Thermal-Aware Data Freshness: Formulation and Evaluation

[![CodeFactor](https://www.codefactor.io/repository/github/singhalshubh/thermal-aware-data-freshness-formulation-and-evaluation/badge)](https://www.codefactor.io/repository/github/singhalshubh/thermal-aware-data-freshness-formulation-and-evaluation)

**This project has been done in collaboration with Dr. Deepak Gangadhar at International Institute of Information Technology Hyderabad in the capacity of Research Assistant.**

#### Overview of Project
In safety-critical systems, one can find several applications such as Adaptive Cruise Control (ACC) with task chains, whereby a task produces output data that is consumed by another task in the chain. Typically, there is a freshness requirement for the data consumed by a task for the data to be valid. Working with invalid data can put the system at serious risk. Similarly, the reliable operation of the applications on the processor requires the processor temperature to remain within the allowed peak temperature. There isn't any other work in literature that presents an analysis framework to derive periods of a task set considering both end-to-end data freshness and thermal constraints. In this paper, we attempt to fill this gap systematically by presenting a thermal-aware data freshness analysis framework considering a 2-task scenario and then extend it to an n-task scenario. We perform evaluations for schedulability based on the task periods derived using the thermal-aware data freshness formulation proposed in this work.<br>

### How to run the code?
- Description of files 
 + *it.cpp* is a C++ code, performing EDF scheduling of real-time task-chains (Adapted from MiBench) given the temperature constraints and data freshness bounds.
 + *optimization.m* in  MATLAB, requires the  **values  of idle  time  and _m_(splitting parts)** for  all  the  tasks  for  various  temperature ranges along with the **data freshness** for  every task  chain fixed  at  **30%  of  the  maximum  freshness  bound**,  as  **the  input**  where we have  considered  BCET  of  every  task  to  be  exactly  half of their WCET. The *output* are the *periods of the tasks*. 
 + *aut.sh* contains the sample task chains for the simulation.
 
 - Running the code
    + ./aut.sh will generate the sequence in which EDF scheduling occurs and the total freshness obtained.
    + You can modify the task chains, where format is <exec> <period> <task-id> <power> for a task in aut.sh.
 
 ### Results
 *Schedulability Percentage and Utilization at various temperature  ranges  for  different  freshness  bounds.  (a)80%,(b)50%, (c)30%, and (d)12%*
 <img src = "https://github.com/singhalshubh/Thermal-Aware-Data-Freshness-Formulation-and-Evaluation/blob/main/Results/freshness_decreasing.jpg">
 <br><br>
 *Schedulablilty with Different Chain Lengths*
 <br>
 <img src="https://github.com/singhalshubh/Thermal-Aware-Data-Freshness-Formulation-and-Evaluation/blob/main/Results/Schedulablilty%20with%20Different%20Chain%20Lengths.jpeg" width="50%">
 <br> <br>
 *Schedulablilty with Differing Chain Lengths undervarious temperature constraints*
 <br>
 <img src="https://github.com/singhalshubh/Thermal-Aware-Data-Freshness-Formulation-and-Evaluation/blob/main/Results/Schedulablilty%20%20with%20%20different%20%20freshness%20%20boundsunder%20various%20temperature%20constraints.png" width="50%">

<br>
 For any queries feel free to reach out to me at shubhpal@seas.upenn.edu. This research work is currently in progress(Focusing on comments from RTAS 2021).
 
