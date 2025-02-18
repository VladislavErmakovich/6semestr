f = 2;
w_0 = f*2*pi;
n = 8;

[num, den] = butter(n,w_0,'s');

w = logspace(0,2);
H=freqs(num,den,w); 

subplot(2,1,1)
semilogx(w,abs(H),'b','LineWidth',1);
grid on;
ylabel('$|H(\omega)|$','Interpreter','latex');
xlabel('$\omega$, rad/s','Interpreter','latex');
title('АЧХ фильтра Баттерворта 8-го порядка');

subplot(2,1,2)
semilogx(w,angle(H),'r','LineWidth',1); 
grid on;
ylabel('$arg(H(\omega))$','Interpreter','latex');
xlabel('$\omega$, rad/s','Interpreter','latex');
title('ФЧХ фильтра Баттерворта 8-го порядка');




