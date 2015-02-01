![logo](logo.png)

# Back-end lib of the LazyChords project
[![Build Status](https://travis-ci.org/lazychords/lazychords-lib.svg?branch=master)](https://travis-ci.org/lazychords/lazychords-lib)

This is the core of the LazyChords project, where the data models (how the melodies, chords,…) are described and the main algorithm is implemented. If you're looking for a gui, please have look [here](https://github.com/lazychords/lazychords-gui)

Due to the essential aspect of this library for the project, we'll focus on the code quality, the test coverage, and the overall stability.

Any feature or modification on the code base must thus be tested (see the testing section for further information).

## Goal of the project

This project aims at providing a tool to allow musicians to find suitable accompaniments for their melodies. Currently, we focus on providing a good Chord Progression that fits the melody, and that sounds as well as possible.

This can be useful independently of the user's musical skills : 
* For beginners, it allows a quick generation of an a ready-to-play accompaniement without any theoretical background about composition.
* For advanced musicians, the software can be a source of interesting/unusual ideas, and speed up the overall composition process by providing a strong basis to start with.


## How does it work ?

The software relies on a dynamic algorithm, that tries to apply expertly designed music rules to the melody. The rules allow to score a given chord progression with respect to a melody, and the algorithm's task is to find the chord progression that maximizes this score.

## Build instruction

To build the project, you need a recent C++ compiler (this project uses C++11, supported since gcc4.8), cmake, make, and the Boost library.

```
https://github.com/lazychords/lazychords-lib.git
sudo apt-get install cmake make g++ libboost-dev libboost-test-dev libboost-program-options-dev
mkdir build && cd build
cmake ..
make -j
```

## Testing

We currently use the [Catch](https://github.com/philsquared/Catch) test framework. It is fairly easy to master, and its features cover what we need for the project.
