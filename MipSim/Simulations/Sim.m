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

        function [t, x, u] = run(o, x0, controller, dt, Tf)
        % RUN Returns all following states from initial state x0 until final time
        %   Tf using inputs calculated from controller.timedControl(t,x) from 
        %   Controller abstract class
        %   
        %   o.RUN([0.1;0;0;0], SSController(K); 0.01, 10) Simulates system from x0 at t = 0
        %   to t = 10 using the control law u=-Kx
            arguments
                o
                x0
                controller Controller
                dt
                Tf
            end
            t = 0:dt:Tf;
            x = zeros(4,length(t));
            u = zeros(1,length(t));
            x(:,1) = x0;
            controller.reset();
            for ii = 2:length(t)
                u(:,ii) = controller.timedControl(t(ii), x(:, ii-1));
                x(:,ii) = o.update(x(:, ii-1), u(:,ii), dt);
                if isnan(x(:,ii))
                    x = x(:,1:(ii-1));
                    disp('Failed because x became NaN')
                    break;
                end
            end
        end

    end
end