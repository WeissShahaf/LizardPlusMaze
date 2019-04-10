classdef ArduinoSerial < handle
    properties
        %serialPort = '/dev/ttyACM0'; %Linux
        serialPort = 'COM3'; %Windows
        baudRate = 9600;
        terminator = 'LF'
        serialObj;
        
        pollDelay = 0;
        pollPeriod = 0.01;
        
        DEBUG = false;
    end
    
    properties (Access = private)
        pollingTimer;
    end
    
    methods
        function obj = ArduinoSerial(varargin)
            if nargin == 1
                obj.serialPort = varargin{1};
            end
            if ~obj.DEBUG
                obj.serialObj = serial(obj.serialPort, 'BaudRate', obj.baudRate);
                set(obj.serialObj, 'terminator', obj.terminator);    % define the terminator for println);
                fopen(obj.serialObj);
            end
            disp('Arduino initialized');
            obj.pollingTimer = timer(...
                'StartDelay', obj.pollDelay,...
                'Period', obj.pollPeriod,...
                'ExecutionMode', 'fixedSpacing',...
                'TimerFcn', @obj.fcnTimerCallback_PollRead);
            start(obj.pollingTimer);
        end
        
        function delete(obj)
            disp('ArduinoSerial: delete');
            stop(obj.pollingTimer);
            if(isobject(obj.serialObj))
                disp('closing serial port');
                fclose(obj.serialObj);
            end
        end
        
        function write(obj, code, data)
            if obj.DEBUG
                fprintf('%d,%d\n', code, data);
            else
                fprintf(obj.serialObj, sprintf('%d,%d\n', code, data));
            end
        end
        
    end

    methods (Access = private)
        function fcnTimerCallback_PollRead(obj, ~, ~)
            if ~obj.DEBUG
                if(obj.serialObj.BytesAvailable)
                    input = fscanf(obj.serialObj);
                    disp(input);
                end
            end
        end
    end

end




