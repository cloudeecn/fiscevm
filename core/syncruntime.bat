@echo off
set FISCE_PATH=../../fisce-vm
rsync -rv --delete %FISCE_PATH%/FiScENV/target/classes/* runtime
