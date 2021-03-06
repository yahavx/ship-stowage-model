# ship-stowage-model

A stowage model for container ships.

### Running a simulation

Compile the program using `make`, and run in terminal `./ex1 {arg1} {arg2} ... {argn}`, where:
* `arg1` is a directory path, which will be the output directory.
* `arg2`-`argn` are paths to directories, each is a `Travel` directory (at least one should be supplied).

`Travel` directory: consists of `Plan`, `Route`, and `.cargo_data` files, in the format explained [here](https://docs.google.com/document/d/10dw03hb-mj6zL8O08K5YSm4KYTeIv7eTmckcezrzH_E/edit).

The simulator will run all available algorithms on each `Travel` directory. Currently there is one algorithm which we 
run twice on each `Travel` (to get a sense of feeling for comparing algorithms).

The following results will be written to the output directory provided:
* `simulation.results.csv` - number of operations for each algorithm and each travel.  
* `simulation.errors.csv` - errors the simulator detected in each algorithm.
* `output.txt` - stdout capture (simulation logging).
* `error.txt` - stderr capture (non-algorithm errors: missing files, etc).

The last two are logging files, and can be ignored.

### Provided travel examples

Two sets of travels provided: 
* `Travel_1` 
* `Travel_2`, `Travel_3`

To run:
```sh
$ ./ex1 output_1 input/Travel_1
$ ./ex1 output_2 input/Travel_2 input/Travel_3
```

The expected results for each run are at `expected_output/1` `expected_output/2` (respectively).

Compare the results to the expected output:
```sh
$ cat output_1/simulation.results.csv expected_output/1/simulation.results.csv
$ cat output_2/simulation.results.csv expected_output/2/simulation.results.csv
```