classdef (Abstract) Controller < handle
    methods (Abstract)
        u = control(o,t,x)
    end
end