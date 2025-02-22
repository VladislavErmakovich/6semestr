clear workspace
syms w t p1 p2;

% x = (0.1*sin(2*pi*5*t)^2 + cos(2*pi*t))*(heaviside(t)-heaviside(t-9));
x = (0.9*sin(2*pi*5*t)^2 + cos(2*pi*t))*heaviside(t);

f = 2;
w_0 = f*2*pi;
n = 8;

[num, den, k] = butter(n,w_0,'s');

D = prod(1i*w - den(1:n));
% D = (1i*w - p1)*(1i*w - p2);
H = k*1/D;

X_w = fourier(x, t, w);
H_X = X_w * H;


x_new = ifourier(H_X,t);
x_new = subs(x_new,{p1},den(1));
x_new = subs(x_new,{p2},den(2));

%X_new = matlabFunction(x_new, 'Vars', t);
% return
figure
subplot(2,1,1)
fplot(x,[-2,12],'b')
xlabel('$ t $','Interpreter','latex');
ylabel('$ x(t) $','Interpreter','latex');
title('График x(t) до прохождения через фильтр');
grid on;

subplot(2,1,2)
fplot(real(x_new),[-2,12],'b')
xlabel('$ t $','Interpreter','latex');
ylabel('$ x(t) $','Interpreter','latex');
title('График x(t) до прохождения через фильтр');
grid on;