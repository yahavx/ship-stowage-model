Command line arguments handling:
- We except to receive 2, 4, or 6 arguments (according to number of flags supplied). Anything besides that is an error.
- If an invalid flag is supplied (i.e. parameter 1/3/5 is not one of -travel_path, -algorithm_path, -output), it is an error.

Any command line argument error prevents the simulator from running, an error is printed to the screen.
Also, if an invalid output directory is supplied (doesn't exist and cannot be created), an error is printed to the screen.
In all above cases the program is terminated, any other error is handled inside the simulator.

General error handling:
We have an "errors" folder (under the output folder) that contains all the error files, and the error documentation is as follows:
- One file named GeneralErrors, that contains (general) errors such as bad algorithm loading, invalid paths, invalid travels (that cannot be run), etc.
- A file for each algorithm-travel pair, with errors that occurred during the run.

An error file (general or travel-algorithm pair specific) is created only if at least one error was found.
The folder is created if at least one file was created (i.e. at least one error, anywhere).

Our full list of errors can be viewed in common/utils/Error.h, the messages format is under the toString method of this class.
For errors related to input files (plan, route, cargo_data, algorithm instructions), we also report a line number for each error.
