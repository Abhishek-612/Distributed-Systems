#**********************
#*
#* Progam Name: MP1. Membership Protocol.
#*
#* Current file: run.sh
#* About this file: Submission shell script.
#* 
#***********************
#!/bin/sh
rm -f dbg.*.log

make clean > /dev/null
make > /dev/null
./Application testcases/singlefailure.conf > /dev/null
cp dbg.log dbg.0.log
./Application testcases/multifailure.conf > /dev/null
cp dbg.log dbg.1.log
./Application testcases/msgdropsinglefailure.conf > /dev/null
cp dbg.log dbg.2.log

