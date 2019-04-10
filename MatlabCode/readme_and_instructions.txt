LizardPlusMaze repository is meant to control a plusmaze through ardiuno interface.

installation notes:
GUIlayout toolbox and matlab2014b or higher are required.


start by running MegaPlusMaze.m

follow these steps:
1. select data path. this path would hold a folder titled with the current date YYYY-MM-DD, with 3 subfolders: "behavior",cheetah","videos". saved data will be stored in the "behavior" folder.

2. fill in header information:
experimenter, animal number, temperature etc.

3. select task type. upon selection a trial sequence will be generated.

4. to prepare the next trial, press "setup". setup button will turn green once ready.
5. once doors and arms are setup, press start to initiate the trial. start button will turn green
6. press stop to end the trial. stop button will turn green and rest of buttons red. trial time is recorded automatically and noted in table
next trial is automatically selected upotn end of previous trial.

after each trial has ended, table data is saved into a .mat file.

7. to end the session, close the current matlab window.


