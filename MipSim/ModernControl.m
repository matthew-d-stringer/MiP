clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(10) 0 0 0]';

dt = 0.01;
simDt = 0.0005;

[A,B] = sim.linearizedInverted();

C = [1 0 0 0];

sys = ss(A, B, C, 0);
sys = c2d(sys, dt);

Q = diag([20, 0.1, 10, 20]);

R = 10;

K = lqr(sys, Q, R);

controller = SSController(K, dt);
K'

subplot(2, 1, 1);
CompareDifferentThetaErrors(sim, 5:5:15, simDt, 5, controller);
[t, x, u] = sim.run(x0, controller, simDt, 5);
subplot(2, 1, 2);
plot(t, u)
xlabel("Time (seconds)")
ylabel("Voltage")

sim.xyRange([-5 5], -1);
% sim.animateWithComputedU(x0, controller, simDt, 5);