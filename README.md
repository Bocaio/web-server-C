# C Web Server 

A simple web server built from scratch in C. No frameworks, no fluff—just raw sockets and threads for learning the low-level stuff.

## What's inside?
- **Multithreading**: Uses `pthreads` to handle many clients at once.
- **Sockets**: Direct use of `<sys/socket.h>` to listen and accept connections.
- **HTTP**: A basic "Hello World" response to see it in action.

## Quick Start

The easiest way to get started is by using the provided script:

1. **Make the script executable (if needed):**
   ```bash
   chmod +x start_program.sh
   ```
2. **Run the script:**
   ```bash
   ./start_program.sh
   ```

Alternatively, you can compile and run manually:
1. **Compile:**
   ```bash
   gcc -o server server.c -pthread
   ```
2. **Run:**
   ```bash
   ./server
   ```
3. **Test:**
   Open `http://localhost:9090` in your browser or run:
   ```bash
   curl localhost:9090
   ```

## Learning Goals
- How TCP sockets work (`socket`, `bind`, `listen`, `accept`).
- How to manage concurrent connections with threads.
- How HTTP requests and responses actually look.
