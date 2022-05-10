cd root/
ls
gcc -Wall test_mm.c 
exit
ls
gcc -Wall test_mm.c 
gcc -Wall test_mm.c 
valgrind ./a.out 
ls
find . -name "*.c" | while read line; do sed -i '1s/^\(.*\)$/\/\/ This is a personal academic project. Dear PVS-Studio, please check it.\n\1/' "$line"; done
find . -name "*.c" | while read line; do sed -i '2s/^\(.*\)$/\/\/ PVS-Studio Static Code Analyzer for C, C++ and C#: http:\/\/www.viva64.com\n\1/' "$line"; done
rm a.out 
make
rm a.out 
apt-get update
apt-get install pvs-studio
pvs-studio-analyzer trace -- make
#todo menos MISRA
#pvs-studio-analyzer analyze -a 31
pvs-studio-analyzer analyze
#pvs-studio-analyzer analyze
#plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3;CS:1,2,3;MISRA:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
cat report.tasks 
make
./a.out 
valgrind ./a.out 
make
make
make
make
./a.out 
make
./a.out 
man memset
valgrind ./a.out 
make
valgrind ./a.out 
make
make
make
make
./a.out 
valgrind ./a.out 
make
make
make
./a.out 
make
./a.out 
valgrind ./a.out 
make
valgrind ./a.out 
make
valgrind ./a.out 
make
pvs-studio-analyzer trace -- make
#todo menos MISRA
#pvs-studio-analyzer analyze -a 31
pvs-studio-analyzer analyze
#pvs-studio-analyzer analyze
#plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3;CS:1,2,3;MISRA:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
pvs-studio-analyzer analyze -a 31
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3;CS:1,2,3;MISRA:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
cat report.tasks 
ls
rm a.out 
make
make
make
make
make
make clean
make
ls
rm a.out 
make
make
make
ls
./test_mm 
valgrind ./test_mm 
strace  ./test_mm 
ltrace ./test_mm 
make clean
pvs-studio-analyzer trace -- make
#todo menos MISRA
#pvs-studio-analyzer analyze -a 31
pvs-studio-analyzer analyze
#pvs-studio-analyzer analyze
#plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3;CS:1,2,3;MISRA:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
pvs-studio-analyzer analyze -a 31
plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3;CS:1,2,3;MISRA:1,2,3' -t tasklist -o report.tasks PVS-Studio.log
cat report.tasks 
make
make
rm report.tasks strace_out PVS-Studio.log 
ls
rm test_mm
ls
make
make
make clean
make clean
make
make
make
make
make clean
ls
ls
cd ..
ls
cd root/
ls
cd ..
ls
exit
cd root
ls
rm a.out 
gcc test_processes.c test_util.c -Wall
valgrind ./a.out 
exit
cd roo
ls
make
make clean
make
valgrind ./test_mm
make clean
make clean
make
./test_mm 
echo $?
cat Makefile
gcc -Wall -Wextra test_processes.c test_util.c
gcc -Wall -Wextra test_processes.c test_util.c
clear
gcc -Wall -Wextra test_processes.c test_util.c
gcc -Wall -Wextra test_processes.c test_util.c
gcc -Wall -Wextra test_processes.c test_util.c
gcc -Wall -Wextra test_mm.c test_util.c
clear
gcc -Wall -Wextra test_mm.c test_util.c
make
make
gcc -Wall -Wextra test_mm.c test_util.c
gcc -Wall -Wextra test_processes.c test_util.c
gcc -Wall -Wextra test_processes.c test_util.c
./a.out 
valgrind ./a.out 
gcc -Wall -Wextra test_prio.c test_util.c
gcc -Wall -Wextra test_prio.c test_util.c
./a.out 
gcc -Wall -Wextra -C test_mm.c test_util.c syscall.c
gcc -Wall -Wextra -C test_mm.c test_util.c syscall.c
clear
gcc -Wall -Wextra -C test_mm.c test_util.c syscall.c
clear
gcc -Wall -Wextra -C test_mm.c test_util.c syscall.c
gcc -Wall -Wextra -Wno-unused-parameter -C test_mm.c test_util.c syscall.c
clear
gcc -Wall -Wextra -Wno-unused-parameter -C test_mm.c test_util.c syscall.c
gcc -Wall -Wextra -Wno-unused-parameter -C test_mm.c test_util.c syscall.c
gcc -Wall -Wextra -Wno-unused-parameter -C test_mm.c test_util.c syscall.c
gcc -Wall -Wextra -Wno-unused-parameter -C test_mm.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter test_mm.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter test_prio.c test_util.c syscall.c
clear
gcc -c -Wall -Wextra -Wno-unused-parameter test_prio.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter test_prio.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter test_prio.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_prio.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_mm.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_processes.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_processes.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
cleat
clear
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.cclear
clear
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.cclear
clear
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.cclear
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
gcc -c -Wall -Wextra -Wno-unused-parameter -Wno-format test_sync.c test_util.c syscall.c
ls
rm *.o
ls
rm ":set nonu" 
ls
exit
