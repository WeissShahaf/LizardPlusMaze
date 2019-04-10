classdef MegaPlusMazeUi < handle
    
    properties (Access = private)
        parent
        
        %Header
        headerPanel
        headerButtons
        buttonGroup_header
        text_pathDescription
        text_path
        pushButton_path
        text_timer
        
        %Trial table
        trialPanel
        trialButtons
        table_header
        table_trials
        
        %Control
        controlPanel
        controlButtons
        buttonGroup_control
        pushButton_start
        pushButton_manualReward
        pushButton_emptyFeeders
        pushButton_stop
        pushButton_setup
        pushButton_previous
        pushButton_next
        
        %Trial order
        buttonGroup_trial
        popup_trialType
        
        currentRow = 1;
        nRows = 1;
        UI_GRID_PADDING = 5;
        UI_GRID_SPACING = 5;
        UI_BUTTON_SIZE = [90, 26];
    end%properties (Access = private)
    
    properties (SetObservable)
        isSetup %set after trial was set up (bool)
        isStart %set after trial was started (bool)
        isStop  %set after trial was stopped (bool)
    end%properties (SetObservable)
    
    events
        event_path
        event_start
        event_manualReward
        event_emptyFeeders
        event_stop
        event_setup
        event_previous
        event_next
        event_newTrial
        event_newHeader
    end%events
    
    
    methods (Access = public)
        function obj = MegaPlusMazeUi(varargin)
%             {'Selected', 'Trial', 'StartArm', 'TargetArm', 'ChosenArm','TrialTime','Comment','Outcome','FoodItems'};
            trialColNames = {'Selected', 'Trial', 'Start arm', 'Target arm', 'Chosen arm','Trial time','Outcome','#Food Items', 'Comment'};
            headerColNames = {'Day', 'Experimenter', 'Animal ID', 'Trial Type', 'Number of trials', 'Sensor', 'Doors Config', 'Temp', 'Comment','Depth'};
            data = repmat({false, NaN, NaN, NaN, NaN, NaN,' ',NaN,' '}, obj.nRows, 1);
            headerData = {NaN, '', NaN, 'Manual', NaN, NaN, NaN, NaN, '',''};
            
            if nargin == 1 &&  isgraphics(varargin{1})
                obj.parent = varargin{1};
            else
                obj.parent = figure('Position', [400, 400, 900, 650]);
            end

            %                 'Position', [0, 1.9, .7, .1],...
            obj.headerPanel = uix.Panel(...
                'Parent', obj.parent,...
                'Units', 'normalized',...
                'Position', [0, 0.9, 1, .1],...
                'Padding', obj.UI_GRID_PADDING,...
                'Title', 'Experiment');
            
            obj.headerButtons = uix.VBoxFlex(...
                'Parent', obj.headerPanel,...
                'DividerMarkings', 'off',...
                'Spacing', obj.UI_GRID_SPACING);
            
            obj.buttonGroup_header = uix.HBoxFlex(...
                'Parent', obj.headerButtons,...
                'Padding', obj.UI_GRID_PADDING,...
                'Spacing', obj.UI_GRID_SPACING);
            
            obj.text_pathDescription = uicontrol(...
                'Parent', obj.buttonGroup_header,...
                'Style', 'text',...
                'String', 'Path:',...
                'Enable', 'on');
            
            obj.text_path = uicontrol(...
                'Parent', obj.buttonGroup_header,...
                'Style', 'text',...
                'String', '.',...
                'Enable', 'on');
            
            obj.pushButton_path = uicontrol(...
                'Parent', obj.buttonGroup_header,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_path,...
                'String', '...',...
                'Enable', 'on');
