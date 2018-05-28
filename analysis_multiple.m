fs = 16

path = '../results/Outputfiles_datum';
deltaJ = zeros(20,1);
deltaJ_str = repmat('',20,1);
J2 = 0.007;
for i = 1:20
    deltaJ(i) = (-0.1) * (i-1) * J2;
    deltaJ_str(i) = num2str(round(deltaJ(i), 3));
end

1. put deltaJ_str to path
2. open observables.dat
T = 1/data(:,1);
nstring = zeros(20,length(T));
gm = zeros(20,length(T));

for i = 1:20
	1. put deltaJ_str to path
	2. open observables.dat
	T = 1/data(:,1);
	nstring(i, :) = data(:, 5);
	gm(i, :) = data(:, 6);
end

figure(1)
title('$n_{string}$','interpreter', 'latex')
surf(T,deltaJ,nstring)
set(gca, 'fontsize', fs)
xlabel('$\delta J / J_2$','interpreter', 'latex', 'fontsize', fs)
ylabel('$T / J_2$','interpreter', 'latex', 'fontsize', fs)
zlabel('$n_{string}$', 'fontsize', fs)

figure(2)
title('$gm$','interpreter', 'latex')
surf(T,deltaJ,gm)
set(gca, 'fontsize', fs)
xlabel('$\delta J / J_2$','interpreter', 'latex', 'fontsize', fs)
ylabel('$T / J_2$','interpreter', 'latex', 'fontsize', fs)
zlabel('$gm$', 'fontsize', fs)