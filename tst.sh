# Reset tuning log
> tuning_log.txt

# Store original file content
cp src/quantum/bitspace/qfh.cpp src/quantum/bitspace/qfh.cpp.bak

for k1 in 0.2 0.3 0.4 0.5; do
  for k2 in 0.1 0.2 0.3 0.4; do
    # Use a single sed command with multiple expressions to update the file
    sed -e "s/const double k1 = .*/const double k1 = $k1;/" -e "s/const double k2 = .*/const double k2 = $k2;/" src/quantum/bitspace/qfh.cpp.bak > src/quantum/bitspace/qfh.cpp

    # Build and run the test
    ./build.sh &> /dev/null
    result=$(./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json 2>&1 | grep "Overall Accuracy" | awk '{print $3}')
    
    # Log the results
    echo "k1=$k1 k2=$k2 accuracy=$result" >> tuning_log.txt
  done
done

# Restore original file
mv src/quantum/bitspace/qfh.cpp.bak src/quantum/bitspace/qfh.cpp