%             'Position', [0, 0.1, 0.5, .1],...
            
            obj.text_timer = uicontrol(...
                'Parent', obj.buttonGroup_header,...
                'Style', 'text',...
                'String', '0',...
                'FontWeight', 'bold',...
                'FontSize', 14,...
                'Enable', 'on');
            
            obj.buttonGroup_header.Widths = [-1,200,-1,-1];
            
            obj.trialPanel = uix.Panel(...
                'Parent', obj.parent,...
                'Units', 'normalized',...
                'Position', [.5, 0, .5, .2],...
                'Padding', obj.UI_GRID_PADDING,...
                'Title', 'Trials');
            
            obj.table_trials = uitable('Parent', obj.parent,...
                'Parent', obj.parent,...
                'Units', 'normalized',...
                'CellEditCallback', @obj.on_table_trials_edit,...
                'Position', [0, .21, 1, .59],...
                'Data', data, 'ColumnName',...
                trialColNames, 'ColumnEditable', true);
            
            obj.table_header = uitable('Parent', obj.parent,...
                'Parent', obj.parent,...
                'Units', 'normalized',...
                'CellEditCallback', @obj.on_table_header_edit,...
                'ColumnWidth', {100,100,100,100,100,100,100},...
                'Position', [0, .81, 1, .07],...
                'Data', headerData, 'ColumnName',...
                headerColNames, 'ColumnEditable', true);
            
            obj.controlPanel = uix.Panel(...
                'Parent', obj.parent,...
                'Units', 'normalized',...
                'Position', [0, 0, .5, .2],...
                'Padding', obj.UI_GRID_PADDING,...
                'Title', 'Control');
            
            obj.controlButtons = uix.VBoxFlex(...
                'Parent', obj.controlPanel,...
                'DividerMarkings', 'off',...
                'Spacing', obj.UI_GRID_SPACING);
            
            obj.buttonGroup_control = uix.HButtonBox(...
                'Parent', obj.controlButtons,...
                'Padding', obj.UI_GRID_PADDING,...
                'Spacing', obj.UI_GRID_SPACING,...
                'ButtonSize', obj.UI_BUTTON_SIZE);
            
            obj.pushButton_setup = uicontrol(...
                'Parent', obj.buttonGroup_control,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_setup,...
                'String', 'Setup',...
                'Enable', 'on');
            
            obj.pushButton_start = uicontrol(...
                'Parent', obj.buttonGroup_control,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_start,...
                'String', 'Start',...
                'Enable', 'on');
            
            obj.pushButton_manualReward = uicontrol(...
                'Parent', obj.buttonGroup_control,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_manualReward,...
                'String', 'Reward',...
                'Enable', 'on');
            
            obj.pushButton_emptyFeeders = uicontrol(...
                'Parent', obj.buttonGroup_header,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_emptyFeeders,...
                'String', 'Empty feeders',...
                'Enable', 'on');
            
            obj.pushButton_stop = uicontrol(...
                'Parent', obj.buttonGroup_control,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_stop,...
                'String', 'Stop',...
                'Enable', 'on');
            
            obj.pushButton_previous = uicontrol(...
                'Parent', obj.buttonGroup_control,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_previous,...
                'String', '<<',...
                'Enable', 'on');
            
            obj.pushButton_next = uicontrol(...
                'Parent', obj.buttonGroup_control,...
                'Style', 'pushbutton',...
                'Callback', @obj.on_next,...
                'String', '>>',...
                'Enable', 'on');
            
            obj.trialButtons = uix.VBoxFlex(...
                'Parent', obj.trialPanel,...
                'DividerMarkings', 'off',...
                'Spacing', obj.UI_GRID_SPACING);
            
            obj.buttonGroup_trial = uix.HButtonBox(...
                'Parent', obj.trialButtons,...
                'Padding', obj.UI_GRID_PADDING,...
                'Spacing', obj.UI_GRID_SPACING,...
                'ButtonSize', obj.UI_BUTTON_SIZE);
            
            obj.popup_trialType = uicontrol(...
                'Parent', obj.buttonGroup_trial,...
                'Style', 'Text',...
                'String', 'Trial type:',...
                'Enable', 'on');
            
            obj.popup_trialType = uicontrol(...
                'Parent', obj.buttonGroup_trial,...
                'Style', 'popup',...
                'Callback', @obj.on_popupChanged,...
                'String', {'Manual', 'Random', 'Place-start arm','Place-start in center','Place-Tmaze','Response','Cue- with starting arm','Cue- starting in center','Cue-Tmaze'},...
                'Enable', 'on'...
                ,'Position',[144,16.5,200,26]);
            
            addlistener(obj, 'isSetup', 'PostSet', @obj.on_property_isSetup);
            addlistener(obj, 'isStart', 'PostSet', @obj.on_property_isStart);
            addlistener(obj, 'isStop', 'PostSet', @obj.on_property_isStop);
        end%function obj = MegaPlusMazeUi(varargin)
        
        %New content for the header table is assigned
        function setHeaderData(obj, data)
            if size(data,2) ~= size(get(obj.table_header, 'Data'),2)
                error('MegaPlusMazeUI: header data size does not match existing table');
            else
                obj.table_header.Data = data;
                obj.headerChanged();
            end
        end
        
        %New content for the trial table is assigned
        function setTrialData(obj, data)
            if size(data,2) ~= size(get(obj.table_trials, 'Data'),2)
                error('MegaPlusMazeUI: trial data size does not match existing table');
            else
                obj.table_trials.Data = data;
                obj.nRows = size(data, 1);
                obj.rowChanged(1);
            end
        end
        
        %Add the 'Time' entry to the current trial/row
        function addTrialTime(obj, t)
            data = get(obj.table_trials, 'Data');
            data{obj.currentRow, 6} = t;
            outcomes(obj);
