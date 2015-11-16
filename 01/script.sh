rm -f out.txt
for i  in 1 2 4 6 8 10 12 14 16 18 20 22 24
    do
    ./ex1 1000000000 $i >> out.txt
    done

