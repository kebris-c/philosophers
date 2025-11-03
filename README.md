# philosophersphilosophers

philosophers is a project that simulates the classical Dining Philosophers problem, focusing on concurrency, synchronization, and avoiding deadlocks. The goal is to implement multiple philosophers (processes or threads) who alternately think, eat, and sleep while sharing forks, without causing starvation or race conditions.

🧠 Objective

The project aims to:

Understand multithreading and process management in C.

Learn to use mutexes, semaphores, or other synchronization tools.

Prevent deadlocks, data races, and starvation.

Manage timing for actions like eating, sleeping, and thinking precisely.

⚙️ Usage Compilation

Compile the program with:

make

This will generate the philo executable.

Example ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

Example run:

./philo 5 800 200 200

5 philosophers

Each dies if they don’t eat in 800ms

Each eats for 200ms

Each sleeps for 200ms

Optional last argument specifies how many times each philosopher must eat before ending the simulation.

🔧 Rules

Philosophers take forks one at a time.

A philosopher dies if they don’t eat before time_to_die.

The simulation ends either when a philosopher dies or all philosophers have eaten the specified number of times.

Must handle any number of philosophers efficiently and safely.

🧹 Makefile Rules Command Description make Compile the executable make clean Remove object files make fclean Remove objects and the executable make re Rebuild everything make help Show a brief help hint of available rules ⚡ Key Points

Uses threads or processes depending on the variant.

Implements mutexes or semaphores to avoid conflicts on shared forks.

Accurate time management for eating, sleeping, and thinking actions.

Prevents deadlocks and data races.

Fully compliant with the 42 Norm.

👨‍💻 Author

By kebris-c
