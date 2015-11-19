for i in 4 16 64 256 1024 4000 20000 200000
do
    ./single_threaded.s 0.3 0.4 0.01 $i
    ./multi_threaded.s 0.3 0.4 0.01 $i
done
