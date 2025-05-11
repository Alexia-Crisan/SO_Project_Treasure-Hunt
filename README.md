# Treasure Hunt Game System

This repository contains a C-based simulation of a treasure hunt game system. The project is split into three phases, each adding more complexity to the system, focusing on file systems, process management, signals, inter-process communication, and integration with external tools. The system allows users to create, manage, and participate in digital treasure hunts, store data, communicate between processes, and compute scores.

---

## Project Overview

### Phase 1: File Systems
In Phase 1, the system focuses on storing and managing treasure hunt data using file operations. The `treasure_manager` program handles:
- Adding, viewing, and removing treasures.
- Managing game sessions (hunts) through directories.
- Storing treasure data in binary files.
- Logging operations and creating symbolic links for hunt logs.

### Phase 2: Processes & Signals
Phase 2 extends the system to support multi-process communication. The `treasure_hub` program:
- Interacts with the user via a command-line interface.
- Starts a background monitor process to track treasure hunts.
- Communicates with the monitor using signals (SIGUSR1) to request actions like listing hunts or viewing treasures.
- Supports the `stop_monitor` and `exit` commands with proper process termination handling.

### Phase 3: Pipes, Redirects & External Integration
Phase 3 completes the system by integrating pipes and redirects for inter-process communication. The system:
- Sends data between the monitor and the main process via pipes.
- Adds the `calculate_score` command to calculate a user's total score in each hunts.
- Integrates an external program that calculates scores and communicates the results back to the main process.

---

## Features

- **File Operations:**
  - Create and manage hunts with treasure records stored in binary files.
  - List, view, add, and remove treasures.
  - Log operations and manage directories.
  - Symbolic links for easy access to logged files.

- **Process Management:**
  - Background process for monitoring hunts.
  - Signal-based communication between processes.
  - Graceful termination of processes using `SIGCHLD`.

- **Inter-Process Communication:**
  - Use of pipes for sending data between the monitor and the main process.
  - External score calculation program integrated via pipes.
  
---

