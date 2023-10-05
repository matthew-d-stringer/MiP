classdef SimpleController < Controller
    properties
        func = @(t,x) 0;
    end
    methods
        function o = SimpleController(func, dt)
            o.func = func;
            o.dt = dt;
        end
        function u = control(o,t,x)
            u = o.func(t,x);
        end
    end
end