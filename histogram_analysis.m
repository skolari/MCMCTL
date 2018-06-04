fs = 16;
nr = 90;
%path = '..\Results\Outputfiles_2018-05-28_21h\deltaJ_-0137\';
%path = '..\Results\Outputfiles_2018-05-29_00h\deltaJ_0004\';
%path = '..\Results\Outputfiles_2018-06-01_00h\deltaJ_-0133\';
path = '..\Results\Outputfiles_2018-06-03_19h\deltaJ_0133\';

J2 = 0.07;
%path = '..\Results\Outputfiles_2018-05-28-14-47-14\';
path1 = strcat(path,'nr_',num2str(nr),'_Energy.dat');
path2 = strcat(path, 'Observables.dat');
% path = path  + '_Energy.dat';

data = load(path1);
data2 = load(path2);
T = 1/data2(nr,1)/J2;
tit = sprintf('%.4f', T);

figure 
hold on
title(strcat('T=', tit))
hist(data, 100)
