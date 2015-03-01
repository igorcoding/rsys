rsys
====

### Recommender system library
Has support of collaborative filtering SVD algorithm

Written in C++ with Python bindings via Boost.Python

In order to build it:
```
mkdir build
cd build
cmake ..
make
```

Python bindings are located under ```rsys-python/``` directory. To build them you will just need to run

```python setup.py install```
