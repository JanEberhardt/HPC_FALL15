rm *.dat
for n in 32 64 128
#note: on cluster run with these args:
#for n in 128 256 1024
do
    #single thread...
    ./single_threaded 1.0 1.0 $n 0.00001;

    #multi-threaded
    for i in 2 4 8 16
    do
        ./multi_threaded 1.0 1.0 $n 0.00001 $i;
        ./barriered 1.0 1.0 $n 0.00001 $i;
    done
done


