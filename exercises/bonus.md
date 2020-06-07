### Bonus

All the directories referenced below are inside the `bonus` directory.

Our travel main directory (that contains some travel sub-directories) that shows our error handling mechanism, is called `travels-error-handling`.

There is one travel that demonstrates almost all the different kind of errors formats and handling, called `Travel-Very-Diverse-Errors`. It contains a `readme` inside that explains the travel.

Several other travels in `travels-error-handling`:
- `Travel-No-Errors`: a clean run, no errors folder is generated.
- `Travel-Diverse-Errors`: also show many different errors.
- `Travel-Bad-Input`, `Travel-Bad-Input-2`: travels that can't be executed.

All those travels should be ran with our algorithms.

What makes our error handling special:
- It is very informative and well organized, and allows to debug problems very easily.
- Each error has a number, that is always printed at the end.
- During the simulation, at the end of each error section (initialization, port visit), there is a summary of which errors
  were reported by the simulator, and which by the algorithmFactory (algorithmFactory is not penalized for mismatch, but its easy to compare).

### Validation example

We have another travel main directory named `validation` which consists of 1 travel, named `Invalid-Load`, that shows how our simulation validates the algorithmFactory,
and in this case faults him for bad behaviour.
The `validation` directory also contains an `Load-Bad-Position.so`, which is the algorithmFactory to run with it.
