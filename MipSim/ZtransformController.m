classdef ZtransformController < Controller
    properties         
        prevInput; % Vector from most recent to oldest
        prevOutput;
        InputWeights;
        OutputWeights;

        C; % Matrix for finding y

        feedForwardFunc = @(t,x) 0;
    end
    methods
        function o = ZtransformController(tf, C)
            o = o@Controller(tf.Ts);
            [Num, Dem] = tfdata(tf);
            o.InputWeights = Num{1};
            o.OutputWeights = Dem{1};

            o.prevInput = zeros(length(o.InputWeights), 1);
            o.prevOutput = zeros(length(o.OutputWeights)-1,1);

            o.C = C;
        end

        function reset(o)
            o.prevInput = zeros(length(o.InputWeights), 1);
            o.prevOutput = zeros(length(o.OutputWeights)-1,1);
        end

        function addFeedForwardFunc(o, feedForwardFunc)
            o.feedForwardFunc = feedForwardFunc;
        end

        function removeFeedForwardFunc(o)
            o.feedForwardFunc = @(t,x) 0;
        end

        function u = control(o,t,x)
            y = o.C*x;
            % Shift input vector to add new output
            o.prevInput = circshift(o.prevInput,1);
            o.prevInput(1) = y;

            u = o.InputWeights * o.prevInput - o.OutputWeights(2:end) * o.prevOutput;
            
            u = u/o.OutputWeights(1);

            % Shift input vector to add new output
            o.prevOutput = circshift(o.prevOutput,1);
            o.prevOutput(1) = u;

            u = u + o.feedForwardFunc(t,x);
        end
    end
end