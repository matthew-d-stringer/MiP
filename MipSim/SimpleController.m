classdef SimpleController < Controller
    properties
        func = @(t,x) 0;
    end
    methods
        function o = SimpleController(func)
            o.func = func;
        end
        function u = control(o,t,x)
            u = o.func(t,x);
        end
    end
end