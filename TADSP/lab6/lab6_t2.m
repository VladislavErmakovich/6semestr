clear workspace

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

dtmf_signals = dtmf_signals / max(abs(dtmf_signals));

X_k = DFT(dtmf_signals);
magnitude = abs(X_k);
phase = angle(X_k);

figure;

subplot(2, 2, 1); 
stem(0:length(X_k)-1, magnitude, 'filled', 'k'); 
title('Модуль ДПФ DFT');
xlabel('Частота (k)');
ylabel('|X_k|');
grid on;

subplot(2, 2, 2); 
stem(0:length(X_k)-1, phase, 'filled', 'k'); 
title('Аргумент ДПФ DFT');
xlabel('Частота (k)');
ylabel('∠X_k (рад)');
grid on;

X_k_fft = fft(dtmf_signals);
magnitude_fft = abs(X_k_fft);
phase_fft = angle(X_k_fft);

subplot(2, 2, 3); 
stem(0:length(X_k)-1, magnitude, 'filled'); 
title('Модуль ДПФ fft');
xlabel('Частота (k)');
ylabel('|X_k| fft');
grid on;

subplot(2, 2, 4); 
stem(0:length(X_k)-1, phase, 'filled'); 
title('Аргумент ДПФ fft');
xlabel('Частота (k)');
ylabel('∠X_k (рад) fft');
grid on;


