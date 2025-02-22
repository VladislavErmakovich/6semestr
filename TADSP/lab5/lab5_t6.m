% Очистка рабочего пространства
clear;

% Параметры фильтра
f = 2; % Частота
w_0 = f * 2 * pi; % Угловая частота
n = 8; % Порядок фильтра
f_shift = 5; % Сдвиг частоты

% Определение коэффициентов фильтра Баттерворта
[num, den, k] = butter(n, w_0, 's');

% Определение переменных
syms w t;

% Расчет функции передачи
D = prod(1i * w - den(1:n));
H = k * 1 / D;

% Получение импульсной характеристики
h = ifourier(H, w, t);
h_shift = h * exp(1i * 2 * pi * f_shift * t);
h_shift_func = matlabFunction(h_shift, 'Vars', t);

% Построение графика импульсной характеристики
t_vals = linspace(0, 8, 1000);
h_mod_vals = arrayfun(h_shift_func, t_vals);

figure;
subplot(2,1,1)
plot(t_vals, real(h_mod_vals), 'b', t_vals, imag(h_mod_vals), 'r');
legend('Re(h_{shift})', 'Im(h_{shift})');
title('Модифицированная импульсная характеристика');
xlabel('t(с)');
ylabel('Амплитуда');
grid on;

% Определение сигнала x(t)
heaviside_vals = heaviside(t_vals);
x_vals = (0.1 * sin(2 * pi * 5 * t_vals).^2 + cos(2 * pi * t_vals)) .* heaviside_vals;

% Свертка сигнала x(t) с h_shift(t)
y_vals = conv(x_vals, h_mod_vals, 'same') * (t_vals(2) - t_vals(1)); % Умножаем на шаг по времени


subplot(2,1,2)
plot(t_vals, real(y_vals),'b', t_vals, imag(y_vals),'g'); % Используем только действительную часть
legend('Re(x_{shift})', 'Im(x_{shift})');
title('Сигнал после прохождения через импульсную характеристику');
xlabel('t(с)');
ylabel('Амплитуда');
grid on;


