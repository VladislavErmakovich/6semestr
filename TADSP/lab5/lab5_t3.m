f = 2;
w_0 = f*2*pi;
n = 8;

[num, den, k] = butter(n,w_0,'s');

syms w t;
D = prod(1i*w - den(1:8));
H = k*1/D;

h = ifourier(H,w,t);
%disp(h)

h_mod_func = matlabFunction(h, 'Vars', t);

% Построение графика
figure;
fplot(h_mod_func , 'b');
title('Модифицированная импульсная характеристика');
xlabel('Время (с)');
ylabel('Амплитуда');
grid on;
