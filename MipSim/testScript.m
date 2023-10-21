clear; close all; clc; 
sim = MipSim;

x0 = [-deg2rad(15) 0 0 0]';

dt = 0.001; % Sample time of controller
simDt = 0.0005; % Sample time of simulation

G1 = sim.linearizedU2ThetaTF();
G2 = sim.linearizedTheta2Phi(); 

C1 = LoadControllerFromMatFile("DiscreteEquivalentDesign.mat");
H1 = c2d(C1, dt);

C2 = LoadControllerFromMatFile("OuterloopController.mat");
H2 = -c2d(C2, dt*4);

% rlocus(G2)
% H = tf([1], [1], dt);
    
feedForwardFunc = @(t,x) (sim.antiGravityFunc(t,x));

innerLoop = ZtransformController(H1, [1 0 0 0]);
outerLoop = ZtransformController(H2, [0 1 0 0]);
innerLoop.addFeedForwardFunc(feedForwardFunc);

% controller = CascadeController(innerLoop, outerLoop);
% controller = innerLoop;

% CompareFeedForward(sim, x0, dt, 5, controller, feedForwardFunc);

% CompareDifferentThetaErrors(sim, 0:5:15, simDt, 5, controller);

frames = sim.animateWithComputedU(x0, innerLoop, simDt, 5);
% sim.saveAnimation("Animation.avi",frames);
