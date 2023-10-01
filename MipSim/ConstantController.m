classdef ConstantController < Controller
    properties 
        u;
    end
    methods
        function o = ConstantController(u)
            o.u = u;
        end

        function u = control(o,t,x)
            u = o.u;
        end
    end
end