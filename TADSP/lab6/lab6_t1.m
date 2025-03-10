clear workspace

fs = 4000;
duration = 0.5; % длительность  
pause = 0.5; % пауза между кждым символом
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

sound(dtmf_signals, fs);


audiowrite('lab6_t1_375.wav', dtmf_signals, fs);

time= 0:1/fs:(length(dtmf_signals)-1)/fs; 
figure;
plot(time, dtmf_signals, 'k');
xlabel('Время (с)'); 
ylabel('Амплитуда'); 
title('DTMF сигналы для символов 3-7-5'); 
grid on; 
