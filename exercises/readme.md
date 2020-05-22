### Makefiles

To use the two `makefile`s successfully, its better to `make` the algorithms first, and then the simulator.
If you want to `make` the simulator first, make sure you do `make clean` after (from the algorithms), so it will generate all the `.o` files from scratch (and not use `.o` files created by the simulator, which its not capable of).

### Critical errors
Critical errors prevents the `simulator` program from running.
- If the output path supplied (when supplied) is invalid (folder doesn't exist and cannot be created), it is a critical error.
- Command line arguments.  
    - We except to receive 2, 4, or 6 arguments (according to number of flags supplied). Anything besides that is a critical error.
    - If an invalid flag is supplied (i.e. parameter 1/3/5 is not one of -travel_path, -algorithm_path, -output), it is a critical error.

In all above cases, the program is terminated immediately, and a message is a printed to the screen.
Any other error is handled by the simulator.

### Simulator error handling
The simulator generates an "errors" folder (under the output folder) that contains all the error files, and the error documentation is as follows:
- One file named `GeneralErrors`, that contains (general) errors such as bad algorithm loading, invalid paths, invalid travels (that cannot be run), etc.
- A file for each algorithm-travel pair, named `algorithmName_travelName_errors` with errors that occurred during the run.

Any error file is created only if at least one error was found.

Our full list of errors can be viewed in common/utils/Error.h, the messages format is under the toString method.
For errors related to input files (plan, route, cargo_data, algorithm instructions), we also report a line number for each error.
