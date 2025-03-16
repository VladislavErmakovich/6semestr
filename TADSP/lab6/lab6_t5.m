 clear; clc

[x,fs] = audioread("010.wav");

Ln = length(x); 
% Параметры анализа 
L=32;    
% перекрытие между фреймами 
N=round(fs/60);  
N_bins=round(N/2)+1;  % число эффективных частотных отсчетов 
w = hann(N);   % оконная функция Ханна для анализа 
 
N_frames=floor((Ln-N)/L); % число фреймов 
 
Spec_lin = zeros(N_bins,N_frames);  % Линейная амплитуда
Spec_log = zeros(N_bins,N_frames);  % Логарифмическая амплитуда

for l = 1:N_frames
    x_frame = x(1+(l-1)*L:(l-1)*L+N);
    x_frame = x_frame.*w;
    
    X_kl = fft(x_frame);
    X_kl = X_kl(1:N_bins)/(N/2);
    
    Spec_lin(:,l) = abs(X_kl);       
    Spec_log(:,l) = 20*log10(abs(X_kl));
end


figure('Position',[100 100 1000 700]);

% Линейная спектрограмма
subplot(3,2,[1 2]);
pcolor(Spec_lin);
shading flat;
colormap(jet);
title('Линейная спектрограмма');
ylabel('Частотный индекс \it k'); 
xlabel('Временной индекс \it l');
colorbar;
set(gca,'CLim',[0, 0.1]);  


subplot(3,2,[3 4]);
pcolor(Spec_log);
shading flat;
colormap(jet);
title('Логарифмическая спектрограмма (дБ)');
ylabel('Частотный индекс \it k'); 
xlabel('Временной индекс \it l');
colorbar;
set(gca,'CLim',[-75 -15]);

% Временной сигнал
subplot(3,2,[5 6]);
plot((0:Ln-1)/fs, x);
xlabel('Время, с');
ylabel('Амплитуда');
xlim([0 (Ln-1)/fs]);