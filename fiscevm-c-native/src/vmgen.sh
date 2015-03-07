#!/bin/bash
ABSPATH=$(cd "$(dirname "$0")"; pwd)
cd $ABSPATH/main/vmcore
vmgen fisce.vmg
sed -i ".vmbak" -e 's/(Cell)//g; s/^#line.*//g' fisce-vm.i
cp fisce-labels.i fisce-labels.i.vmbak
grep ^INST_ADDR fisce-labels.i.vmbak > fisce-labels.i
