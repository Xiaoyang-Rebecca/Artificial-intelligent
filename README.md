# Artificial-intelligent

The projects for AI



Thanks for using reinforcement learning to solve PD world !

Contact XiaoyangLi (xiaoyang.rebecca.li@gmail.com)

- Collaborators: [Priyal Kulkarni
](https://www.linkedin.com/in/priyal-kulkarni-1205/),[Sarthak Sharma](https://www.linkedin.com/in/sarthak-sharma-uh/)


%%%%%%%%%%%%%%%%%%%%   Q_learning    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



Language         :  C++ 


Program compiler :  GNU GCC (IDE-codeblocks) http://www.codeblocks.org/downloads


[ output.txt ]
        
Header line: The experiment name, the numbered execution (each experiment is executed twice), the seed used for this execution.

		#Subsequent lines are the q-tables:

		- One line indicating step number, Reward and Blocks Delivered after every 40 steps

		- The q-table using state representation 2: (i, j, x), ordered as: N S E W after first 100 steps, after 1st dropofff is filled and after each termination.

[Q_learning results]
           
	    Exactly the same content as Output.txt except we put all the result in separate files and 6 folders

						


%%%%%%%%%%%%%%%%%%%%   Visualization %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Language         :  Matlab

[QtableFullstate . m] is the main function to generate screen shot of Q table and Fullstate

     Input template: (We need to copy the Q table from txt file to the corresponding templates)

		Qread0.dat for Q table x=0
 
		Qread1.dat for Q table x=1
 
     Output:

         original    fig images in [Visualization \ QtableFullstate Fig] folder

         compacted   jpg images in [QtableFullstate JPEG] folder 


[PerformanceMeasure.m] is the main function to generate performance measurement
 
    Input template: (We need to copy the Q table from txt file to the corresponding templates)

		Perf.xlsx (variablenames = { 'steps','Reward','BlocksDelivered','BankAccount','Note'}

    Output :
 
         original   fig images in [Visualization \ PerformanceMeasure Fig] folder
 
         compacted   images in appendix chaps of report.
