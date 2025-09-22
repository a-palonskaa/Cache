# Cache implementation

This repository contains cache implementations: belady and 2q

# Run the program

```bash
git clone https://github.com/a-palonskaa/Cache
cd Cache
```

## To build
```bash
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build ./build
```

## To run belady
```bash
./build/belady
```

## To run 2q
```bash
./build/2q
```

## To run tests
[note:] test scripts (.sh) work on Unix-like systems.

## Before running tests
```bash
chmod +x ./test/compare.sh
chmod +x ./test/e2e_test.sh
```

## To run comparison on small data

```bash
./test/compare.sh generated
```

## To run comparison on big data with time measurement

```bash
./test/compare.sh
```

## To run e2e tests use

```bash
./test/e2e_test.sh
```

## Input|output Format
input:
```
<cache cap> <requests amount> <r1> <r2> <r3> ... <rm>
```
output:
```
<number of hits>
```

# Ideal to 2Q caching comparisson

2Q VS Belady algorithm on 100 randomly generated tests(requests amount from cap to 6×cap).
- Capacity 4–23:
    2Q was 18.8% worse than Belady.
- Capacity 100–199:
    2Q was 12.3% worse than Belady.
- Capacity 1000–1100:
    2Q was 10.0% worse than Belady.
- Capacity 10000–10100:
    2Q was 9.5% worse than Belady.
