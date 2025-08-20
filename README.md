# 🖥️ Animated Process Manager (Round Robin Simulation)

This project is an **animated simulation of a process scheduler** using a **dynamic queue (TAD Cola Dinámica)**, without considering process priorities. It implements the **Round Robin** scheduling algorithm, one of the most widely used CPU scheduling policies.

Processes are given a fixed time slice (quantum) of **1 second**.

* If a process does not finish during its quantum, it is requeued.
* If a process finishes, it is moved to the **attended processes queue**.

The program displays the simulation **graphically inside the Windows CMD**, including CPU usage, execution progress, and process animations.

---

## 👤 Authors

* **Base implementation**: *Edgardo Adrián Franco Martínez*
* **Implementation & adaptation**: *Escompiladores*
  - Arciniega Valdez Yair Emmanuel
  - Marcial Jiménez Víctor Ariel
  - Ramírez Hernández Edwin


**Version:** `1.100.2` (May 2025)

---

## 📂 Project Structure

```
📁 RoundRobin-Scheduler
├── main.c               # Main program logic
├── TADColaDin.c         # Dynamic queue implementation
├── TADColaDin.h         # Queue header file
├── presentacionWin.c    # Windows CMD drawing and animations
├── presentacion.h       # Drawing functions header
├── procesos.exe         # Compiled executable (Windows)
└── README.md            # Documentation
```

---

## ⚙️ Compilation

Compile the project using **GCC**:

### Windows:

```bash
gcc presentacionWin.c TADColaDin.c main.c -o procesos.exe
```

### Linux / WSL (may require adaptation for Windows.h dependencies):

```bash
gcc main.c presentacionWin.c TADColaDin.c -o procesos
```

---

## 🚀 Usage

1. Run the program:

   ```bash
   ./procesos.exe
   ```

2. Enter the **number of processes** to simulate.

3. For each process, provide:

   * Name
   * Activity
   * ID
   * Execution time

4. The CMD will display:

   * **CPU monitor** with currently running process
   * **Progress bar** showing execution percentage
   * **Next process to be attended**
   * **Last process attended**
   * **Table of completed processes with total execution times**

---

## 🎮 Features

* Round Robin process scheduling (1 second quantum)
* Dynamic queue implementation
* Real-time **animated display** in CMD
* Process info visualization (name, ID, activity, execution time)
* Execution time measurement with `clock()`
* Graphical animations for process entry/exit

---

## CMD output

![WhatsApp Image 2025-08-20 at 3 44 24 PM](https://github.com/user-attachments/assets/daa4b5d6-8e20-4644-854b-6231429225d8)


---

## 🛠️ Requirements

* **Windows CMD (120x30 recommended size)**
* **GCC compiler** (e.g., MinGW for Windows)
* ANSI escape code support for colors

---

## 📖 Notes

* This simulation does **not** consider process priorities.
* Quantum is fixed to **1 second**, but can be modified in `#define TIEMPO_BASE`.
* Designed for **educational purposes** in Operating Systems courses (ESCOM-IPN).

