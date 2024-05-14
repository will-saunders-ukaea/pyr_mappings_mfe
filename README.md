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
Consistency check, mapping point from physical to local and back to physical:
is_contained: 1
0: -0.662212 -0.628457 -0.53109
1: -0.657703 -0.622957 -0.544601
error: 0.0152685

Consistency check, mapping point from physical to local and back to physical:
is_contained: 0
0: 0.371997 0.351053 -0.472035
1: 0.385513 0.362541 -0.472035
error: 0.0177381
```


# Sample output from Nektar++ 5.3.0 (commit 2e0fb86da236e7e5a3590fcf5e0f608bd8490945)
```
Consistency check, mapping point from physical to local and back to physical:
is_contained: 1
0: -0.662212 -0.628457 -0.53109
1: -0.662212 -0.628457 -0.53109
error: 1.11022e-16

Consistency check, mapping point from physical to local and back to physical:
is_contained: 1
0: 0.371997 0.351053 -0.472035
1: 0.371997 0.351053 -0.472035
error: 1.24127e-16
```

