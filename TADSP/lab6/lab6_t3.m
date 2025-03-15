clear; clc

fs = 4000;
duration = 0.5;  
pause = 0.5; 
t_signal = 0:1/fs:duration;

freqs = [697,1447;
        852,1209;
        770,1336];

dtmf_signals = [];

for i = 1:size(freqs, 1)
    f1 = freqs(i, 1);
    f2 = freqs(i, 2);
    dtmf_signal = sin(2*pi*f1*t_signal) + sin(2*pi*f2*t_signal);
    dtmf_signals = [dtmf_signals, dtmf_signal, zeros(1, round(pause * fs))]; 
end

N = length(dtmf_signals);
X_k = fft(dtmf_signals);
magnitude = abs(X_k);
f = fs*(0:N-1)/N; % Реальные частоты в Гц

P1 = magnitude(1:floor(N/2)+1);
f_one_sided = f(1:floor(N/2)+1);

stem(f_one_sided, P1, 'filled','k','MarkerSize', 4); 
title('Спектр DTMF-сигналов с реальными частотами');
xlabel('Частота (Гц)');
ylabel('Амплитуда');
xlim([0 2000]); 
grid on;
