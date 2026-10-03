*This project has been created as part of the 42 curriculum by drezan.*

## Description

Codexion is a multithreaded simulation written in C that models coders in a shared
co-working hub, competing for a limited set of USB dongles to compile quantum code.
Each coder is represented by a POSIX thread. Compiling requires holding two dongles
simultaneously (the one to the coder's left and the one to their right), and dongles
are shared between neighboring coders in a circular arrangement.

The goal of the project is to implement correct, deadlock-free, and starvation-free
resource sharing between threads, under two different arbitration policies:

- **fifo**: dongles are granted in strict arrival order.
- **edf**: dongles are granted to whichever waiting coder has the earliest burnout
  deadline (`last_compile_start + time_to_burnout`).

The simulation also enforces a mandatory cooldown period after a dongle is released,
and runs a separate monitor thread that detects burnout (a coder failing to start
compiling in time) with millisecond precision, stopping the simulation immediately
when it occurs.

## Instructions

### Compilation

```
make
```

Builds the `codexion` binary with `-Wall -Wextra -Werror -pthread`.

Other targets:
- `make clean` — remove object files
- `make fclean` — remove object files and the binary
- `make re` — full rebuild

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) in the simulation |
| `time_to_burnout` | Max time (ms) a coder can go without starting a compile before burning out |
| `time_to_compile` | Time (ms) spent compiling, while holding both dongles |
| `time_to_debug` | Time (ms) spent debugging |
| `time_to_refactor` | Time (ms) spent refactoring |
| `number_of_compiles_required` | Simulation stops once every coder reaches this many compiles |
| `dongle_cooldown` | Time (ms) a dongle stays unavailable after being released |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

Example:

```
./codexion 5 3000 200 150 150 10 100 edf
```

All arguments are mandatory and validated; invalid input (negative numbers,
non-integers, wrong argument count, or an unrecognized scheduler) causes the
program to reject the input and exit cleanly without starting the simulation.

## Resources

- *Understanding CPU, Threads and Thread Scheduling for Multithreading Programming* — Medium: https://towardsdev.com/understanding-cpu-threads-and-thread-scheduling-for-multithreading-programming-bb6647584357
- *What are Threads in Computer Processor or CPU?* — GeeksforGeeks: https://www.geeksforgeeks.org/operating-systems/what-are-threads-in-computer-processor-or-cpu/
- *Understanding Threads in C* — Medium: https://medium.com/@akshatarhabib/understanding-threads-in-c-c9feb5e9372a
- *Thread Management Functions in C* — GeeksforGeeks: https://www.geeksforgeeks.org/c/thread-functions-in-c-c/

### How AI was used

An AI assistant (Claude) was used throughout development as a reviewer and
debugging aid, not as a code generator for the mandatory logic:

- **Design discussion**: talked through why a per-dongle ticket lock couldn't
  provide genuine FIFO ordering, and why a priority-queue (heap) per dongle,
  keyed by arrival order or by deadline, was needed to support both `fifo` and
  `edf` under one mechanism.
- **Bug finding, not bug fixing**: the AI was explicitly asked to point out
  problems in code I had already written, rather than write it. This surfaced
  several concurrency bugs I then fixed myself, including:
  - a lost-wakeup bug from waiting on the wrong condition variable when
    acquiring two dongles,
  - a deadlock from acquiring dongles in inconsistent order (left/right
    instead of sorted by id),
  - a race where a dongle's wait queue was mutated outside its mutex,
  - a race where a coder could be granted a dongle, then have its cooldown
    wait begin, while a second coder's EDF-reordered entry also appeared to
    win, resulting in a double grant,
  - an unprotected read/write of `compile_count` and `last_compile` shared
    between a coder's own thread and the monitor thread.
- **Tooling guidance**: how to use ThreadSanitizer and Valgrind effectively on
  a timing-sensitive multithreaded program, including why their slowdown can
  trigger false burnouts and how to adjust test parameters (and, for Valgrind,
  `--fair-sched=yes`) to get a meaningful result instead.
- **Norm compliance**: suggestions on how to split an over-length function
  into a norm-compliant helper without changing its behavior.

All AI-suggested fixes were manually reviewed, traced through by hand, and
tested (via stress runs under both schedulers, ThreadSanitizer, and Valgrind)
before being accepted.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions)**: a coder always acquires its
  two dongles in a fixed global order (lowest id first), rather than in
  `left`/`right` order. This breaks the circular-wait condition: no coder can
  ever be holding a dongle while blocked waiting for a lower-id dongle, which
  makes a cycle of mutual waiting impossible, regardless of coder count or
  timing.
