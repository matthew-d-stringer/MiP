clear; close all; clc; 

dt = 0.0001;

H1 = tf([3, -5, 3], [1, -2, 1], dt);
controller = ZtransformController(H1, [1 0 0 0]);

times = 0:dt:0.06;
u = zeros(1, length(times));
for ii = 1:length(times)
    t = times(ii);
    u(ii) = controller.control(t, [1;0;0;0]);
end

step(H1)

figure;
plot(times,u)
