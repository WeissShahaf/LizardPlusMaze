
classdef MegaPlusMaze < handle
    
    properties (Access = private)
        ui;
        mainWindow
        arduinoSerial = ArduinoSerial('COM3');%COM3 on pylon PC /COM5 on plexon PC
        %arduinoSerial = ArduinoSerial('/dev/ttyACM0');
        currentTrial;
        currentHeader;
        currentPath = '.';
        trialTimer = timer('ExecutionMOde', 'fixedRate', 'Period', 1);
        
        datePrefix %Generated when header is edited
        
        %Arduino events
        EVENT_HANDSHAKE = 0;
        EVENT_SELECT_START_ARM = 1;
        EVENT_SELECT_TARGET_ARM = 2;
        EVENT_OPEN_DOOR = 3;
        EVENT_CLOSE_DOOR = 4;
        EVENT_SELECT_SENSOR = 5;
        EVENT_START_TRIAL = 6;
        EVENT_STOP_TRIAL = 7;
        EVENT_DISABLE_ARMS = 8;
        EVENT_DISABLE_SENSORS = 9;
        EVENT_ERROR = 10;
        EVENT_COUNT = 11;
        EVENT_TASK_STATE1_ON = 12;
        EVENT_TASK_STATE2_ON = 13;
        EVENT_TASK_STATE1_OFF = 14;
        EVENT_TASK_STATE2_OFF = 15;
        EVENT_TASK_STATE2 = 16;
        
    end
    
    
    methods
        function obj = MegaPlusMaze(varargin)
            %Create main window here
            obj.mainWindow = figure('Position', [400, 400, 900, 650],...
                'CloseRequestFcn', @obj.on_windowCloseRequested);
            
            %Pass handle of main Window to UI as a parent
            obj.ui = MegaPlusMazeUi(obj.mainWindow);
            
            obj.trialTimer.TimerFcn = @obj.on_timerUpdate;
            
            %Connect events from UI
            addlistener(obj.ui, 'event_start', @obj.on_start);
            addlistener(obj.ui, 'event_manualReward', @obj.on_manualReward);
            addlistener(obj.ui, 'event_emptyFeeders', @obj.on_emptyFeeders);
            addlistener(obj.ui, 'event_stop', @obj.on_stop);
            addlistener(obj.ui, 'event_setup', @obj.on_setup);
            addlistener(obj.ui, 'event_newTrial', @obj.on_event_newTrial);
            addlistener(obj.ui, 'event_newHeader', @obj.on_event_newHeader);
            addlistener(obj.ui, 'event_path', @obj.on_event_path);
            
            %Initialize header and table
            s = date;
            
            %Set default header values
            %Day, Experimenter, AnimalID, Task Type, Number of trials,
            %Sensor, Doors Config, Temp, Comment
            obj.ui.setHeaderData({s, '', NaN, 'Manual', 20, 1, 2, NaN, '',''});
            
            %Generate a table (defaults to manual)
            obj.generateTrialTable(); %Generate trial sequence
            
        end
        
        %Object destructor
        function delete(obj)
            disp('MegaPlusMaze: delete');
            stop(obj.trialTimer);
            delete(obj.arduinoSerial);
        end
        %The manual reward button was pressed
        function on_manualReward(obj, ~, ~)
            obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, 0);
            pause(1);
            obj.arduinoSerial.write(obj.EVENT_SELECT_SENSOR, obj.currentHeader.sensor);
        end
        % the empty feeders button was pressed
        function on_emptyFeeders(obj, ~, ~)
            obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, 0);
            obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,5);
            for j=1:26
                for i= 1:4
                    obj.arduinoSerial.write(obj.EVENT_SELECT_TARGET_ARM,i);
                    pause(1);
                end
                
            end
            obj.arduinoSerial.write(obj.EVENT_SELECT_TARGET_ARM,...
                obj.currentTrial.targetArm)
            obj.arduinoSerial.write(obj.EVENT_SELECT_SENSOR, obj.currentHeader.sensor);
            
        end%function on_emptyFeeders(obj, ~, ~)
        
        
        %%%%%%%         %The setup button was pressed %%%%%%%%%%%%
        function on_setup(obj, ~, ~)
            obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,...
                obj.currentTrial.startArm);%select start arm
            % % %             %Trial setup   % % %
                        %1st trial only- disable all arms
            if obj.currentTrial.number==1 %for first trial
                obj.arduinoSerial.write(obj.EVENT_DISABLE_ARMS, 0);
            end

            %first open all doors
            for i=1:4
                obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR, i); %open all doors
            end
            pause(2);
            closeDoorsConfig(obj)% close doors according to door configuration
            pause(1);
            obj.ui.isSetup = true;
            obj.ui.isStart = false;
            obj.ui.isStop = true;%if setup is pressed again- send stop TTL
            
        end
        
        function closeDoorsConfig(obj, ~, ~)
            %close doors according to door config and task type
            switch obj.currentHeader.doorsConfig%check Doors Config
                
                case 0%manual open field task
                    obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, 0);
                    for i = 1:4
                        obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,i);
                    end
                    
                case 1%close all nontarget arms ,open target arm
                    obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,...
                        obj.currentTrial.targetArm);
                    for i = 1:4
                        if(i ~= obj.currentTrial.targetArm)
                            obj.arduinoSerial.write(obj.EVENT_CLOSE_DOOR,i);
                        end
                    end
                    
                case 2%close only start arm
                    %close start arm
                    obj.arduinoSerial.write(obj.EVENT_CLOSE_DOOR,...
                        obj.currentTrial.startArm);
                    
                case {3,4}%start in center - close all doors
                    for i = 1:4  %close all doors
                        obj.arduinoSerial.write(obj.EVENT_CLOSE_DOOR,i);
                    end
                    
                case 5%% tamze: close start arm and opposite arm
                    %close doors 1,3
                    obj.arduinoSerial.write(obj.EVENT_CLOSE_DOOR,  3);
                    obj.arduinoSerial.write(obj.EVENT_CLOSE_DOOR,  1);
                    
            end%switch doorsConfig%check Doors Config
        end
        
        %The start button was pressed
        function on_start(obj, ~, ~)
            if ~obj.ui.isStart
                %set sensors
                obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, obj.currentTrial.startArm);
                obj.arduinoSerial.write(obj.EVENT_DISABLE_ARMS, obj.currentTrial.startArm);
                sensor = obj.currentHeader.sensor;
                obj.arduinoSerial.write(obj.EVENT_SELECT_SENSOR, sensor);
                
                %start timers
                tic
                stop(obj.trialTimer);
                start(obj.trialTimer);
                %Trial Start
                obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,...
                    obj.currentTrial.startArm);
                obj.arduinoSerial.write(obj.EVENT_SELECT_TARGET_ARM,...
                    obj.currentTrial.targetArm);
                
                obj.arduinoSerial.write(obj.EVENT_START_TRIAL, 0);
                
                %                 open relevent doors
                openDoorsConfig(obj);
             
                obj.ui.isStart = true;
                obj.ui.isStop = false;
            end
        end%function on_start(obj, ~, ~)
        
        function openDoorsConfig(obj,~,~)
            switch obj.currentHeader.doorsConfig

                case 0%manual open field task
                    obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, 0);
                    for i = 1:4
                        obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,i);
                    end
                    
                case {1,2}%open start arm only
                    obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,  obj.currentTrial.startArm);
                    
                case 3 % starting from center- open 2,4 only
                    for i=[2,4]
                        obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,  i);
                    end
                    
                case 4% open all doors
                    for i=1:4
                        obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,  i);
                    end
                    
                case 5 % T maze -  open 2,4, then start arm
                    obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,  2);
                    obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,  4);
                    %                        obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, obj.currentTrial.startArm+1);
                    obj.arduinoSerial.write(obj.EVENT_OPEN_DOOR,  obj.currentTrial.startArm);
                    
            end%switch obj.currentHeader.doorsConfig
        end%function openDoorsConfig(obj,~,~)
        
        %The stop button was pressed
        function on_stop(obj, ~, ~)
            if ~obj.ui.isStop
                obj.arduinoSerial.write(obj.EVENT_STOP_TRIAL, 0);%send TTL
                t = toc;
                stop(obj.trialTimer);%stop trial timer
                obj.ui.addTrialTime(t);%update time slot
                obj.arduinoSerial.write(obj.EVENT_DISABLE_ARMS, 0);%disable arms
                obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, 0);%disable sensors
                % %
                %                 for i=1:4
                %                     obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,i);
                %                 end
                % % %
                
                
                obj.saveTables();%update tables
                obj.ui.isStart = false;
                obj.ui.isStop = true;
            end
        end%function on_stop(obj, ~, ~)
        
        
        
        %A new trial was selected
        function on_event_newTrial(obj, ~, event)
            obj.currentTrial = event.userdata;
            if obj.ui.isStart == false
                obj.ui.isSetup = false;
            end
        end
        
        %The header was changed from the GUI
        function on_event_newHeader(obj, ~, event)
            newHeader = event.userdata;
            if isempty(obj.currentHeader) ||...
                    obj.currentHeader.nTrials ~= newHeader.nTrials ||...
                    ~strcmp(obj.currentHeader.trialType, newHeader.trialType)
                obj.currentHeader = newHeader;
                obj.generateTrialTable();
            else
                obj.currentHeader = newHeader;
            end
            obj.ui.isSetup = false;
            
            obj.datePrefix = datestr(datetime('now'), 'yyyy-mm-dd-HH-MM-SS');
        end
        
        %A new path was selected
        function on_event_path(obj, ~, event)
            obj.currentPath = event.userdata;
        end
        
        function on_timerUpdate(obj, ~, ~)
            if obj.ui.isStart
                t = toc;
                obj.ui.updateTimer(t);
            end
        end
        
        %The Main Window requested to close
        function on_windowCloseRequested(obj, ~, ~)
            
            if isgraphics(obj.mainWindow, 'figure')
                obj.saveTables();
                delete(obj.mainWindow);
            end
            
            obj.delete();
        end
        
        %Save header and trial tables
        function saveTables(obj)
            if ~isnan(obj.currentTrial.startArm)
                [headerTable, trialTable] = obj.ui.getTable();%get current table contents
                
                %asign file name
                shortDate=obj.datePrefix(1:10); shortTime=obj.datePrefix(12:19);
                shortDate(ismember(shortDate,'-'))=[]; shortTime(ismember(shortTime,'-'))='_';
                fullFileName = [obj.currentPath,filesep,shortDate,filesep,shortTime,...
                    '_','behavior',filesep,num2str(obj.currentHeader.animalID),...
                    '_', shortTime, '.mat'];
                %create data folders
                new_dir=[obj.currentPath,filesep,shortDate,filesep,shortTime,'_'];
                if ~isfolder([new_dir,'behavior'])
                    mkdir ([new_dir,'behavior']);
                    mkdir ([new_dir,'cheetah']);
                    mkdir ([new_dir,'videos']);
                end
                %asign table variable names
                trialTable.Properties.VariableNames={'Selected', 'Trial', 'StartArm', 'TargetArm', 'ChosenArm', 'TrialTime','Outcome','FoodItems', 'Comment'};
                %                 trialTable.Properties.VariableNames={'Selected', 'Trial', 'StartArm', 'TargetArm', 'ChosenArm','TrialTime','Comment'};
                headerTable.Properties.VariableNames={'Day', 'Experimenter', 'AnimalID', 'TrialType', 'NumberOfTrials', 'Sensor', 'doorsConfig', 'Temp', 'Comment','Depth'};
                %save file
                save(fullFileName, 'headerTable', 'trialTable');
                disp(['Wrote: ', fullFileName]);
            end
        end%function saveTables(obj)
        
        %Generate an empty trial list
        function generateTrialTable(obj)
            
            data = repmat({false, NaN, NaN, NaN, NaN, NaN,' ',NaN,' '}, obj.currentHeader.nTrials, 1);
            startArm = nan(1, obj.currentHeader.nTrials);
            targetArm = nan(1, obj.currentHeader.nTrials);
            %Add incrementing trial number
            for i = 1:obj.currentHeader.nTrials
                data{i, 2} = i;
            end
            
            switch obj.currentHeader.trialType
                %  // EVENT_TASK_STATE1 LOW + EVENT_TASK_STATE2 LOW ->place /response with a starting arm
                % // EVENT_TASK_STATE1 HIGH + EVENT_TASK_STATE2 LOW ->place /response without a starting arm
                % // EVENT_TASK_STATE1 LOW + EVENT_TASK_STATE2 HIGH ->visual cue with starting arm
                % // EVENT_TASK_STATE1 HIGH + EVENT_TASK_STATE2 HIGH ->visual cue without a starting arm
                %specific trial ordering for type 1
                case 'Manual'
                    obj.currentHeader.doorsConfig=0;
                    obj.arduinoSerial.write(obj.EVENT_DISABLE_SENSORS, 0);
                    for i = 1:4
                        obj.arduinoSerial.write(obj.EVENT_SELECT_START_ARM,i);
                    end
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Random'
                    for i=1:4%switch all arms to place/response task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_ON,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_OFF,i);
                    end
                    
                    %create start and target vectors
                    idx=randi(4,obj.currentHeader.nTrials);
                    values=idx(randi(obj.currentHeader.nTrials),:);
                    startArm=values;
                    clear values
                    
                    targetArm = nan(1, obj.currentHeader.nTrials);
                    for i=1:obj.currentHeader.nTrials
                        Isit=true;
                        while Isit
                            values=randi(4);
                            if values~=startArm(i)
                                targetArm(i)=values;
                                Isit=false;
                            else
                                Isit=true;
                            end
                        end
                    end
                    
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Place-start arm'
                    %create start and target vectors
                    startArm = nan(1, obj.currentHeader.nTrials + 1);
                    values = [1, 3, 4];
                    count = 1;
                    for i=1:4%switch all arms to response task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_ON,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_OFF,i);
                    end
                    
                    for i = 1 : ceil((obj.currentHeader.nTrials + 1)/3)%21 values
                        for j=1:randi(10)
                            idx = randperm(3);
                        end
                        startArm(count : count + 2) = values(idx);
                        count = count + 3;
                    end
                    
                    startArm(obj.currentHeader.nTrials + 1 : end) = [];%remove the 21st value
                    targetArm = ones(obj.currentHeader.nTrials, 1) .* 2;%target always arm 2
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Place-start in center'
                    %change Doors Config to 4
                    obj.currentHeader.doorsConfig=4;
                    %update ui header
                    obj.ui.setHeaderData({obj.currentHeader.day,obj.currentHeader.experimenter,...
                        obj.currentHeader.animalID, obj.currentHeader.trialType,...
                        obj.currentHeader.nTrials, obj.currentHeader.sensor,...
                        obj.currentHeader.doorsConfig,obj.currentHeader.temp,...
                        obj.currentHeader.comment,obj.currentHeader.depth});
                    
                    %create start and target vectors
                    startArm = nan(1, obj.currentHeader.nTrials);
                    startArm(:)=1;
                    
                    targetArm = ones(obj.currentHeader.nTrials, 1) .* 2;%target always arm 2
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    
                case 'Place-Tmaze'
                    %change Doors Config to 5
                    obj.currentHeader.doorsConfig=5;
                    %update ui header
                    obj.ui.setHeaderData({obj.currentHeader.day,obj.currentHeader.experimenter,...
                        obj.currentHeader.animalID, obj.currentHeader.trialType,...
                        obj.currentHeader.nTrials, obj.currentHeader.sensor,...
                        obj.currentHeader.doorsConfig,obj.currentHeader.temp,...
                        obj.currentHeader.comment,obj.currentHeader.depth});
                    %create start and target vectors
                    startArm = nan(1, obj.currentHeader.nTrials + 1);
                    values = [1, 3];
                    count = 1;
                    for i=1:4%switch all arms to response task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_ON,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_OFF,i);
                    end
                    
                    for i = 1 : ceil((obj.currentHeader.nTrials + 1)/3)%21 values
                        for j=1:randi(10)
                            idx = randperm(2);
                            idx2 = randperm(2);
                        end
                        startArm(count : count + 2) = values([idx,idx2(2)]);
                        count = count + 3;
                    end
                    
                    startArm(obj.currentHeader.nTrials + 1 : end) = [];%remove the 21st value
                    targetArm = ones(obj.currentHeader.nTrials, 1) .* 2;%target always arm 2
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Response'
                    %create start and target vectors
                    startArm=nan(1,obj.currentHeader.nTrials);
                    targetArm=nan(1,obj.currentHeader.nTrials);
                    values = 1:4;
                    count = 1;
                    for i=1:4%switch all arms to response task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_ON,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_OFF,i);
                    end
                    
                    for i=1: ceil(obj.currentHeader.nTrials/4)%20 values
                        for j=1:randi(10)
                            idx = randperm(4);
                        end
                        startArm(count : count + 3) = values(idx);
                        targetArm(count : count + 3) = values(idx(end : -1 : 1));
                        targetArm(targetArm<1)=4;
                        
                        
                        count=count+4;
                    end
                    startArm(obj.currentHeader.nTrials + 1 : end)=[];%remove the 21st value
                    targetArm = startArm - 1; %counter clock wise direction target arms vector
                    targetArm(targetArm==0)=4;%fix for arm 1->4
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Cue- with starting arm'
%                      obj.currentHeader.doorsConfig=5;
                    %create start and target vectors
                    startArm = nan(1, obj.currentHeader.nTrials);
                    targetArm = nan(1, obj.currentHeader.nTrials);
                    for i=1:4%switch all arms to cue task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_OFF,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_ON,i);
                    end
                    
                    
                    values = [2, 4];
                    count = 1;
                    for i = 1:obj.currentHeader.nTrials
                        for j=1:randi(10)
                            idx = randperm(2);
                        end
                        startArm(count : count + 1) = values(idx);
                        targetArm(count : count + 1) = values(idx(end : -1 : 1));
                        count = count + 2;
                    end
                    
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Cue- starting in center'
                    
                    %change Doors Config to 3
                    obj.currentHeader.doorsConfig=3;
                    %update ui header
                    obj.ui.setHeaderData({obj.currentHeader.day,obj.currentHeader.experimenter,...
                        obj.currentHeader.animalID, obj.currentHeader.trialType,...
                        obj.currentHeader.nTrials, obj.currentHeader.sensor,...
                        obj.currentHeader.doorsConfig,obj.currentHeader.temp,...
                        obj.currentHeader.comment,obj.currentHeader.depth});
                    
                    %create start and target vectors
                    startArm = nan(1, obj.currentHeader.nTrials);
                    startArm(:)=5;
                    
                    targetArm = nan(1, obj.currentHeader.nTrials);
                    for i=1:4%switch all arms to cue task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_OFF,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_ON,i);
                    end
                    
                    values = [2, 4];
                    count = 1;
                    for i = 1:obj.currentHeader.nTrials
                        for j=1:randi(10)
                            idx = randperm(2);
                        end
                        
                        targetArm(count : count + 1) = values(idx(end : -1 : 1));
                        count = count + 2;
                    end
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                case 'Cue-Tmaze'
                    %change Doors Config to 5
                    obj.currentHeader.doorsConfig=5;
                    %update ui header
                    obj.ui.setHeaderData({obj.currentHeader.day,obj.currentHeader.experimenter,...
                        obj.currentHeader.animalID, obj.currentHeader.trialType,...
                        obj.currentHeader.nTrials, obj.currentHeader.sensor,...
                        obj.currentHeader.doorsConfig,obj.currentHeader.temp,...
                        obj.currentHeader.comment,obj.currentHeader.depth});
                    
                    %create start and target vectors
                    startArm = nan(1, obj.currentHeader.nTrials);
                    targetArm = nan(1, obj.currentHeader.nTrials);
                    for i=1:4%switch all arms to cue task
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE1_OFF,i);
                        obj.arduinoSerial.write(obj.EVENT_TASK_STATE2_ON,i);
                    end
                    
                    
                    values = [1, 3];
                    values2 = [2, 4];
                    count = 1;
                    for i = 1:obj.currentHeader.nTrials
                        for j=1:randi(10)
                            idx = randperm(2);
                            idx2 = randperm(2);
                            
                        end
                        startArm(count : count + 1) = values(idx);
                        targetArm(count : count + 1) = values2(idx2);
                        count = count + 2;
                    end
                    
                    
                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            end%switch
            
            %asign to table data
            for i = 1 : obj.currentHeader.nTrials
                data{i, 3} = startArm(i);
                data{i, 4} = targetArm(i);
            end
            obj.ui.setTrialData(data);
            obj.ui.isSetup = false;
        end%function generateTrialTable(obj)
        
    end%methods
    
end%classdef



