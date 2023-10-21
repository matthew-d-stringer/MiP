classdef (Abstract) Controller < handle
    properties 
        pT = 0;
        pU = 0;
        dt = 0.01;
    end
    methods (Abstract)
        u = control(o,t,x)
    end
    methods
        function o = Controller(dt)
            o.dt = dt;
        end
        function reset(o) 
            o.pT = 0;
            o.pU = 0;
        end
        function u = timedControl(o, t, x) 
            if o.pT + o.dt <= t
                u = o.control(t,x);
                o.pU = u;
                o.pT = t;
            else
                u = o.pU;
            end
        end
    end
end