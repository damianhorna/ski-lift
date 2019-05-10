### Run:
```bash
mpic++ main.cpp QueueElement.cpp QueueElement.h ThreadManager.cpp ThreadManager.h easylogging++.h -lpthread -o ski.out
mpirun -n 4 ski.out
```

### Logger:
https://github.com/zuhd-org/easyloggingpp/tree/v8.91#logging

v8.91 instead newest version because it doesn't need c++11
