f = 2;
w_0 = f*2*pi;
n = 8;

[num, den, k] = butter(n,w_0,'s');
syms w t;
% Исходная импульсная характеристика
h = ifourier(k / prod(1i*w - den(1:8)), w, t);

% Модификация: сдвиг на 5 Гц
h_mod_sym = h * exp(1i*2*pi*5*t);

% Преобразование в численную функцию
h_mod_func = matlabFunction(h_mod_sym, 'Vars', t);

% Вычисление значений
t_vals = linspace(0, 1, 1000);
h_mod_vals = arrayfun(h_mod_func, t_vals);

% Построение графика
figure;
plot(t_vals, real(h_mod_vals), 'b', t_vals, imag(h_mod_vals), 'r');
legend('Re(h_{mod})', 'Im(h_{mod})');
title('Модифицированная импульсная характеристика');
xlabel('Время (с)');
ylabel('Амплитуда');
grid on;