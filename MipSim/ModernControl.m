clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(5) 0 0 0]';

dt = 0.005;

[A,B] = sim.linearizedInverted();

C = [1 0 0 0];

sys = ss(A, B, C, 0);
sys = c2d(sys, dt);

Q = diag([1, 1, 1, 1]);

R = 1;

K = lqr(sys, Q, R);

controller = SSController(K);

sim.xyRange([-5 5], -1);
[t, x, u] = sim.run(x0, controller, dt, 5);
sim.animateWithComputedU(x0, controller, dt, 5);
figure;
plot(t, u)