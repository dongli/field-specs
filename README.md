Introduction
============

This is a tiny C++ parser created by using `Flex` and `Bison` for parsing the
field specs like:

    "U:3d:keep_vert_coord"
    "calc_pt(P, T):3d"

in the father project `geodiag-cmor`. I store it here for demonstration the
usage of `Flex` and `Bison`.

The test results are:

    $ ./test
    --> Input: 1
    AST Nodes:
      [Number]: 1
    --> Input: 2.0e+3
    AST Nodes:
      [Number]: 2000
    --> Input: U
    AST Nodes:
      [ID]: U
    --> Input: abs(U)
    AST Nodes:
      [Function]: abs
      [ID List]:
      [ID]: U
    --> Input: calc_pt(T, P, Q)
    AST Nodes:
      [Function]: calc_pt
      [ID List]:
        [ID]: T
        [ID]: P
        [ID]: Q
    --> Input: 1+2
    AST Nodes:
      [Expression]:
        [Left]:
          [Number]: 1
        [Operator]: +
        [Right]:
          [Number]: 2
    --> Input: 1.0e+3*(X+2)
    AST Nodes:
    [Expression]:
      [Left]:
        [Number]: 1000
      [Operator]: *
      [Right]:
        [Expression]:
          [Left]:
            [ID]: X
          [Operator]: +
          [Right]:
            [Number]: 2
    --> Input: 2d:keep_vert_coord
    AST Nodes:
      [Spec]: 2d
      [Spec]: keep_vert_coord

Author
======

- Li Dong <dongli@lasg.iap.ac.cn>
