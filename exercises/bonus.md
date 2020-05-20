### Bonus

All the directories referenced below are inside the `bonus` directory.

Our travel main directory (that contains some travel sub-directories) that shows our error handling mechanism, is called `travels-error-handling`.
There is one travel that demonstrates almost all the different kind of errors formats and handling:
- `Travel-Very-Diverse-Errors`
There is also a readme inside this travel directory (`Travel-Very-Diverse-Errors`) that explains it well.

Several other travels in that directory:
- `Travel-No-Errors`: a clean run, no errors folder is generated.
- `Travel-Diverse-Errors`: also show many different errors.
- `Travel-Bad-Input`, `Travel-Bad-Input-2`: travels that can't be executed.

What makes our error handling special:
- It is very informative and well organized, and allows to debug problems very easily.
- Each error has a number, that is always printed at the end.
- During the simulation, at the end of each error section (initialization, port visit), there is a summary of which errors
  were reported by the simulator, and which by the algorithm (algorithm is not penalized for mismatch, but its easy to compare).

### Validation example

We have another travel main directory named `validation` which consists of 1 travel, named `Invalid-Load`, that shows how our simulation validates the algorithm,
and in this case faults him for bad behaviour.
The `validation` directory also contains an `Load-Bad-Position.so`, which is the algorithm to run with it.