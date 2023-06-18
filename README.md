# Multi-threaded Task Manager Application

![Task Manager](https://github.com/Tasalog/Multi-threaded-Task-Manager-Application/blob/main/images/task_manager.png)

The Multi-threaded Task Manager Application is a Java-based project that aims to provide a multi-threaded task management system. It allows users to create and manage tasks in a concurrent environment, leveraging the power of multiple threads to execute tasks simultaneously.

## Features

- **Task Creation:** Users can create tasks with a specified name and duration.
- **Task Scheduling:** The application automatically schedules tasks to be executed based on their duration and the available threads.
- **Thread Pool:** The application manages a thread pool, allowing for efficient task execution by reusing threads.
- **Concurrency:** Tasks are executed concurrently using multiple threads, maximizing system resources and improving performance.
- **Status Monitoring:** Users can monitor the status of tasks, including their execution progress and completion.
- **Interruptible Tasks:** Users can interrupt running tasks, allowing for dynamic management of task execution.
- **Task Prioritization:** The application supports task prioritization, enabling users to define the importance of each task.
- **Task Dependency:** Users can specify dependencies between tasks, ensuring that dependent tasks are executed in the correct order.
- **Logging:** The application provides detailed logging of task execution, including start time, end time, and any exceptions that occurred.

## Installation

To run the Multi-threaded Task Manager Application, follow these steps:

1. Clone the repository:

   ```bash
   git clone https://github.com/Tasalog/Multi-threaded-Task-Manager-Application.git```
   
2. Open the project in your preferred IDE.

3. Build the project using your IDE's build or compile command.

4. Run the compiled application, and you're ready to start managing tasks!

## Usage

The Multi-threaded Task Manager Application provides a user-friendly command-line interface (CLI) for task management. Upon launching the application, you'll see a prompt where you can enter various commands to interact with the system.

Here are some available commands:

`create <task_name> <duration>`: Creates a new task with the given name and duration.

`list`: Lists all the created tasks along with their status. 

`priority <task_name> <priority>`: Sets the priority of a task (1 to 10, with 10 being the highest).

`exit`: Exits the application.

Please note that the commands are case-insensitive.  
