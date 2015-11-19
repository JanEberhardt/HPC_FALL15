for x in 128 256 512
do
    for i in 1 6 12
    do
        ./openmp.s 1 1 $x 0.0001 $i
    done
done
