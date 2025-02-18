t = -2:10;
x = (0.1*sin(2*pi*5.*t).^2 + cos(2*pi.*t)).*(unit_step(t)-unit_step(t-9));

stem(t, x,'b')
xlabel('$ t $','Interpreter','latex');
ylabel('$ x(t) $','Interpreter','latex');
title('График x(t)');
grid on;