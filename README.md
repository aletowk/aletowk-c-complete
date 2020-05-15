# Aletowk C Complete

It uses my static-queue and state_manager implementation.

## How To:

### Threads: 

Use the main in ./src/mains/main_thread.c 

The following lines will start a TCP Server thanks to thread and socket utilities on the address 127.0.0.1 port 8080. Also, it creates a thread to test state_manager in an asynchronous way
```
$make
$make run
```

So in another terminal you can find a way to send data arrays like :
```
data = [0,0,0] # Do nothing
data = [1,1,0] # Change into Survival State
data = [1,2,0] # Change into Mission state
```

