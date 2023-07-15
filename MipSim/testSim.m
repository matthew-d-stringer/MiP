clear;clc;

m1 = 1; % kg
m2 = 1; % kg

L1 = 0.6; % m
L2 = 0.5; % m

% Sim from 0 to 10s
t = 0:0.1:10;

theta1 = pi/3 .* cos(2*t) + sin(t);
theta2 = pi/6 .* cos(2*t) - 2*sin(t);

% Animation
% figure;
tic;
hL1 = plot([0 L1*sin(theta1(1))],[0 -L1*cos(theta1(1))], 'r-');
hold on;
hB1 = plot(L1*sin(theta1(1)),-L1*cos(theta1(1)), 'bo', 'MarkerFaceColor','b');
hL2 = plot( [
        L1*sin(theta1(1)) 
        L1*sin(theta1(1))+L2*sin(theta2(1))
    ],[
        -L1*cos(theta1(1)) 
        -L1*cos(theta1(1))-L2*cos(theta2(1))
    ], 'r-');
hB2 = plot(L1*sin(theta1(1))+L2*sin(theta2(1)), -L1*cos(theta1(1))-L2*cos(theta2(1)), 'ko', 'MarkerFaceColor','k');

axis([-1.5 1.5 -1.5 1.5]);

for ii=2:length(t)
    set(hL1, 'XData', [ 0 L1*sin(theta1(ii)) ]);
    set(hL1, 'YData', [ 0 -L1*cos(theta1(ii)) ]);

    set(hB1, 'XData', L1*sin(theta1(ii)), 'YData', -L1*cos(theta1(ii)))

    set(hL2, 'XData', [
        L1*sin(theta1(ii)) 
        L1*sin(theta1(ii))+L2*sin(theta2(ii))
    ]);
    set(hL2, 'YData', [
        -L1*cos(theta1(ii)) 
        -L1*cos(theta1(ii))-L2*cos(theta2(ii))
    ]);

    set(hB2, 'XData', L1*sin(theta1(ii))+L2*sin(theta2(ii)), 'YData', -L1*cos(theta1(ii))-L2*cos(theta2(ii)))

    drawnow;
    pause(0.1 - toc);
    tic;
end
toc