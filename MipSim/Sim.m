classdef (Abstract) Sim < handle
    methods (Abstract)
        x = f(o,x,u)
    end
    methods 
        function newX = update(o, x, u, dt) 
        % UPDATE Uses RK-4 to determine next state x from current state x and input u
        %   dt seconds ahead from now.
            k1 = o.f(x, u);
            k2 = o.f(x + k1 * dt/2, u);
            k3 = o.f(x + k2 * dt/2, u);
            k4 = o.f(x + k3 * dt, u);
            newX = x + (k1 + 2*k2 + 2*k3 + k4)/6 * dt;
        end
    end
end