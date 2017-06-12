#!/usr/bin/env python
import sys
"""

A utility to transform a kernel source code into a macro for embedding


./utils/convert_kernel.py ./kernel.cl


"""


KERNEL_FILE=sys.argv[1]

fp = open(KERNEL_FILE, "r")

KERNEL_SOURCE=fp.read()

#print (KERNEL_SOURCE)

KERNEL_MACRO="#define ENGINE_OPENCL_KERNEL_SOURCE_DEFAULT "
KERNEL_MACRO+='"'
KERNEL_MACRO+=KERNEL_SOURCE.replace("\n", "\\\n")
KERNEL_MACRO+='"'

print (KERNEL_MACRO)



