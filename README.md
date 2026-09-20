*This project has been created as part of the 42 curriculum by jnantes-.*

# Codexion

## Description

Codexion is a concurrency simulation written in C. It models coders sharing a
limited number of USB dongles while trying to compile, debug, and refactor their
code before burnout deadlines are reached.

Each coder runs in its own POSIX thread. A coder needs two dongles to compile,
one on each side, except when there is only one coder, in which case only one
dongle exists and the coder eventually burns out. The simulation stops when a
coder burns out or when every coder has compiled the required number of times.

The project focuses on thread synchronization, mutex-protected shared state,
condition variables, cooldown handling, and FIFO/EDF arbitration through a
custom heap-based priority queue.

### Scheduling

Each dongle owns its own request queue. Requests store:

- the requesting coder;
- arrival time;
- deadline, computed as `last_compile_start + time_to_burnout`.

The queue is implemented as a custom binary heap:

- `fifo` prioritizes the earliest arrival time;
- `edf` prioritizes the earliest burnout deadline;
- deterministic tie-breakers use arrival time and coder id.

### Blocking Cases Handled

**Deadlock prevention:**
Coders acquire dongles in a deterministic order by dongle id. This avoids a
circular wait where every coder holds one dongle while waiting forever for the
next one.

**Single coder case:**
With one coder, there is only one dongle. The coder takes it, cannot compile
because two dongles are required, and eventually burns out.

**Dongle cooldown:**
Each dongle stores `cooldown_until`. When released, the dongle cannot be reused
until the cooldown timestamp has passed.

**Starvation prevention:**
Dongle requests are arbitrated through a heap queue using FIFO or EDF. Under EDF,
coders closer to burnout are prioritized by deadline when requesting a dongle.

**Precise burnout detection:**
A monitor thread checks each coder's last compile start time and stops the
simulation when a coder exceeds `time_to_burnout`.

**Log serialization:**
All output is protected by `log_mutex`, so state messages do not interleave.

**Shutdown:**
The shared stop flag is protected by `state_mutex`. When stopping, the monitor
wakes all dongle queues with condition-variable broadcasts so blocked coders can
exit.

### Thread Synchronization Mechanisms

`pthread_mutex_t` is used for:

- `log_mutex`: serializes output.
- `state_mutex`: protects `stop`, `compile_count`, and `last_compile_start`.
- each dongle mutex: protects physical dongle ownership and cooldown state.
- each queue mutex: protects heap requests and queue size.

`pthread_cond_t` is used for:

- each dongle queue condition variable, allowing coders to sleep until queue
  state changes instead of busy-waiting.

Coders communicate only through shared synchronized state. They do not directly
inspect or signal each other. The monitor performs global shutdown by setting
the shared stop flag and broadcasting to every queue.

Race conditions are avoided by reading and writing shared timestamps, counters,
stop state, queue contents, and logs only while holding the appropriate mutex.

### Project Layout

```text
Makefile
main.c
src/codexion.h
src/dongle_utils.c
src/heap_utils.c
src/monitor_utils.c
src/print_utils.c
src/request_utils.c
src/scheduler_utils.c
src/startup_utils.c
src/state_utils.c
src/thread_utils.c
src/time_utils.c
```

## Instructions

Build the project from the repository root:

```sh
make
```

Run:

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```sh
./codexion 3 1200 200 100 100 2 50 edf
```

Arguments:

- `number_of_coders`: number of coder threads and dongles.
- `time_to_burnout`: maximum time in milliseconds since the last compile start.
- `time_to_compile`: time spent compiling while holding dongles.
- `time_to_debug`: time spent debugging.
- `time_to_refactor`: time spent refactoring.
- `number_of_compiles_required`: stop condition when all coders reach it.
- `dongle_cooldown`: time after release before a dongle can be reused.
- `scheduler`: either `fifo` or `edf`.

Useful Makefile rules:

```sh
make clean
make fclean
make re
```

## Resources

- `pthread_create`, `pthread_join`, `pthread_mutex_*`, `pthread_cond_*`
  documentation.
- POSIX time functions: `clock_gettime`, `usleep`.
- Binary heap / priority queue data structure references.
- Classic dining philosophers concurrency problem discussions.

AI was used as a study and review assistant during development. It helped
explain threads, mutexes, condition variables, heap scheduling, edge cases, and
testing strategy. Code decisions were checked manually with
compilation, `norminette`, and runtime tests.
