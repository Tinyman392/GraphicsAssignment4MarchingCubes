#!/bin/bash

rm march test.txt
#rm test.txt
make -f makeMarch
#./march mc1.dat 1 > test.txt
#./cview 20 20 20 test.txt | ./rd_view -
./march mc1.dat 1 -n > test.txt
./cview 20 20 20 test.txt | ./rd_view -
#./march mc2.dat 1 > test.txt
#./cview 20 20 20 test.txt | ./rd_view -
#./march mc2.dat 1.33 -n > test.txt
#./cview 20 20 20 test.txt | ./rd_view -