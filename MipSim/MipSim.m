classdef MipSim
    properties 
        r = 6.5e-2/2; %m
        l = 6.5e-2; %m
        Iw = 0.00185; %kg m^2
        Ib = 0.1; %fix
        Mw = 0.032; %kg
        Mb = 0.5; %kg

        StallTorque = 0.157; %Nm
        MaxVel = 104; %rad/s

        % Below are constants used in the differential equation
        K1;
        K2;
        K3;
        K4;

        axisVals;

        % animationSpeed = 0.1;
    end
    methods
        function obj = MipSim(obj) 
            obj.K1 = obj.Iw + (obj.Mw + obj.Mb)*obj.r^2;
            obj.K2 = obj.Mb*obj.r*obj.l;
            obj.K3 = obj.Ib + obj.Mb * obj.l^2;
            obj.K4 = obj.Mb*9.8*obj.l;
            obj.axisVals = [-1 10 -1 10];
        end

        function xyRange(o, xVal)
            o.axisVals = [xVal(1) xVal(2) -1 -1+xVal(2)-xVal(1)];
        end

        % x = [theta, phi, theta', phi']'
        function xDot = f(o,x, voltage) 
            Ct = cos(x(1));
            St = sin(x(1));
            
            torque = o.StallTorque*voltage - o.StallTorque/o.MaxVel * x(4);
            acc = [
                o.K1     o.K2*Ct
                o.K2*Ct  o.K3
            ]\[
                o.K2*St*x(3)^2 + torque
                o.K4*St - torque
            ];

            xDot = [
                x(3:4)
                acc
            ];
        end

        function newX = update(o, x, u, dt) 
            k1 = o.f(x, u);
            k2 = o.f(x + k1 * dt/2, u);
            k3 = o.f(x + k2 * dt/2, u);
            k4 = o.f(x + k3 * dt, u);
            newX = x + (k1 + 2*k2 + 2*k3 + k4)/6 * dt;
        end

        function x = run(o, x0, uFunc, dt, Tf)
            t = 0:dt:Tf;
            x = zeros(4,length(t));
            x(:,1) = x0;
            for ii = 2:length(t)
                x(:,ii) = o.update(x(:, ii-1), uFunc(x(:, ii-1)), dt);
                if isnan(x(:,ii))
                    x = x(:,1:(ii-1));
                    print("Failed because x became NaN")
                    break;
                end
            end
        end

        function [A, B] = hanging(o)
            % Ex' = Gx + Du
            E = [
                o.Mb*o.r*o.l   o.Iw+(o.Mb+o.Mw)*o.r^2
                (o.Ib+o.Mb*o.l^2)    o.Mb*o.r*o.l
            ];
            E = [
                eye(2)  zeros(2)
                zeros(2) E
            ];

            G = [
                zeros(2) eye(2)
                zeros(1,3) -o.StallTorque/o.MaxVel
                o.Mb*9.8*o.l 0 0 o.StallTorque/o.MaxVel
            ]
            D = [ 0; 0; o.StallTorque; -o.StallTorque; ];

            A = E\G;
            B = E\D;
        end

        % xbar = [theta, phi, theta', phi']'
        function A = linearized(o,xbar, ubar)
            Ct = cos(xbar(1));
            St = sin(xbar(1));
            w = (o.K1*o.K3 - o.K2^2*Ct^2)^(-1);

            % DT = Dtheta
            DTw = (2*o.K2^2 * St)*(o.K1*o.K3 - o.K2^2*Ct^2)^(-2);

            F = [
                o.K3       -o.K2*Ct
                -o.K2*Ct   o.K1
            ];
            DTF = [
                0        o.K1*St
                o.K2*St  0
            ];
            u = [
                o.K2 * St * xbar(3)^2 + ubar
                o.K4 * St - ubar
            ];
            DTu = [
                o.K2 * Ct * xbar(3)^2 
                o.K4 * Ct              
            ];

            DTf = DTw*F*u + w*DTF*u + w*F*DTu;
            DPf = [0;0];
            DdTf = w*F*[2*o.K2*St*xbar(3); 0];
            DdPf = [0;0];

            A = [
                zeros(2)  eye(2) 
                [DTf DPf] [DdTf DdPf]
            ];
        end

        function animateWithConstantTorque(o, x0, torque, dt) 
            o.animateWithComputedTorque(x0, @() torque, dt);
        end

        function animateWithComputedTorque(o, x0, torqueFunc, dt)
            Tf = 10;
            t = 0:dt:Tf;
            x = o.run(x0, torqueFunc, dt, Tf);
            axis(o.axisVals);
            o.updateObjs(x(1,1), x(2,1));
            tic;
            for ii=2:length(t)
                clf;
                title("Time = "+t(ii)+" Theta = "+rad2deg(x(1,ii))+" Phi = "+rad2deg(x(2,ii)));
                axis(o.axisVals);
                o.updateObjs(x(1,ii), x(2,ii));
                drawnow;
                pause(dt-toc);
                tic;
            end
        end

        function [wheel, rod, wheelPos] = updateObjs(this, theta, phi)
            y = -this.r*phi;
            z = this.l*cos(theta);
            y2 = y - this.l*sin(theta);

            wheel = rectangle('Position', [y-this.r -this.r 2*this.r 2*this.r], 'Curvature', [1 1]);
            rod = line([y y2], [0 z]);
            wheelPos = line([y y+this.r*cos(phi)], [0 this.r*sin(phi)]);
        end
    end
end