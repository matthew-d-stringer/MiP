clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(1) 0 0 0]';

TF = sim.linearizedU2ThetaTF();

load("ControlSystemDesignerSession.mat");
C = ControlSystemDesignerSession.DesignerData.Designs(1).Data.C;
fprintf("Choosing Design: %s\n", ControlSystemDesignerSession.DesignerData.Designs(end).Name)

dt = 0.001;

H = c2d(C,dt);

feedForwardFunc = @(t,x) (antiGravityFunc(sim,t,x));

controller = ZtransformController(H, [1 0 0 0]);

CompareFeedForward(sim, x0, dt, 5, controller, feedForwardFunc);

sim.xyRange([-4 10], -1);

% figure;
% frames = sim.animateWithComputedU(x0, controller, dt, 3);
% sim.saveAnimation('Animation.avi', frames);

function voltage = antiGravityFunc(sim,t,x)
    Ct = cos(x(1));
    St = sin(x(1));
    torque = (1 + sim.K2/sim.K1 *Ct)^(-1) * (sim.K4 * St - sim.K2^2/sim.K1*Ct*St*x(3)^2);
    voltage = torque/sim.KtR + sim.Kv * x(4);
end