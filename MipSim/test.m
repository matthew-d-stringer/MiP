clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(10) 10 0 0]';
    
[A,B] = sim.linearizedInverted();
    
[K,S,P] = lqr(A, B, diag([1 0.2 1 1]), 2);
% [K,S,P] = lqr(A, B, diag([1 0.00001 1 0.00001]), 2);

sim.xyRange([-4 10], -1);

% sim.updateObjs(x0(1), x0(2));

controller = SSController(K);

frames = sim.animateWithComputedU(x0, controller, 0.001, 3);
% sim.saveAnimation('Animation.avi', frames);

% TF = sim.linearizedTransferFunction()

