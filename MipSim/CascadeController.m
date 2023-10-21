classdef CascadeController < Controller 
    properties
        innerLoop;
        outerLoop;
    end
    methods
        function o = CascadeController(innerLoop, outerLoop) 
            arguments
                innerLoop ZtransformController
                outerLoop ZtransformController
            end
            o = o@Controller(0);
            o.innerLoop = innerLoop;
            o.outerLoop = outerLoop;
        end
        function u = control(o, t, x)
            o.innerLoop.setSetpoint(o.outerLoop.timedControl(t,x));
            u = o.innerLoop.timedControl(t,x);
        end
    end
end