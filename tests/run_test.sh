cd ../cmake-build-debug
cmake ..
ninja

cd ../tests
./resonix_test > audio.csv
gnuplot -p -e "set datafile separator ','; plot 'audio.csv' using 2 with lines"