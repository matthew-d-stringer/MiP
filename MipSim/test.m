clear; close all; clc;
sim = MipSim;

x0 = [1.5 0 0 0]';
    
[A,B] = sim.linearizedInverted();

[K,S,P] = lqr(A, B, eye(4), 2);

sim.xyRange([-4 10], -1);

frames = sim.animateWithComputedU(x0, @(x) -K*x, 0.01, 4);
% sim.saveAnimation('Animation.avi', frames);