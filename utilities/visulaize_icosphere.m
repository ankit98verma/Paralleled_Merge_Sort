%%
clear all;
format long;

set(0,'defaultAxesFontSize', 14);
set(0, 'DefaultLineLineWidth', 2);

epsilon = 1e-6;
%% 
vertices = readtable('../results/gpu_vertices.csv');  
% vertices = readtable('../results/cpu_vertices.csv');  

edges = readtable('../results/cpu_edges.csv');
x = vertices.x;
y = vertices.y;
z = vertices.z;
%% plotting vertices
figure(3);
hold on;
plot3(x, y, z, 'r*');
%% plotting edeges
hold on;
for i = 1:height(edges)
    x_tmp = [edges.x1(i), edges.x2(i)];
    y_tmp = [edges.y1(i), edges.y2(i)]; 
    z_tmp = [edges.z1(i), edges.z2(i)];
    plot3(x_tmp, y_tmp, z_tmp, 'b');
end
saveas(gcf, '../results/icosphere.png');

%%
%tmp opsI am not generating any 
clear;
g_arr= readtable('../results/gpu_arr.csv');  % skips the first three rows of data

g = g_arr.sums;
figure()
plot(0:length(g)-1, g);

c_arr= readtable('../results/cpu_arr.csv');  % skips the first three rows of data
c = c_arr.sums;
figure()
plot(0:length(c)-1, c);

