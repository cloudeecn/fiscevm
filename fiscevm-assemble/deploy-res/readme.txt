You can find the source code for the vm at http://fiscevm.googlecode.com/svn/tags/releases/
= Quick Start =
 # Unzip the file to an empty folder (eg. d:\fiscevm in windows, ~/fiscevm in linux)
 # Open command prompt in windows, open an termanal windows in linux
 # Change current dir to the unzipped dir (eg. cd /d d:\fiscevm , cd ~/fiscevm)
 # Run "run HelloWorld" in windows, run "./run.sh HelloWorld"

= Basic Develop = 
 # Unzip the file to an empty folder (eg. d:\fiscevm in windows, ~/fiscevm in linux)
 # Create a new java project in java ide(eg. Eclipse) the project root is the unzipped path ( eg. d:\fiscevm in windows, ~/fiscevm in linux)
 # Open build path setting, set "src" as source folder, set "fisce_scripts" as default output folder. In the lib tab, add the "lib/FiScENV-7.2.0.jar", remove the default jre lib.
 # You can make your own script, the example is Benchmark.java and HelloWorld.java (Yes, it's very "java", you can use the ide's context assistant to help you.)
 # Use "run <your class name>" to run your own script.
