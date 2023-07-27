clear; close all; clc;
sim = MipSim;

x0 = [deg2rad(1) 10 0 0]';
    
% [A,B] = sim.linearizedInverted();
    
% [K,S,P] = lqr(A, B, diag([1 0.2 1 1]), 2);
% % [K,S,P] = lqr(A, B, diag([1 0.00001 1 0.00001]), 2);

TF = sim.linearizedU2ThetaTF();
% sisotool(TF)

load("controller.mat");

% pzmap(TF*C/(1+TF*C))

dt = 0.01;

H = c2d(C,dt);
dtTF = c2d(TF,dt);
% pzmap(dtTF*H/(1+dtTF*H))

controller = ZtransformController(H, [1 0 0 0]);
% controller.control(0,x0)

sim.xyRange([-4 10], -1);

% step(H*dtTF/(1+H*dtTF))
frames = sim.animateWithComputedU(x0, controller, 0.001, 3);
% sim.saveAnimation('Animation.avi', frames);


% subplot(1,2,1);
% step(H);
% t = 0:dt:0.5;
% y = zeros(1,length(t));
% pY = 0;
% pT = 0;
% for ii = 1:length(t)
%     % y(ii) = 0.1969*1 - 0.1899*pT + 0.8702 * pY;
%     % y(ii) = controller.InputWeights * [1;pT] - controller.OutputWeights(2:end) * [pY];
%     y(ii) = controller.control(t(ii), [1;0;0;0]);
%     pY = y(ii);
%     pT = 1;
% end
% subplot(1,2,2);
% stairs(t,y)
% title("Step Response")


% [Num, Dem] = tfdata(C)
% Num = Num{1};
% Dem = Dem{1};