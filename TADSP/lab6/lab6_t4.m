clear; clc

x1 = [0, -1, 1, -1, 1,-1, 0,0,0];
x2 = [0,0.2, 0.5, -1, 0.5,0.2,0,0,0];
N = length(x1);

X1 = fft(x1);
X2 = fft(x2);
Y= X1.*X2;

y = ifft(Y);

subplot(2,1,1)
stem(0:N-1, real(y), 'r', 'LineWidth', 1.5, 'Marker', 'o')
title('Круговая свертка')
xlabel('Отсчёты')
ylabel('Амплитуда')
xlim([-0.5 8.5])
grid on

M = length(x1) + length(x2) - 1; 
x1_pad = [x1, zeros(1, M - length(x1))]; % Дополнение нулями
x2_pad = [x2, zeros(1, M - length(x2))]; 

X1_pad = fft(x1_pad);
X2_pad = fft(x2_pad);
Y_lin = X1_pad .* X2_pad;
y_lin = ifft(Y_lin);


subplot(2,1,2)
stem(0:M-1, real(y_lin), 'b', 'LineWidth', 1.5, 'Marker', 's')
title('Линейная свертка')
xlabel('Отсчёты')
ylabel('Амплитуда')
xlim([-0.5 16.5])
grid on