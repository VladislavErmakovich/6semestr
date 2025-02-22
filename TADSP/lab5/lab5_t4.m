clear workspace

f = 2;
w_0 = f*2*pi;
n = 8;
f_shift = 5;

[num, den, k] = butter(n, w_0, 's');

syms w t;

D = prod(1i*w - den(1:n));

H = k*1/D;

h = ifourier(H, w, t);

h_shift = h * exp(1i*2*pi*f_shift*t);
h_shift_func = matlabFunction(h_shift, 'Vars', t);

% Построение графика
t_vals = linspace(0, 1, 1000);
h_mod_vals = arrayfun(h_shift_func, t_vals);

% Построение графика
figure;
plot(t_vals, real(h_mod_vals), 'b', t_vals, imag(h_mod_vals), 'r');
legend('Re(h_{shift})', 'Im(h_{shift})');
title('Модифицированная импульсная характеристика');
xlabel('t(с)');
ylabel('Амплитуда');
grid on;


