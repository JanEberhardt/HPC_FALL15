rm *.dat
for n in 32 64 128
#note: on cluster run with these args:
#for n in 128 256 1024
do
    #single thread...
    ./single_threaded.s 1.0 1.0 $n 0.00001;

    ./mpi.s 1.0 1.0 $n 0.00001;

    echo "";
done


