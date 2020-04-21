# ship-stowage-model

A stowage model for container ships.

### Running a simulation

Compile the program, and supply as command line arguments (in order):
* A directory path, which will be the output directory.
* One or more paths to directories which contains
`Plan`, `Route`, and `.cargo_data` files (aka `Travels`), in the format explained [here](https://docs.google.com/document/d/10dw03hb-mj6zL8O08K5YSm4KYTeIv7eTmckcezrzH_E/edit).

The simulator will run all available algorithms on each directory. The following results will be written to the output directory provided:
* `simulation.results.csv` - number of operations for each algorithm and each travel.  
* `simulation.errors.csv` - errors the simulator detected in each algorithm.
* `output.txt` - stdout capture (simulation status at each point).
* `error.txt` - stderr capture (non-algorithm errors: missing files, etc).