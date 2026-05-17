# Garmin FIT C++ SDK

## FIT SDK Documentation
The FIT SDK documentation is available at [https://developer.garmin.com/fit](https://developer.garmin.com/fit)
## FIT SDK Developer Forum
Share your knowledge, ask questions, and get the latest FIT SDK news in the [FIT SDK Developer Forum](https://forums.garmin.com/developer/).

## Requirements
A C++ compiler is required to compile the FIT C++ SDK, such as the GNU g++/gcc, MSVC, or Apple Clang compilers.

## Install
The FIT C++ SDK can be [cloned from the FIT C++ SDK repository on GitHub](https://github.com/garmin/fit-cpp-sdk).

## Examples
Example implementations of the FIT SDK are provided and can be found in the examples/ folder.
* **Decode**: Demonstrates decoding a FIT file; and use of the Decode and Message Broadcaster classes and Message Listener event interfaces.

* **Encode**: Demonstrates encoding an activity FIT file and programmatic generation of messages.

To compile the examples, leverage the makefiles. Note that the makefiles use the GNU g++ compiler.
```bash
# Navigate to the example
cd /path/to/fit/sdk/cpp/examples/decode

# Build the example project
make clean
make

# Run the compiled example
./decode someFitFile.fit
```

```bash
# Navigate to the example
cd /path/to/fit/sdk/cpp/examples/encode

# Build the example project
make clean
make

# Run the compiled example
./encode
```