%             % also add outcome coloumn
%             if data{obj.currentRow, 5} == 5
%                 data{obj.currentRow, 7}=NaN;
%             elseif data{obj.currentRow, 4} == data{obj.currentRow, 5}
%                 data{obj.currentRow, 7}='Correct';
%             else
%                 data{obj.currentRow, 7}='Error';
%             end
            set(obj.table_trials, 'Data', data);
            drawnow();
        end
        
        function outcomes(obj)
              data = get(obj.table_trials, 'Data');
              for i=1:size(data,1)
                  if data{i, 5} == 5
                      data{i, 7}=NaN;
                  elseif data{i, 4} == data{i, 5}
                      data{i, 7}='Correct';
                  elseif isnan(data{i, 5}) || (data{i, 5})==NaN %|| (data{i, 4})=='NaN'
                      data{i, 7}=NaN;
                  else
                      data{i, 7}='Error';
                  end
              end
            set(obj.table_trials, 'Data', data);
             drawnow();
        end
        %update timer text
        function updateTimer(obj, t)
            %             String = ['Tatget: ' obj. num2str(round(t))];
            String = num2str(round(t));
            obj.text_timer.String = num2str(round(t));
            if(t > 600)
                obj.text_timer.String =[String ' OVER-TIME'];
                obj.text_timer.ForegroundColor = 'r';
%                 obj.text_timer.BackgroundColor = 'y';               
            elseif(t > 300)
                obj.text_timer.String =[String ' Lure to center'];
                obj.text_timer.ForegroundColor = [0.9 0.6 0];%orange
            else
                obj.text_timer.ForegroundColor = 'g';
%                 obj.text_timer.BackgroundColor = [0.5 0.5 0.5];
            end
        end
        
        %display current target arm
        function updateTarget(obj, arm)
            obj.text_target.String = num2str(arm);
            
        end
        
        %Return the content of both ui tables as Matlab table
        function [headerTable, trialTable] = getTable(obj, ~, ~)
            headerTable = cell2table(obj.table_header.Data);
            trialTable = cell2table(obj.table_trials.Data);
        end
        
    end%methods (Access = public)
    
    methods (Access = private)
        
        %The current row changed (next or previous trial)
        function trial = rowChanged(obj, row)
            if (row > 0 & row) < (obj.nRows)
                obj.currentRow = row;
            end
            
            data = get(obj.table_trials, 'Data');
            for i = 1:obj.nRows
                data{i, 1} = false;
            end
            data{obj.currentRow, 1} = true;
            set(obj.table_trials, 'Data', data);
            drawnow();
            trial.number = data{obj.currentRow, 2};
            trial.startArm = data{obj.currentRow, 3};
            trial.targetArm = data{obj.currentRow, 4};
            trial.sensor = data{obj.currentRow, 5};
            trial.doorsConfig = data{obj.currentRow, 6};
            userEventData = UserEventData(trial);
            notify(obj, 'event_newTrial', userEventData);
        end
        
        %The content of the current header was changed
        function header = headerChanged(obj)
            data = get(obj.table_header, 'Data');
            header.day = data{1, 1};
            header.experimenter = data{1, 2};
            header.animalID = data{1, 3};
            header.trialType = data{1, 4};
            header.nTrials = data{1, 5};
            header.sensor = data{1, 6};
            header.doorsConfig = data{1, 7};
            header.temp = data{1, 8};
            header.comment = data{1, 9};
            header.depth = data{1, 10};
            
            userEventData = UserEventData(header);
            notify(obj, 'event_newHeader', userEventData);
        end
        
        %Change the color of the button if setup is done
        function on_property_isSetup(obj, ~, ~)
            if(obj.isSetup)
                obj.pushButton_setup.BackgroundColor = [.4, 1, .4];
                obj.pushButton_stop.BackgroundColor = [.5, .5, .5];
            else
                obj.pushButton_setup.BackgroundColor = [1, .4, .4];
            end
        end
        
        %         %Change the color of the button if start is pushed-system is in
        %         trial mode
        function on_property_isStart(obj, ~, ~)
            if(obj.isStart)
                obj.pushButton_start.BackgroundColor = [.4, 1, .4];%green
            else
                obj.pushButton_start.BackgroundColor = [1, .4, .4];%red
            end
        end
        
        function on_property_isStop(obj, ~, ~)
            if(obj.isStop)
                obj.pushButton_stop.BackgroundColor = [.4, 1, .4];%green
                obj.text_timer.String =[obj.text_timer.String ' STOPPED'];
                obj.text_timer.ForegroundColor = 'r';
            else
                obj.pushButton_stop.BackgroundColor = [1, .4, .4];%red
            end
        end
        %Something in the header table UI changed
        function on_table_header_edit(obj, ~, ~)
            obj.headerChanged();
        end
        
        %Something in the trial table UI changed
        function on_table_trials_edit(obj, ~, ~)
            obj.rowChanged(obj.currentRow);
        end
        
        %Browse path button clicked
        function on_path(obj, ~, ~)
            folderName = uigetdir();
            if folderName ~= 0
                obj.text_path.String = folderName;
                userEventData = UserEventData(folderName);
                notify(obj, 'event_path', userEventData);
            end
        end
        
        %Manual reward button clicked
        function on_manualReward(obj, ~, ~)
            notify(obj, 'event_manualReward');
        end
        
        function on_emptyFeeders(obj, ~, ~)
