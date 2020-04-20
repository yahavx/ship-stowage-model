# ship-stowage-model

A stowage model for container ships.

### Running a simulation

Compile the program, and supply as command line arguments one or more paths to directories which contains
`Plan`, `Route`, and `.cargo_data` files, in the format explained [here](https://docs.google.com/document/d/10dw03hb-mj6zL8O08K5YSm4KYTeIv7eTmckcezrzH_E/edit).

The simulator will run all available algorithms on each directory. Results will be written to `simulation-outout/results.csv`.