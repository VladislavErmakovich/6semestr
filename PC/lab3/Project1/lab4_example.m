%% Модель строит спектрограммы для лабораторной работы №6
[sig_orig,fs] = audioread ('chirp_output.wav');


figure;
%subplot(131);
specgram(sig_orig,512, fs , kaiser(512,7), 475);
set(gca,'Clim', [-65 15]);
xlabel('Время, с');
ylabel('Частота, Гц');
title('Исходный сигнал');
set(gca, 'FontName', 'Times New Roman');
set(gca, 'FontSize', 14);

