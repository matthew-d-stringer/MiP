classdef ZtransformController < Controller
    properties         
        prevInput; % Vector from most recent to oldest
        prevOutput;
        InputWeights;
        OutputWeights;

        C; % Matrix for finding y
    end
    methods
        function o = ZtransformController(tf, C)
            [Num, Dem] = tfdata(tf);
            o.InputWeights = Num{1};
            o.OutputWeights = Dem{1};

            o.prevInput = zeros(length(o.InputWeights), 1);
            o.prevOutput = zeros(length(o.OutputWeights),1);

            o.C = C;
        end

        function u = control(o,t,x)
            y = o.C*x;
            % Shift input vector to add new output
            o.prevInput = circshift(o.prevInput,1);
            o.prevInput(1) = y;

            u = o.InputWeights * o.prevInput - o.OutputWeights(1:end-1) * o.prevOutput(1:end-1);
            
            u = u/o.OutputWeights(end);

            % Shift input vector to add new output
            o.prevOutput = circshift(o.prevOutput,1);
            o.prevOutput(1) = u;
        end
    end
end