#!/bin/bash
ABSPATH=$(cd "$(dirname "$0")"; pwd)
cd $ABSPATH/main/vmcore
vmgen fisce.vmg
sed -i -e 's/(Cell)//g; s/^#line.*//g' fisce-vm.i
