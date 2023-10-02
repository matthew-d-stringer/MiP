clear; close all; clc; 
sim = MipSim;

x0 = [-deg2rad(10) 0 0 0]';

dt = 0.001;

TF = sim.linearizedU2ThetaTF();

load("DiscreteEquivalentDesign.mat");
Controller = ControlSystemDesignerSession.DesignerData.Designs(end);
C = Controller.Data.C;
fprintf("Choosing Design: %s\n", Controller.Name)

H = -1* c2d(C, dt);
% H = tf([1], [1], dt);

feedForwardFunc = @(t,x) (antiGravityFunc(sim,t,x));

controller = ZtransformController(H, [1 0 0 0]);
% controller.addFeedForwardFunc(feedForwardFunc);

CompareFeedForward(sim, x0, dt, 5, controller, feedForwardFunc);

% frames = sim.animateWithComputedU(x0, controller, dt, 3);
% sim.saveAnimation("Animation.avi",frames);

function voltage = antiGravityFunc(sim,t,x)
    Ct = cos(x(1));
    St = sin(x(1));
    torque = (1 + sim.K2/sim.K1 *Ct)^(-1) * (sim.K4 * St - sim.K2^2/sim.K1*Ct*St*x(3)^2);
    voltage = torque/sim.KtR + sim.Kv * x(4);
end