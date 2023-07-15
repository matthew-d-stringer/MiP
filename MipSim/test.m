clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(35) 3 0 0]';
    
[A,B] = sim.linearized(zeros(4,1), 0)
[A,B] = sim.linearizedInverted()

% [K,S,P] = lqr(A, B, diag([1 0.2 1 1]), 2);

% sim.xyRange([-4 10], -1);

% frames = sim.animateWithComputedU(x0, @(x) -K*x, 0.001, 3);
% sim.saveAnimation('Animation.avi', frames);