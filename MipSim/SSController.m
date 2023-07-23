classdef SSController < Controller
    properties
        K;
    end
    methods
        function o = SSController(K)
            o.K = K;
        end

        function u = control(o,t,x)
            u = -o.K*x;
        end
    end
end