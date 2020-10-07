function [local_deadlines,min_util] = optimization(n,bcet,wcet,deadline,idle,m)
    
    num_local_deadlines = n-1;
    function [util] = utilization(d)
        util = 0.0;
        for k = 1:num_local_deadlines
            util = util + 2*wcet(k)/(d(k)+bcet(k)+idle(k)+bcet(k+1)/m(k+1));
        end
    end
    lb = zeros(1,num_local_deadlines);
    ub = deadline * ones(1,num_local_deadlines);
    A = [];
    b = [];
    Aeq = ones(1,num_local_deadlines);
    sum_wcet = 0.0;
    for i = 2:n-1
        sum_wcet = sum_wcet + wcet(i)/m(i);
    end
    beq = deadline - sum_wcet;
    num_partitions = sum(1:num_local_deadlines);
    partition_size = deadline / num_partitions / 2;
    e = zeros(1,num_local_deadlines);
    for j = 1:num_local_deadlines
       e(j) = partition_size * sum(1:j);
    end

    options = optimset('Display', 'off');
    [local_deadlines,min_util] = fmincon(@utilization,e,A,b,Aeq,beq,lb,ub,[],options);
end