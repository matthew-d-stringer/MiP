clear all; close all; clc;

I_w = 3.815e-5; % kg*m^2
m_w = 0.033*2; % kg
I_b = 0.836*1/12*((16/100)^2 + (4/100)^2); % kg*m^2
m_b = 0.836; % kg
r   = 0.034; % m
L   = 0.08; % m
g   = 9.81; % m/s^2
s   = 0.012*2; % kg*m
k   = s/(130 * pi/180/60); % N*m*s

A1 = I_w + (m_w + m_b)*r^2;
A2 = m_b*r*L;
A3 = 2*s;
A4 = -2*k;
A5 = 2*k;

B1 = m_b*r*L;
B2 = I_b + m_b*L^2;
B3 = -m_b*g*L;
B4 = -2*s;
B5 = 2*k;
B6 = -2*k;

s = tf('s');

A = A1*s^2 - A4*s;
B = A2*s^2 - A5*s;
C = A3;
D = B1*s^2 - B5*s;
E = B2*s^2 + B3 - B6*s;
F = B4;

clear A1 A2 A3 A4 A5;
clear B1 B2 B3 B4 B5 B6;
clear I_w m_w I_b m_b r L g k;

%G1 = (C - (A*F/D)) / (B - (A*E/D));
G1 = -6.6617*s / ((s+9.778)*(s-9.287)*(s+0.004333));
G2 = ((F*B/C) - E) / (D - (F*A/C));

D1 = -19.572*tf([0.14 1],[0.08 1]);
% D1(z) = 
%
%   119.5462 z + -70.8202
%   ---------------------
%      -3.4904 z + 1
%  
% Sample time: 0.1 seconds
%
% u(k) = 3.4904*u(k-1) -70.8202*e(k) + 119.5462*e(k-1)

D2 = 3.8835*tf([0.36 1], [0.088 1]);
% D2(z) =
%  
%   -49.5016 z + 41.2773
%   ---------------------
%      -3.1153 z + 1
%  
% Sample time: 0.1 seconds
%
% u(k) = 3.1153*u(k-1) + 41.2773*e(k) - 49.5016*e(k-1)