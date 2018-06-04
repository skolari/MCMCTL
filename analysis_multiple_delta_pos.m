fs = 16;
n = 20;
path = '..\Results\Outputfiles_2018-06-03_19h\deltaJ_';
deltaJ = zeros(n,1);
deltaJ_str = strings([n,1]);
J2 = 0.07;

for i = 1:n
    deltaJ(i) = (0.1) * (i) * J2;
    deltaJ_str(i) = num2str(round(deltaJ(i), 3));
    deltaJ_str(i) = erase(deltaJ_str(i),".");
end

deltaJ(2) = 0.006;
deltaJ_str(2) = num2str(round(deltaJ(1), 3));
deltaJ_str(2) = erase(deltaJ_str(1),".");



tmppath = path + deltaJ_str(1) + "\"  + "Observables.dat"; 
data = load(tmppath);
T = (1/J2)./data(:,1);
nstring = zeros(length(T),n);
gm = zeros(length(T),n);

for i = 1:n
    tmppath = path + deltaJ_str(i) + "\"  + "Observables.dat"; 
    data = load(tmppath);
	nstring(:, i) = data(:, 5);
	gm(:,i) = data(:, 6);
end
deltaJ  = deltaJ./J2;
[X,Y] = meshgrid(T,deltaJ);
figure(3)
title('$n_{string}$','interpreter', 'latex')
surf(X,Y,nstring','EdgeColor','none','LineStyle','none','FaceLighting','phong')
set(gca, 'fontsize', fs)
set(gca,'Ydir','reverse')

ylabel('$\delta J / J_2$','interpreter', 'latex', 'fontsize', fs)
xlabel('$T / J_2$','interpreter', 'latex', 'fontsize', fs)
zlabel('$n_{string}$', 'fontsize', fs,'interpreter', 'latex')
h = colorbar;
ylabel(h,'$n_{string}$', 'fontsize', fs,'interpreter', 'latex')

figure(4)
title('$gm$','interpreter', 'latex')
surf(X,Y,gm','EdgeColor','none','LineStyle','none','FaceLighting','phong')
set(gca, 'fontsize', fs)
set(gca,'Ydir','reverse')

ylabel('$\delta J / J_2$','interpreter', 'latex', 'fontsize', fs)
xlabel('$T / J_2$','interpreter', 'latex', 'fontsize', fs)
zlabel('$g_m$', 'fontsize', fs,'interpreter', 'latex')
h = colorbar;
ylabel(h,'$g_m$', 'fontsize', fs,'interpreter', 'latex')
