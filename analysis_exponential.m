%% load data
fs = 16;
LL = [4, 10];
v=1.3;
beta = 1/8;

for L = LL
    filename = strcat('Outputfiles/ObservablesL',num2str(L),'.dat');
    data = load(filename);
    T = 1./data(:, 1);
    nstring = data(:, 5);
    delta = 0.2;
    for i= 1:length(nstring)
        if nstring(i) < 0.446+delta
            index = i;
        end
    end
    T_c = T(index);
    gm = data(:, 6);

    %% expo calc
    exp_n = (T - T_c).^(-beta) .* nstring;
    exp_T = L.*(T - T_c).^v;

    figure(1)
    hold on
    plot(exp_T,exp_n, '.')
    set(gca, 'fontsize', fs)
    set(gca,'TickLabelInterpreter','latex')
    xlabel('$|T-T_c|^{\nu} \; L$', 'fontsize', fs, 'interpreter', 'latex')
    ylabel('$|T-T_c|^{-\beta} \; n_{string}$', 'fontsize', fs, 'interpreter', 'latex')
    xlim([0, max(real(exp_T))])
end