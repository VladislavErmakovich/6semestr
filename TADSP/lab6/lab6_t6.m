clear; clc

[x,fs] = audioread("010.wav");


start_time = 0.1;          % Начало отрезка (сек)
duration_ms = 250;         % Длительность отрезка (мс)

start_sample = round(start_time * fs) + 1;
end_sample = start_sample + round((duration_ms/1000) * fs) - 1;

x_new = x(start_sample:end_sample);

X_k = fft(x_new);

N = length(x_new);
Px = sum(abs(X_k).^2);


magnitude_sq = abs(X_k).^2; 
epsilon = 0.005;
mask = magnitude_sq/Px > epsilon; % Логическая маска

f_axis = fs*(0:N-1)/N;

figure;
subplot(4,1,1);
stem(f_axis, abs(X_k)/max(abs(X_k)));
title('Спектр амплитуд');
xlabel('Частота (Гц)');

subplot(4,1,2);
stem(f_axis, mask);
title('Выделенные компоненты');
xlabel('Частота (Гц)');

X_reconstructed = X_k .* mask; 
x_reconstructed = ifft(X_reconstructed); 

t = (0:length(x_new)-1)/fs; % Временная ось

subplot(4,1,3);
plot(t, x_new, 'b');
title('Исходный отрезок сигнала');
xlabel('Время, сек');
xlim([0 t(end)]);

subplot(4,1,4);
plot(t, x_reconstructed, 'r');
title('Восстановленный сигнал');
xlabel('Время, сек');
xlim([0 t(end)]);

soundsc(real(x_reconstructed), fs);