clear all; close all; clc;

data = readmatrix('curve.csv');

times = data(:,1);

times = times/1000;
times = times - times(1);

vels = data(:,2);

cftool(times, vels)