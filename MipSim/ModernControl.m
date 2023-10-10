clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(10) 0 0 0]';

dt = 0.005;
simDt = 0.0001;

[A,B] = sim.linearizedInverted();

C = [1 0 0 0];

sys = ss(A, B, C, 0);
sys = c2d(sys, dt);

Q = diag([20, 1, 5, 1]);

R = 1;

K = lqr(sys, Q, R);

controller = SSController(K, dt);

sim.xyRange([-5 5], -1);
% [t, x, u] = sim.run(x0, controller, simDt, 5);
sim.animateWithComputedU(x0, controller, simDt, 5);
% figure;
% plot(t, u)