clear workspace

syms t;

x = (0.1*sin(2*pi*5*t)^2 + cos(2*pi*t))*(heaviside(t)-heaviside(t-9));

fplot(x, [-2, 10],'b')
xlabel('$ t, c $','Interpreter','latex');
ylabel('$ x(t) $','Interpreter','latex');
title('График x(t)');
grid on;