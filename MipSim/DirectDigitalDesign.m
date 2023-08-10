clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(15) 0 0 0]';
    
dt = 0.001;

TF = sim.linearizedU2ThetaTF();
discreteTF = c2d(TF, dt);

load("DigitalFirstDesignSession.mat");
H = ControlSystemDesignerSession.DesignerData.Designs(end).Data.C;
fprintf("Choosing Design: %s\n", ControlSystemDesignerSession.DesignerData.Designs(end).Name)

controller = ZtransformController(H, [1 0 0 0]);

feedForwardFunc = @(t,x) (antiGravityFunc(sim,t,x));

controller.addFeedForwardFunc(feedForwardFunc);

% CompareFeedForward(sim, x0, dt, 5, controller, feedForwardFunc);
% CompareDifferentThetaErrors(sim, [1 3:3:18], dt, 5, controller);

% sim.xyRange([-4 10], -1);
% frames = sim.animateWithComputedU(x0, controller, dt, 3);
% sim.saveAnimation('Animation.avi', frames);

function voltage = antiGravityFunc(sim,t,x)
    Ct = cos(x(1));
    St = sin(x(1));
    torque = (1 + sim.K2/sim.K1 *Ct)^(-1) * (sim.K4 * St - sim.K2^2/sim.K1*Ct*St*x(3)^2);
    voltage = torque/sim.KtR + sim.Kv * x(4);
end