# producer_consumer

## Description
This program simulates a producer-consumer scenario using multiple threads in a C/C++ environment. It demonstrates synchronization techniques to manage access to a shared buffer with semaphores. The program handles multiple consumer threads that process transactions and log their activities, while a single producer thread feeds the buffer with transactions.

## Features
- **Multithreading**: Utilizes multiple threads to simulate consumers processing transactions.
- **Synchronization**: Uses semaphores to control access to a shared buffer between threads to prevent race conditions.
- **Logging**: Logs all activities of threads, including transaction handling and sleeps, to standard output or a specified log file.
- **Dynamic Transaction Handling**: Producer dynamically adds transactions to a buffer which consumer threads process.
- **Sleep Simulation**: Simulates sleep/wait states using nanosleep for specified durations to mimic real-world transaction delays.

## References:
1. https://shivammitra.com/c/producer-consumer-problem-in-c/#
2. https://gist.github.com/navin-mohan/c6719faf72c37be7e20020f31be8cec3
3. https://www.youtube.com/watch?v=HDohXvS6UIk (learned how to use malloc)
4. https://www.youtube.com/watch?v=l6zkaJFjUbM&t=949s
5. https://stackoverflow.com/questions/21856025/getting-an-accurate-execution-time-in-c-micro-seconds#:~:text=A%20simple%20use%20case%20%3A,count()%3B
6. https://stackoverflow.com/questions/65455817/how-to-properly-terminate-multiple-consumers-after-all-producers-have-ended#:~:text=on%20this%20post.-,How%20to%20properly%20terminate%20multiple%20consumers%20after%20all%20producers%20have,of%20processing%20it%20like%20normal.
