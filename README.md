### Run:
```bash
mpic++ main.cpp QueueElement.cpp QueueElement.h ThreadManagerBase.cpp ThreadManagerBase.h easylogging++.h easylogging++.cc MessageType.h ThreadManager.cpp ThreadManager.h Logger.h -std=c++11 -pthread -o ski.out
mpirun -n 2 ski.out
mpirun -n 4 --hostfile hostfile ski.out | grep -e "INFO.*MAI"
```

### Logger:
https://github.com/zuhd-org/easyloggingpp/tree/v8.91#logging

v8.91 instead newest version because it doesn't need c++11

#### In case of problems:
```bash
ps aux | grep ski | tr -s " " | cut -d " " -f2 | xargs kill -9
```