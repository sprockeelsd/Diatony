# compile the project and run the unit tests
echo "compiling and running tests..."
mvn clean package -q
echo "compilation done and tests performed"
# path of the executable
launch_solver="java --enable-preview -cp target/maxicp-0.0.1-pcjsocmsr.jar:target/maxicp-0.0.1-jar-with-dependencies.jar org.maxicp.cp.examples.DARPInsertion"
currentDate=$(date +%Y-%m-%d_%H-%M-%S);  #
outFileOpt="results/darp/darp-opt-${currentDate}"  # filename of the results (with the date at the end of the file)
declare -a relaxations=("shaw" "random")  # each relaxation to try
mkdir -p "results/darp"  # where the results will be written
rm -f $outFileOpt  # delete filename of the results if it already existed (does not delete past results, unless their datetime is the same)
# the solver must print only one line when it is finished, otherwise we won't get a CSV at the end
# this is the header of the csv. This header needs to change depending on the solver / type of experiment that is being run
# all rows need to be printed by the solver itself
# the id of the commit can be retrieved with the command `git rev-parse HEAD`
echo "instance | n_vehicles | n_requests | best | method | max_runtime | solutions_over_time | n_nodes | n_failures | n_sols | is_completed | relaxation | args | commitId " >> $outFileOpt
timeout=900  # timeout in seconds
iter=10   # number of iterations, for lns only
echo "writing inputs"
# write all the configs into a temporary file
inputFile="inputFileDARPOpt"
rm -f $inputFile  # delete previous temporary file if it existed
for (( i=1; i<=$iter; i++ ))  # for each iteration
do
  for rel in "${relaxations[@]}"  # for each relaxation to perform
  do
    # extracts the instances from the data folder
    # write one line per instance containing its filename, along with the relaxation to perform
    find data/DARP/Cordeau2003/ -type f | sed "s/$/,$rel/"  >> $inputFile
  done
done
# add a comma and a random seed at the end of each line
awk -v seed=$RANDOM '{
    srand(seed + NR); # seed the random number generator with a combination of $RANDOM and the current record number
    rand_num = int(rand() * 10000); # generate a random number. Adjust the multiplier for the desired range.
    print $0 "," rand_num;
}' $inputFile | sponge $inputFile
# at this point, the input file contains rows in the format
# instance_filename,relaxation,seed
echo "launching experiments in parallel"
# the command line arguments for launching the solver. In this case, the solver is run using
# ./executable -f instance_filename -r relaxation -t timeout -s seed -m mode_for_optimisation -v verbosity
# change this depending on your solver
# the number ({1}, {2}) corresponds to the rows present in the inputFile, beginning at index 1 (i.e. in this case 3 columns, so 1, 2 and 3 are valid columns)
cat $inputFile | parallel --colsep ',' $launch_solver -f {1} -r {2} -t $timeout -s {3} -m opt -v 0 >> $outFileOpt
# delete the temporary file
rm -f $inputFile
