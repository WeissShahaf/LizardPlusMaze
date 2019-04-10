
close all; clear;
cd(  'Z:\WeissShahaf\PlusMaze\documentation\LizardPlusMaze')
%  MegaPlusMaze


%load file
filename='Z:\WeissShahaf\PlusMaze\GL0148\20180626\148_2018-06-26-12-05-01.mat';
data=load(filename);
data.filename=filename; clear filename;
%give variable names to table columns
% data.trialTable.Properties.VariableNames={'Selected', 'Trial', 'StartArm', 'TargetArm', 'ChosenArm', 'TrialTime', 'Comment'};
% data.headerTable.Properties.VariableNames={'Day', 'Experimenter', 'AnimalID', 'TrialType', 'NumberOfTrials', 'Sensor', 'TrainingPhase', 'Temp', 'Comment'};
%delete trial selection column
data.trialTable(:,1)=[];

temp=(data.trialTable(:,4))







