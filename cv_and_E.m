%% load data
fs = 20;

filename = strcat('Outputfiles/Observables.dat');
data = load(filename);
T = 1./data(:, 1);
E = data(:, 2);
cv = data(:, 3);

%% cv
figure(1)
hold on
plot(T,cv, 'x','markersize',8, 'color', 'red')
set(gca, 'fontsize', fs)
set(gca,'TickLabelInterpreter','latex')
ylabel('$C_v/J_1$', 'fontsize', fs, 'interpreter', 'latex')
xlabel('$T/J_1$', 'fontsize', fs, 'interpreter', 'latex')


%% cv
figure(2)
hold on
plot(T,E, 'x','markersize',8)
set(gca,'TickLabelInterpreter','latex')
set(gca, 'fontsize', fs)
ylabel('$E/J_1$', 'fontsize', fs, 'interpreter', 'latex')
xlabel('$T/J_1$', 'fontsize', fs, 'interpreter', 'latex')

