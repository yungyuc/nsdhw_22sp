========================
Columnar Structure Array
========================

--------
Abstract
--------

An **array of structure (AoS)** is not cache-friendly nor SIMD friendly. One of its replacement is **structure of array (SoA)**.
However, AoS is so straightforward for programmers to code in, and easy to maintain. It is also fundamental for OOP.

This project designs a structure helper for programmers to use SoA, but to code as in AoS.

---------
Objective
---------
- C++ base-level API
  
  - Fundamental Types support
  - Custom type support, possibly at declaration phase
  - Wrapper for STL containers *(optional)*
  - Automatic type extension by type structure discovery, with parsers *(optional)*

- Python API
  
  - A fixed-type list wrapper for PyObject-s
  - NumPy- & Pandas- friendly *(optional)*

- Ensure memory layout properties hold
  
  We should satisfy `key features of columnar format <https://arrow.apache.org/docs/format/Columnar.html#:~:text=The%20columnar%20format%20has,access%20in%20shared%20memory>`_ ,
  
  - Data adjacency for sequential access of same member
  - Constant-time random access
  - SIMD and vectorization friendly
  - Relocatable with zero-copy access

  And addtionally,

  - Compile-time address resolution
    
    Hopefully with same instructions as AoS, with light object reference handle (not too much than a pointer).
    
  - Optimizations for cache locality, e.g. padding *(optional)*

--------
Schedule
--------
.. list-table::
   :widths: auto
   :header-rows: 1

   * - Week
     - Description
   * - Week 1
     - Draft high-level prototypes.
   * - Week 2
     - Implement and test core C++ module.
   * - Week 3
     - Benchmark and check if requirements are met for C++ module.
   * - Week 4
     - Draft C++ APIs with tests.
   * - Week 5
     - Implement Python binding to C++ module.
   * - Week 6
     - Draft Python APIs with tests.
   * - Week 7
     - Overall test & re-benchmark.
       Draft project report & documentation.
   * - Week 8
     - Optimizations & addtional features.

---------
Reference
---------
- https://yyc.solvcon.net/en/latest/nsd/schedule/22sp_nycu/schedule.html#columnar-array
- https://arrow.apache.org/docs/format/Columnar.html#buffer-alignment-and-padding