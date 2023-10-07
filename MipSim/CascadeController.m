classdef CascadeController < Controller 
    properties
        innerLoop;
        outerLoop;
    end
    methods
        function o = CascadeController(innerLoop, outerLoop) 
            o = o@Controller(0);
            o.innerLoop = innerLoop;
            o.outerLoop = outerLoop;
        end
        function u = control(o, t, x)
        end
    end
end