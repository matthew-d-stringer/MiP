clear; close all; clc; 

dt = 0.0001;

H1 = tf([2881.769393, -5512.656553, 2635.884492], [1, -1.7759, 0.7884], dt);
H2 = tf([1.488080471214443], [1, -1], dt);
controller = ZtransformController(H2, [1 0 0 0]);

times = 0:dt:0.06;
u = zeros(1, length(times));
for ii = 1:length(times)
    t = times(ii);
    u(ii) = controller.control(t, [1;0;0;0]);
end

step(H2)

figure;
plot(times,u)