- **Starvation prevention**: each dongle maintains its own wait queue, stored
  as a binary heap shared by the (at most two) coders that use it. Under
  `fifo`, the heap key is arrival order; under `edf`, it is the coder's
  burnout deadline, with coder id as a tie-break. A coder only proceeds once
  it is at the head of the queue, which guarantees that no waiting coder is
  passed over indefinitely by later arrivals.
- **Atomic grant under EDF reordering**: a coder is removed from the heap and
  the dongle is marked in-use in the same critical section as the "am I at
  the head and free?" check that lets it proceed — before any cooldown wait
  begins. This prevents a race where a second coder's entry could reorder the
  heap (because EDF reorders on insert) while the first coder's cooldown wait
  was in progress, which previously allowed both coders to believe they had
  won the same dongle.
- **Cooldown handling**: after releasing a dongle, a coder cannot reacquire it
  until `dongle_cooldown` milliseconds have passed. This is enforced with
  `pthread_cond_timedwait` against an absolute deadline, re-checked in a loop
  to guard against spurious or unrelated wakeups returning early.
- **Precise burnout detection**: a dedicated monitor thread repeatedly checks
  every coder's time since their last compile start against
  `time_to_burnout`, independent of the coder threads themselves, so
  detection isn't delayed by a coder being blocked on a lock.
- **Clean shutdown on burnout**: once the monitor detects a burnout, it sets a
  shared stop flag and broadcasts the condition variable of every dongle, not
  just the dongles adjacent to the coder that burned out. Every coder thread's
  wait loop re-checks the stop flag, so a coder waiting on a dongle anywhere
  in the ring wakes up and exits instead of blocking forever, allowing all
  threads to be joined and the program to exit cleanly.
- **Log serialization**: each dongle's own mutex also guards the "has taken a
  dongle" print tied to that acquisition, and all per-coder state prints
  happen from a single thread at a time holding the relevant lock, so two
  state-change messages never interleave mid-line.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle**: protects that dongle's wait queue
  (the heap), its `in_use` flag, and its `last_compile` timestamp. Every
  read or write of this shared state happens while holding the dongle's own
  mutex — never from outside it — which is what makes the heap and in-use
  flag safe to share between the two coders that use a given dongle.
- **`pthread_cond_t` per dongle**: used for two distinct kinds of waiting on
  the same dongle, both guarded by the same mutex:
  - waiting for the dongle to become available to the coder at the head of
    its queue (`pthread_cond_wait`, woken by a broadcast on release or on
    simulation stop), and
  - waiting out the cooldown period after release (`pthread_cond_timedwait`
    against an absolute deadline, re-checked in a loop).
  A single condition variable serves both purposes because both waits are
  guarded by the same predicate-and-mutex discipline, and a release always
  broadcasts, so any waiter re-evaluates whichever condition it's actually
  blocked on.
- **Per-coder `pthread_mutex_t`**: guards `compile_count` and `last_compile`
  on each coder, since both fields are written by the coder's own thread and
  read by the monitor thread. The monitor copies both fields into local
  variables while holding the lock, then evaluates the burnout condition
  against the snapshot, keeping the critical section short.
- **A global stop flag protected by its own mutex**: `sim_stop()` locks,
  reads, and unlocks a shared flag, used both by coder threads (to know when
  to abandon waiting on a dongle) and by the monitor (to signal that burnout
  was detected). This mutex is always released before any dongle mutex is
  acquired, in both the monitor and the coder threads, which avoids a
  lock-ordering inversion that could otherwise deadlock the shutdown path
  against an in-progress dongle acquisition.
- **A heap (priority queue) per dongle**, implemented manually since C89 has
  no standard container for this, used to decide who is "next" for a given
  dongle under either scheduler. The only difference between `fifo` and `edf`
  is the key used to order entries; the acquisition logic that consumes the
  heap is otherwise identical for both.
- **Race conditions avoided by example**: the dongle's wait queue is only
  ever mutated while holding that dongle's mutex (insertion on request,
  removal on grant) — an earlier version called the insertion function before
  locking, which allowed two coders requesting the same dongle to corrupt the
  heap concurrently. Similarly, a coder's grant (removing it from the queue
  and marking the dongle in-use) happens in the same locked section as the
  check that authorized it, rather than after a cooldown wait that releases
  the lock, which is what previously allowed a dongle to be granted twice
  under `edf`.
