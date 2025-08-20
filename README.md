# Animated Process Manager Simulation (Round Robin)

**Authors:**

* Original code and implementation: Edgardo Adrian Franco Martinez
* Current implementation: Escompiladores

**Version:** 1.100.2 (May 2025)

## Description

This program simulates a **Round Robin CPU scheduling algorithm** without considering process priorities. Modern processors often use this method, which assigns each process a fixed CPU time called a *quantum*. If the process does not finish in this time, it is interrupted and moved to the end of the ready queue, allowing the next process to run.

This simulation:

* Takes a list of processes provided by the user.
* Processes them one by one with a quantum of 1 second.
* Moves completed processes to the "Processed" queue.
* Re-enqueues incomplete processes until they finish.
* Animates the process queue in the command prompt.

## Files

* `TADColaDin.c` — Implementation of the dynamic queue (TAD Cola).
* `TADColaDin.h` — Header file for the dynamic queue.
* `main.c` — Main program that runs the simulation.
* `presentacionWin.c` — Handles the animated visualization in the Windows CMD.
* `presentacion.h` — Header file for presentation/animation functions.
* `procesos.exe` — Precompiled executable (Windows).

## Compilation

For Windows:

```bash
gcc presentacionWin.c TADColaDin.c main.c -o main.exe
```

For Linux/macOS:

```bash
gcc main.c presentacionWin.c TADColaDin.c -o main
```

Run the executable after compilation:

```bash
./main.exe   # Windows
./main       # Linux/macOS
```

## How it works

* The program visualizes the process queue in the command prompt.
* Each process is served for 1 second (quantum).
* Processes that finish are moved to the "Processed" queue.
* Processes that do not finish are placed back at the end of the queue.
* The simulation continues until all processes are completed.