%             if obj.isStart==false%only carry out if not in trial
                notify(obj, 'event_emptyFeeders');
                obj.text_timer.ForegroundColor = 'c';
                obj.text_timer.String = 'Emptying feeders';
%             end
        end
        
        %Start button clicked
        function on_start(obj, ~, ~)
            if obj.isSetup && (isempty(obj.isStart) || ~obj.isStart)
                obj.text_timer.String ='Starting trial, please wait';
                obj.pushButton_start.BackgroundColor =  [0.9 0.6 0];%change color to orange
                pause(5);%wait 5 seconds before starting the trial- to make sure there is enough time between trials
                 obj.text_timer.String = ' Trial Started';
                notify(obj, 'event_start');                
            end
        end
               
        %Stop button clicked
        function on_stop(obj, ~, ~)
            if obj.isSetup && (isempty(obj.isStop) || ~obj.isStop)                            
                outcomes(obj);
%                   obj.text_timer.String = ' Trial Stopped';
                obj.pushButton_stop.BackgroundColor =  [0.9 0.6 0];%change color to orange
                obj.text_timer.String ='Stopping trial, please wait';
                 pause(5)%wait 5 seconds before stopping the trial- to make sure there is enough time between trials
                notify(obj, 'event_stop');                
               
                if obj.currentRow < obj.nRows
                    obj.rowChanged(obj.currentRow + 1);
                end
            end
        end
        
        %Setup button clicked
        function on_setup(obj, ~, ~)
            
            obj.pushButton_setup.BackgroundColor = [0.9 0.6 0];%change color to orange
            obj.text_timer.ForegroundColor = [0.9 0.6 0];
             obj.text_timer.String = ' Setting up, please wait...';
             outcomes(obj);
            notify(obj, 'event_setup');            
            obj.text_timer.String = sprintf('%s%i\n%s%i','Start: ',obj.table_trials.Data{obj.currentRow,3},'Target: ',obj.table_trials.Data{obj.currentRow,4});
            %
            obj.text_timer.ForegroundColor = 'k';
        end
        
        %'Previous' button clicked
        function on_previous(obj, ~, ~)
            notify(obj, 'event_previous');
            if obj.currentRow > 1
                %                 obj.rowChanged(obj.currentRow,4);
                obj.rowChanged(obj.currentRow-1);
            end
        end
        
        %'Next' button clicked
        function on_next(obj, ~, ~)
            notify(obj, 'event_next');
            if obj.currentRow < obj.nRows
                obj.rowChanged(obj.currentRow + 1);
            end
        end
        
        %Popup menu changed
        function on_popupChanged(obj, ~, ~)
            %Add trialtype from popup
            sels = get(obj.popup_trialType, 'String');
            idx  = get(obj.popup_trialType, 'Value');
            trialType = sels{idx};
            
            %Emit two events in order to force regeneration of table
            obj.table_header.Data{obj.currentRow, 4} = 'None';
            obj.headerChanged();
            obj.table_header.Data{obj.currentRow, 4} = trialType;
            obj.headerChanged();
        end
        
    end%methods (Access = private)
    
end%classdef MegaPlusMazeUi < handle
