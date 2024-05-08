# Building

```
mkdir build
cd build
cmake ..
make
```

# Running

```
# From build directory
./Foo
```

# Sample output from Nektar++ 5.5.0
```
Vertices match:
Lcoord:         -1 -1 -1
OLD   physical: -1 -1 -0.6
NEW   physical: -1 -1 -0.6
Lcoord:         1 -1 -1
OLD   physical: -0.5 -1 -0.6
NEW   physical: -0.5 -1 -0.6
Lcoord:         1 1 -1
OLD   physical: -0.479483 -0.474973 -0.661479
NEW   physical: -0.479483 -0.474973 -0.661479
Lcoord:         -1 1 -1
OLD   physical: -1 -0.5 -0.6
NEW   physical: -1 -0.5 -0.6
Lcoord:         -1 -1 1
OLD   physical: -0.804335 -0.747833 -0.429676
NEW   physical: -0.804335 -0.747833 -0.429676

Interior differs:
Lcoord:         0 0 0
OLD   physical: -0.641909 -0.611403 -0.545578
NEW   physical: -0.647038 -0.61766 -0.530208
Lcoord:         -0.2 -0.2 -0.2
OLD   physical: -0.716263 -0.692459 -0.548265
NEW   physical: -0.718451 -0.695129 -0.541707
Lcoord:         -0.125 -0.125 0.125
OLD   physical: -0.662212 -0.628457 -0.53109
NEW   physical: -0.667261 -0.634616 -0.51596

Consistency check, mapping point from physical to local and back to physical:
is_contained: 1
0: -0.662212 -0.628457 -0.53109
1: -0.657703 -0.622957 -0.544601
error: 0.0152685
```



