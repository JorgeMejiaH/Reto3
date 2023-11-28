echo "Thread test in progress..."
echo "Size;Process;Time" > DartBoardMPI.csv

for i in {1..10}; do
    echo "Running iteration $i"
    for t in 2 4 8 16 32; do
        for j in 100000 1000000 10000000 100000000 1000000000 10000000000; do
            result=$(mpirun -np $t -hostfile workers ./DartBoardMPI $j)
            echo "$j;$t;$result" >> DartBoardMPI.csv
        done
    done
done

echo "" >> 'DartBoardMPI.csv'
echo "done"