clear; close all; clc;
sim = MipSim;

x0 = [0.1 0 0 0]';
    
% A = sim.linearized(x0, 0)
[A,B] = sim.hanging()   

[K,S,P] = lqr(A, B, eye(4), 2);

sim.xyRange([-4 10]);
xVal=[-4 10];
sim.axisVals = [xVal(1) xVal(2) -1 -1+xVal(2)-xVal(1)];

% K

% sim.axisVals

% sim.animateWithConstantTorque(x0, 1,0.05);

% x = sim.run(x0, @(x) 1, 0.01, 10);

sim.animateWithComputedTorque(x0, @(x) 1, 0.01)