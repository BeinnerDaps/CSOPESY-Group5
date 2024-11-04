# MO1 - Process Multiplexer and CLI

## Members (**Group 5 S16**)
1. DEL ROSARIO, MICHAEL GABRIEL A.
2. GONZAGA, EROS ANEELOUV A.
3. KIMHOKO, JAMUEL ERWIN C.
4. MAGDAMO, BIEN RAFAEL O.

## Instructions on How to Run
Step 1. **Download the files under the repository**
        Open "CSOPESY_Group5_Project" > "CSOPESY_Group5_Project.sln"

Step 2. **Complie the project with Visual Studio**
        Ensure the following files are included in the project:
            - CSOPESY-S16_Group5.cpp
	        - Commands.h
            - Commands.cpp
	        - Data.h
            - Data.cpp
	        - Screen.h
            - Screen.cpp
	        - Scheduler.h
            - Scheduler.cpp
	        - ProcessInfo.h
	        - Config.h
	        - config.txt

Step 3. **Run the project through Visual Studio**
        3.1.    Enter path to config.txt file
        3.2.	Type "scheduler-test" to run scheduler
        3.3.	Type "screen -s <process>" to create new process
        3.4. 	Type "screen -r <process>" to open existing process
        3.5. 	Type "process-smi" to see process progress on screen command
        3.6. 	Type "screen -ls" to see all processes
        3.7. 	Type "exit" to return to main menu from screens.
        3.8.	Type "report-util" to write to text file

**Note:** The entry class file where the main function is located is in: `CSOPESY-S16_Group5.cpp`
