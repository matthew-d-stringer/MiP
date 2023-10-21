classdef MipSim < AnimatedSim
    properties 
        % Inertia of the wheel
        Iw = 3.815e-5; % kg*m^2

        % Mass of the wheel
        Mw = 0.033*2; % kg

        % Inertia of the body about rotational axis
        Ib = 0.01981217487; % kg*m^2
        
        % Mass of body
        Mb = 0.570; % kg

        % Radius of the wheel
        r  = 0.034; % m

        % Length to center of mass
        l   = 0.319; % m

        % Gravity
        g   = 9.81; % m/s^2

        StallTorque = 0.3924; %Nm
        MaxVel = 19.9; %rad/s

        % Motor Constants (calculated from above)
        % Using equation for motor:
        % V = IR + w Kv and I = Torque * Kt
        KtR; % kt/R = Stall Torque / Voltage
        Kv;  % Kv = Voltage / freeload speed

        % Below are constants used in the differential equation
        % Defined such that the equation becomes:
            % K1 phi'' + K2 cosTheta theta'' - K2 sinTheta (theta')^2 = tau
            % K2 cosTheta phi'' + K3 theta'' - K4 sinTheta = tau
        K1;
        K2;
        K3;
        K4;
    end
    methods
        function o = MipSim() 
            o.Mb = o.Mb - o.Mw;
            o.KtR = o.StallTorque/6; 
            o.Kv = 6/o.MaxVel; 

            o.K1 = o.Iw + (o.Mw + o.Mb)*o.r^2;
            o.K2 = o.Mb*o.r*o.l;
            o.K3 = o.Ib + o.Mb * o.l^2;
            o.K4 = o.Mb*o.g*o.l;
        end

        % x = [theta, phi, theta', phi']'
        function xDot = f(o,x, voltage) 
        % F  Function for differential equation x' = f(x) of the system
        %   xDot = o.F(x,voltage) determines derivative of x from current state and voltage
        % 
        %   From Numerical Renaissance (derived from nonlinear MiP model),
        %   [K2*Ct, K1; K3, K2*Ct] x'' = [K2*St*(theta')^2 + torque; K4*St - torque]

            St = sin(x(1));
            Ct = cos(x(1));

            maxVolts = 6;
            voltage = min(max(voltage, -maxVolts),maxVolts);

            torque = o.KtR*voltage - o.KtR * o.Kv * x(4);
            acc = [
                o.K2*Ct  o.K1
                o.K3     o.K2*Ct
            ]\[
                o.K2*St*x(3)^2 + torque
                o.K4*St - torque
            ];

            xDot = [
                x(3:4)
                acc
            ];
        end

        function [A, B] = linearizedInverted(o)
        % LINEARIZEDINVERTED Returns A,B matrices for linearized system at the 
        %   exact uprighted position.

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
                zeros(1,3) -o.KtR*o.Kv
                o.Mb*9.8*o.l 0 0 o.KtR*o.Kv
            ];
            D = [ 0; 0; o.KtR; -o.KtR; ];

            A = E\G;
            B = E\D;
        end

        function u2t = linearizedU2ThetaTF(o)
        % LINEARIZEDU2THETATF Returns the transfer function for Theta(s)/U(s)
            torque2t = tf([-(o.K2/o.K1+1)], [(-o.K2^2/o.K1 + o.K3) 0 -o.K4]);
            volts2torque = tf([o.Iw 0], [o.Iw/o.KtR o.Kv]);
            u2t = volts2torque * torque2t;
        end

        function t2p = linearizedTheta2Phi(o)
        % LINEARIZEDTHETA2PHI  Returns the transfer function for PHI(s)/THETA(s)
            t2p = minreal(tf([(o.K3 + o.K2) 0 -o.K4], [-(o.K1 + o.K2) 0 0]));
        end

        function TF = linearizedTransferFunction(o)
        % LINEARIZEDTRANSFERFUNCTION Returns transfer function for linearized 
        %   system at the exact uprighted position.

            % [A,B] = o.linearizedInverted();
            % C = [1 0 0 0 ];
            % s = tf('s');
            % TF = C * inv(s*eye(4) - A) * B;

            TF = tf([o.K1],[(o.K2^2 - o.K1*o.K3) 0 o.K1*o.K4]);
        end

        % xbar = [theta, phi, theta', phi']'
        function [A, B] = linearized(o,xbar, ubar)
        % LINEARIZED Computes linearized version of system using Jacobian
        %   Ideally used to find linear forms at equilibrium points
        %   A \approx Dx f(xbar, ubar)
        %   B \approx Du f(xbar, ubar)

            Ct = cos(xbar(1));
            St = sin(xbar(1));
            w = (o.K2^2 * Ct^2 - o.K1*o.K3)^(-1);

            % DT = Dtheta
            DTw = (-2*o.K2^2 * St * Ct)*(o.K2^2 * Ct^2 - o.K1*o.K3)^(-2);

            F = [
                o.K2*Ct -o.K1
                -o.K3   o.K2*Ct
            ];
            DTF = [
                -o.K2*St 0
                0        -o.K2*St
            ];
            u = [
                o.K2 * St * xbar(3)^2 - o.KtR * o.Kv * xbar(4) + o.KtR*ubar
                o.K4 * St             + o.KtR * o.Kv * xbar(4) - o.KtR*ubar
            ];
            DTu = [
                o.K2 * Ct * xbar(3)^2 
                o.K4 * Ct 
            ];

            DTf = DTw*F*u + w*DTF*u + w*F*DTu;
            DPf = [0;0];
            DdTf = w*F*[2*o.K2*St*xbar(3); 0];
            DdPf = w*F*[
                -o.KtR * o.Kv
                 o.KtR * o.Kv
            ];

            A = [
                zeros(2)  eye(2) 
                [DTf DPf] [DdTf DdPf]
            ];

            B = w*F*[
                o.KtR
                -o.KtR
            ];
            B = [0;0;B];
        end


        function [wheel, rod, wheelPos] = updateObjs(this, x)
        % UPDATEOBJS Plots pendulum animation with given theta and phi
            theta = x(1);
            phi = x(2);
            
            l = 5;
            y = -this.r*phi;
            z = l*cos(theta);
            y2 = y - l*sin(theta);

            r = 1;

            axis(this.axisVals);

            wheel = rectangle('Position', [y-r -r 2*r 2*r], 'Curvature', [1 1]);
            rod = line([y y2], [0 z]);
            wheelPos = line([y y+r*cos(phi)], [0 r*sin(phi)], 'Color', 'k');
        end
    end
end